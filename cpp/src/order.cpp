#include "order.hpp"

Order::Order(long long timestamp, std::string ticker, Side side, int quantity, double price)
    : timestamp_(timestamp),
      ticker_(ticker),
      side_(side),
      price_(price),
      quantity_(quantity)
{
}

long long Order::getTimestamp() const
{
    return timestamp_;
}

std::string Order::getTicker() const
{
    return ticker_;
}

Side Order::getSide() const
{
    return side_;
}

double Order::getPrice() const
{
    return price_;
}

int Order::getQuantity() const
{
    return quantity_;
}

bool Order::isBuy() const
{
    return side_ == Side::BUY;
}

bool Order::isSell() const
{
    return side_ == Side::SELL;
}

bool Order::isMarket() const
{
    return price_ == 0.0;
}

bool Order::isEligible(double price) const
{
    if (isMarket()) {
        return true;
    }
    if (isBuy()) {
        return price_ <= price;
    }
    else {
        return price_ >= price;

    }
}

bool Order::isEligibleBid(double price) const
{
    return isBuy() && price_ >= price;
}

bool Order::isEligibleAsk(double price) const
{
    return isSell() && price_ <= price;
}

