#include "orderbook.hpp"

#include <string>
#include <tuple>
#include <vector>

class Auction {
public:

    struct Candidate {
        double price;
        int buyVolume;
        int sellVolume;
        double refPrice;

        Candidate(double price, int buyVolume, int sellVolume, double refPrice);

        double refDistance() const;
        int matched() const;
        int imbalance() const;
    };

    Auction(OrderBook orderBook, int refPrice);

    std::vector<Candidate> getCandidates();
    std::vector<Candidate> getBestCandidates(std::vector<Candidate> candidates);
    Candidate tieBreakByOldestOrder(std::vector<Candidate> candidates);

    Candidate processAuction();

    std::tuple<double, int, int> executeAuction();

private:

    OrderBook orderBook_;
    int refPrice_;
    std::vector<Candidate> candidates_;

};

