from dataclasses import dataclass, field


@dataclass
class Candidate:
    """
    Represents a candidate price for the auction.

    Attributes:
        price: candidate price.
        matched: matched volume at this candidate price.
        imbalance: imbalance at this candidate price.
        eligible_orders: List of eligible orders at this candidate price.
        ref_price: reference price used for distance calculation.
    """
    price: float
    buy_volume: int
    sell_volume: int
    ref_price: float | None = None

    @property
    def ref_distance(self):
        if self.ref_price is None:
            return None
        return abs(self.price - self.ref_price)

    @property
    def matched(self):
        return min(self.buy_volume, self.sell_volume)

    @property
    def imbalance(self):
        return self.buy_volume - self.sell_volume