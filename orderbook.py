import numpy as np
import pandas as pd

from order import Order

class OrderBook:
    """
    Represents an order book containing buy and sell orders at close.

    Attributes:
        path (str): Path to the CSV file containing order data.
        buy_orders (list): List of buy orders.
        sell_orders (list): List of sell orders.
        orders (list): List of all orders.
    """

    def __init__(self):

        self.path = None
        self._orders = []
        self._buy_orders = []
        self._sell_orders = []

    @property
    def buy_orders(self):
        return [order for order in self.orders if order.is_buy]

    @property
    def sell_orders(self):
        return [order for order in self.orders if order.is_sell]

    @property
    def orders(self):
        return self._orders

    @property
    def prices(self):
        """Return a sorted list of unique prices from the order book."""
        return sorted({float(order.price) for order in self.orders if not order.is_market})

    @classmethod
    def from_order(cls, order):
        return cls.from_orders([order])

    @classmethod
    def from_orders(cls, orders):
        """Build an OrderBook from an existing list of Order objects."""
        book = cls()
        book._orders = list(orders)
        return book

    @classmethod
    def from_csv(cls, path):
        """
        Build an OrderBook from a CSV file path.
        """
        try:
            ob_df = pd.read_csv(path, header=None, names=['time', 'ticker', 'side', 'quantity', 'price'])
            orders = []

            for index, row in ob_df.iterrows():
                order = Order(
                    time=row['time'],
                    ticker=row['ticker'],
                    side=row['side'],
                    quantity=row['quantity'],
                    price=row['price']
                )
                orders.append(order)

            return cls.from_orders(orders)

        except Exception as e:

            print(f"Error occurred while reading CSV file: {e}")
            return cls()

    def get_eligible_orders(self, price):
        """Return a list of eligible orders for a given price. An order is eligible if it is a market order or if its price is compatible with the given price."""
        return [order for order in self.orders if order.is_eligible(order, price)]

    def get_eligible_bids(self, price):
        """Return a list of eligible buy orders for a given price. A buy order is eligible if it is a market buy order or if its price is greater than or equal to the given price."""
        return [order for order in self.buy_orders if order.is_eligible_bid(order, price)]    

    def get_eligible_asks(self, price):
        """Return a list of eligible sell orders for a given price. A sell order is eligible if it is a market sell order or if its price is less than or equal to the given price."""
        return [order for order in self.sell_orders if order.is_eligible_ask(order, price)]

    def get_buy_volume(self, price):
        """Return the total quantity of eligible buy orders for a given price."""
        return sum(float(order.quantity) for order in self.get_eligible_bids(price))

    def get_sell_volume(self, price):
        """Return the total quantity of eligible sell orders for a given price."""
        return sum(float(order.quantity) for order in self.get_eligible_asks(price))
