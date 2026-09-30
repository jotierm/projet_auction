#include "auction.hpp"
using namespace std;
#include <iostream>
#include <tuple>
#include <vector>

int main()
{
	OrderBook fileOrderBook({}, {}, {});
	fileOrderBook.loadCSV("data/ARKW.csv");

	const std::vector<Order> orders = fileOrderBook.getOrders();
	std::vector<Order> bids;
	std::vector<Order> asks;

	for (const Order& order : orders) {
		if (order.isBuy()) {
			bids.push_back(order);
		} else {
			asks.push_back(order);
		}
	}

	OrderBook orderBook(orders, bids, asks);
	Auction auction(orderBook, 52.99);
	const std::tuple<double, int, int> result = auction.executeAuction();

	const double auctionPrice = std::get<0>(result);
	const int matched = std::get<1>(result);
	const int imbalance = std::get<2>(result);

	std::cout << "Auction price: " << auctionPrice << '\n';
	std::cout << "matched: " << matched << '\n';
	std::cout << "imbalance: " << imbalance << '\n';

	return 0;
}
