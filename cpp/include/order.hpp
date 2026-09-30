#include <string>

enum class Side {
    BUY,
    SELL
};

class Order {
public:

    Order(long long timestamp, std::string ticker, Side side, int quantity, double price);

    long long getTimestamp() const;
    std::string getTicker() const;
    Side getSide() const;
    double getPrice() const;
    int getQuantity() const;

    bool isBuy() const;
    bool isSell() const;
    bool isMarket() const;

    bool isEligible(double price) const;
    bool isEligibleBid(double price) const;
    bool isEligibleAsk(double price) const;

private:
    long long timestamp_;
    std::string ticker_;
    Side side_;
    double price_;
    int quantity_;
};

