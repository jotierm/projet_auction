import numpy as np
import pandas as pd

from orderbook import OrderBook
from candidate import Candidate

class Auction:
    """
    Represents an auction that determines the auction price based on the order book and a reference price.

    Attributes:
        orderbook: The order book containing buy and sell orders
        reference_price: The reference price (used for tie-breaking)
    """
    def __init__(self, orderbook, reference_price):
        self.orderbook = orderbook
        self.reference_price = reference_price
        self._candidates = []

    @property
    def candidates(self):
        """Return a list of candidates for the auction."""
        if not self._candidates:
            for price in self.orderbook.prices:
                self._candidates.append(Candidate(price=price, 
                                            buy_volume=self.orderbook.get_buy_volume(price),
                                            sell_volume=self.orderbook.get_sell_volume(price),
                                            ref_price=self.reference_price))
        return self._candidates

    @classmethod
    def from_csv(cls, path, reference_price):
        orderbook = OrderBook.from_csv(path)
        return cls(orderbook, reference_price)

    @staticmethod
    def get_best_candidates(candidates):
        """Return the best candidates based on the auction specification."""
        if not candidates:
            return []

        # maximize matched quantity.
        best_matched = max(candidate.matched for candidate in candidates)
        candidates = [candidate for candidate in candidates if candidate.matched == best_matched]

        # minimize absolute imbalance.
        best_abs_imbalance = min(abs(candidate.imbalance) for candidate in candidates)
        candidates = [candidate for candidate in candidates if abs(candidate.imbalance) == best_abs_imbalance]

        # get the closest reference price
        ref_distances = [candidate.ref_distance for candidate in candidates if candidate.ref_distance is not None]
        if ref_distances:
            best_distance = min(ref_distances)
            candidates = [candidate for candidate in candidates if candidate.ref_distance == best_distance]

        return candidates

    def tie_break_by_oldest_order(self, candidates):
        if len(candidates) > 1:
            eligible_orders = []
            for candidate in candidates:
                eligible_orders.extend(self.orderbook.get_eligible_orders(candidate.price))

            if eligible_orders:
                oldest_eligible_order = min(eligible_orders, key=lambda order: order.time)

                if oldest_eligible_order.is_buy:
                    return min(candidates, key=lambda candidate: candidate.price)
                if oldest_eligible_order.is_sell:
                    return max(candidates, key=lambda candidate: candidate.price)

        return candidates[0]

    def process_auction(self):

        orders = self.orderbook.orders
        prices = self.orderbook.prices

        if not orders:
            return None # No orders in the order book   

        if not prices:
            return None  # No eligible prices found

        candidates = self.get_best_candidates(self.candidates)
        
        # Tie breaking by the oldest eligible order side if it remains more than one candidate.
        return self.tie_break_by_oldest_order(candidates)

    def execute(self):

        auction_result = self.process_auction()

        if auction_result is None or auction_result.price is None:
            return 0.0, 0, 0

        price = auction_result.price
        crossed_volume = auction_result.matched
        imbalance = auction_result.imbalance

        return float(price), crossed_volume, imbalance