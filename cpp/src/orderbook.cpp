#include "orderbook.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <vector>

OrderBook::OrderBook(std::vector<Order> orders, std::vector<Order> bids, std::vector<Order> asks)
    : orders_(orders),
      bids_(bids),
      asks_(asks)
{
}

std::vector<Order> OrderBook::getOrders() const 
{
    return orders_;
}

std::vector<Order> OrderBook::getBids() const 
{
    return bids_;
}

std::vector<Order> OrderBook::getAsks() const 
{
    return asks_;   
}

std::vector<double> OrderBook::getPrices() const
{
    std::set<double> uniquePrices;

    for (const Order& order : orders_) {
        if (!order.isMarket()) {
            uniquePrices.insert(order.getPrice());
        }
    }

    return std::vector<double>(uniquePrices.begin(), uniquePrices.end());
}

std::vector<Order> OrderBook::getEligibleOrders(double price) 
{
    std::vector<Order> eligibleOrders;
    for (const Order& order : orders_) {
        if (order.isEligible(price)) {
            eligibleOrders.push_back(order);
        };
    };
    return eligibleOrders;
}

std::vector<Order> OrderBook::getEligibleBids(double price) 
{
    std::vector<Order> eligibleBids;
    for (const Order& order : orders_) {
        if (order.isEligibleBid(price)) {
            eligibleBids.push_back(order);
        };
    };
    return eligibleBids;
}

std::vector<Order> OrderBook::getEligibleAsks(double price) 
{
    std::vector<Order> eligibleAsks;
    for (const Order& order : orders_) {
        if (order.isEligibleAsk(price)) {
            eligibleAsks.push_back(order);
        };
    };
    return eligibleAsks;
}

int OrderBook::getBuyVolume(double price) 
{
    int buyVolume = 0;
    for (const Order& order : bids_) {
        if (order.isEligibleBid(price)) {
          buyVolume += order.getQuantity();  
        };
    };
    return buyVolume;
}

int OrderBook::getSellVolume(double price) 
{
    int sellVolume = 0;
    for (const Order& order : asks_) {
        if (order.isEligibleAsk(price)) {
          sellVolume += order.getQuantity();  
        };
    };
    return sellVolume;
}

void OrderBook::loadCSV(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file");
    }

    std::string line;

    while (std::getline(file, line)) {

        std::stringstream ss(line);
        std::vector<std::string> fields;
        std::string cell;

        while (std::getline(ss, cell, ',')) {
            fields.push_back(cell);
        }

        long long timestamp = std::stoll(fields[0]);

        std::string ticker = fields[1];

        Side side = fields[2] == "B"
                      ? Side::BUY
                      : Side::SELL;

        int quantity = std::stoi(fields[3]);

        bool is_market = fields[4] == "";

        double price = is_market
                         ? 0.0
                         : std::stod(fields[4]);

        Order order(
            timestamp,
            ticker,
            side,
            quantity,
            price
        );

        orders_.push_back(order);
    }
}
