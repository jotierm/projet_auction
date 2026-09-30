#include "auction.hpp"

#include <limits>
#include <stdexcept>
#include <tuple>

Auction::Candidate::Candidate(
    double price,
    int buyVolume,
    int sellVolume,
    double refPrice
)
    : price(price),
      buyVolume(buyVolume),
      sellVolume(sellVolume),
      refPrice(refPrice)
{
}

Auction::Auction(OrderBook orderBook, int refPrice)
    : orderBook_(orderBook), 
      refPrice_(refPrice) 
{
}

double Auction::Candidate::refDistance() const {
    return std::abs(refPrice - price);
};

int Auction::Candidate::matched() const {
    return std::min(buyVolume, sellVolume);
};

int Auction::Candidate::imbalance() const {
    return buyVolume - sellVolume;
};

std::vector<Auction::Candidate> Auction::getCandidates() {

    candidates_.clear();
    for (const double& price : orderBook_.getPrices()) {
        Candidate candidate = Candidate(price, 
                                        orderBook_.getBuyVolume(price), 
                                        orderBook_.getSellVolume(price), 
                                        refPrice_);
        candidates_.push_back(candidate);
    }
    return candidates_;
};

std::vector<Auction::Candidate> Auction::getBestCandidates(std::vector<Candidate> candidates) {
    
    std::vector<Candidate> bestCandidates;

    int bestMatched = 0;
    for (const Candidate& candidate : candidates) {
        bestMatched = std::max(bestMatched, candidate.matched());
    }

    for (const Candidate& candidate : candidates) {
        if (candidate.matched() == bestMatched) {
            bestCandidates.push_back(candidate);
        }
    }

    int bestImbalance = std::numeric_limits<int>::max();
    for (const Candidate& candidate : bestCandidates) {
        bestImbalance = std::min(bestImbalance, std::abs(candidate.imbalance()));
    }

    for (const Candidate& candidate : bestCandidates) {
        if (std::abs(candidate.imbalance()) == bestImbalance) {
            bestCandidates.push_back(candidate);
        }
    }

    double bestRefDistance = std::numeric_limits<double>::max();
    for (const Candidate& candidate : bestCandidates) {
        bestRefDistance = std::min(bestRefDistance, candidate.refDistance());
    }

    for (const Candidate& candidate : bestCandidates) {
        if (candidate.refDistance() == bestRefDistance) {
            bestCandidates.push_back(candidate);
        }
    }
    return bestCandidates;
}


Auction::Candidate Auction::tieBreakByOldestOrder(std::vector<Candidate> candidates) {

    Candidate oldestCandidate = candidates.front();
    long long oldestTimestamp = std::numeric_limits<long long>::max();

    for (const Candidate& candidate : candidates) {

        const std::vector<Order> eligibleOrders = orderBook_.getEligibleOrders(candidate.price);

        for (const Order& order : eligibleOrders) {
            if (order.getTimestamp() < oldestTimestamp) {
                oldestTimestamp = order.getTimestamp();
                oldestCandidate = candidate;
            }
        }
    }

    return oldestCandidate;
}

Auction::Candidate Auction::processAuction() {

    std::vector<Candidate> candidates = getCandidates();
    std::vector<Candidate> bestCandidates = getBestCandidates(candidates);
    Candidate candidate = tieBreakByOldestOrder(bestCandidates);
    return candidate;
}

std::tuple<double, int, int> Auction::executeAuction() {
    Candidate auctionResult = processAuction();

    double price = auctionResult.price;
    int matched = auctionResult.matched();
    int imbalance = auctionResult.imbalance();

    return {price, matched, imbalance};
}

