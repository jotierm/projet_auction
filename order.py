from dataclasses import dataclass


@dataclass
class Order:
    """
    Represents an order in the order book.

    Attributes:
        time: timestamp of the order.
        ticker: instrument ticker.
        side: side of the order ('buy' or 'sell').
        quantity: quantity of the order.
        price: price of the order (None for market orders).
    """
    time: object
    ticker: str
    side: str
    quantity: int
    price: float | None = None

    @property
    def is_market(self):
        return self.price is None
    
    @property
    def is_buy(self):
        return self.side.lower() == 'b'

    @property
    def is_sell(self):
        return self.side.lower() == 's'

    @staticmethod
    def is_eligible(order, price):
        if order.is_market:
            return True
        if order.is_buy:
            return  order.price >= price
        if order.is_sell:
            return order.price <= price
        return False

    @staticmethod
    def is_eligible_bid(order, price):
        return order.is_buy and Order.is_eligible(order, price)

    @staticmethod
    def is_eligible_ask(order, price):
        return order.is_sell and Order.is_eligible(order, price)