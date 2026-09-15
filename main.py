#!/usr/bin/env ipython3

import argparse
from auction import Auction

def main():

    parser = argparse.ArgumentParser(prog="Auction", description="Run an auction based on an order book CSV file and a reference price.")
    parser.add_argument(
        "-i",
        "--input",
        required=True,
        help="Path to the input order book file."
    )
    parser.add_argument(
        "-r",
        "--reference-price",
        required=True,
        type=float,
    )
    args = parser.parse_args()
    try:

        auction = Auction.from_csv(args.input, args.reference_price)
        price, crossed_volume, imbalance = auction.execute()

        print(f"Auction Price: {price}, Crossed Volume: {crossed_volume}, Imbalance: {imbalance}")

    except Exception as e:

        print(f"Error occurred: {e}")

if __name__ == "__main__":
    main()