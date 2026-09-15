# Order book Auction price calculation program

## 1. Objective

This project implements a simplified financial exchange auction engine.
```
The objective is to parse orders from a CSV file, build an order book and determine the auction cross price with the following algorithm:
- Any buy orders can potentially be matched with any sell orders provided their respective prices are compatible i.e. the buy prices are greater than or equal to the sell prices.
- The auction price (or cross price) must be one of the order prices (notwithstanding market orders).
- Considering all the buy and sell orders, find the price at which the maximum number of shares are matched.
- For any potential cross price, eligible orders are the orders compatible with said cross price i.e. all sell orders cheaper than the potential cross price and all buy orders dearer than the potential cross price are eligible. Market orders be they buy or sell are always compatible with any prices. All other orders are deemed non eligible.
- If there is more than one price, choose the one which minimizes the imbalance i.e. which leaves the minimum number of shares from all eligible orders unfilled. Note that this quantity is signed.
- If there still are multiple possible prices, choose the one closer to the reference price (which is provided as an input value before the algorithm is run).
- Finally if there still are multiple prices, a tie breaker is applied: the lowest price is used if the oldest eligible order is a buy and the highest price is used if the oldest eligible order is a sell.
```

## 2. Input Data

Orders are provided in a CSV file without headers.

Input example:

```python
1527604196773077003,AAPL,S,500,270.5700
1527604199695788161,AAPL,B,100,270.3900
1527604199397997988,AAPL,S,100,0
1527604199974781594,AAPL,S,900,278.00
1527604200211637272,AAPL,B,100,0
```

## 3. Architecture

The project is divided into several classes:

### ```Order```

Represents a single order.

### ```OrderBook```

Stores and manages buy and sell orders.

### ```Candidate```

Represents a candidate cross price and the associated executed volume and imbalance.

### ```Auction```

Implements the auction algorithm and determines the cross price.


## 6. Issues and challenges

###

The first challenge has been to build the structure of the code. I have opted for a class structure with the following logic:

$Order -> OrderBook  -> Auction, Candidate$

With such a structure, the code is easier to read, and one can build short methods to avoid heavy functions or methods. It is also easier to debug because we can quickly step into each method and fix the one breaking the execution.

Another challenge has been to build the auction algorithm and to try to minimize its complexity. An issue I had was if we take the list of prices coming from the csv, we can have doublons. A solution can be to build a set from the list of prices in order to run trough each different prices only one time. Another challenge has been to build the tie break function. Indeed, the tricky part is to get the oldest order in the eligible orders from remaining candidates set only. A solution is to build concatenate the lists of each eligible order for all the remaining candidates. This is why I built a ```Candidate``` class, in order to have the price attribute and with the ```Order``` class, we can easily make a distinction if the oldest price is a buy (resp. a sell) and get the lowest (resp. greatest) candidate price (with the use of an anonymous function ```lambda```).
A big issue has been to clean the code, trying to make the code shorter while avoiding removing essential parts. For exemple, I wanted to shorten this part

```python
    @staticmethod
    def get_best_candidates(candidates):
        """Return the best candidates based on the auction specification."""
        if not candidates:
            return []

        # maximize matched quantity.
        best_matched = max(candidate.matched for candidate in candidates)
        candidates = [candidate for candidate in candidates if candidate.matched == best_matched]

        # minimize absolute imbalance.
        best_abs_imbalance = min(abs(candidate.imbalance) for candidate in candidates)
        candidates = [candidate for candidate in candidates if abs(candidate.imbalance) == best_abs_imbalance]

        # get the closest reference price
        ref_distances = [candidate.ref_distance for candidate in candidates if candidate.ref_distance is not None]
        if ref_distances:
            best_distance = min(ref_distances)
            candidates = [candidate for candidate in candidates if candidate.ref_distance == best_distance]

        return candidates
```
However, the order is important here, because first we have to filter the eligible prices that maximize the matched volume. We need then to apply the other filters. If we do a ```condition1 and ... and condition4``` we will not have the same result, because we need to apply the imbalance condition to the orders that already maximize the matched volume.

## 7. Code compilation

I wanted to have the ```main.py``` executable. A solution can be to use the ```pyInstaller``` library with the following command

```bash 
pyinstaller --onefile --name auction  main.py
```

One can now have access to an executable version of the ```main.py``` without having python installed in our machine. We can then run the code with the following (in windows)

```python
.\auction.exe -i "data\ARKW.csv" -r 52.98
```
Running this command will lead to this result

```python 
Auction Price: 52.93, Crossed Volume: 302446.0, Imbalance: -296.0
```