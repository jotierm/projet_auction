#include "order.hpp"

#include <string>
#include <vector>

class OrderBook {
public:

    OrderBook(std::vector<Order> orders, std::vector<Order> bids, std::vector<Order> asks);

    std::vector<Order> getOrders() const;
    std::vector<Order> getBids() const;
    std::vector<Order> getAsks() const;

    std::vector<double> getPrices() const;

    std::vector<Order> getEligibleOrders(double price);
    std::vector<Order> getEligibleBids(double price);
    std::vector<Order> getEligibleAsks(double price);

    int getBuyVolume(double price);
    int getSellVolume(double price);

    void loadCSV(const std::string& filename);

    
private:

    std::vector<Order> orders_;
    std::vector<Order> bids_;
    std::vector<Order> asks_;

};

