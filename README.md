Efficient poker hand evaluator based on prime-number representation of card ranks, and single-bit representation
of card suits (light version, inspired by http://suffe.cool/poker/evaluator.html).

* Deck.h contains the Deck class definition. Cards are dealt from the Deck object, where each card is represented by an integer

c=r+s

where

s=256, 512, 1024, 2048,

r=2,3,5,7,11,13,17,19,23,29,31,37,41.

Deck contains the shuffling and dealing functionality.

* CheckRank.h contains the CheckRank module, which in turn contains functionality to get the rank of any five-card poker hand.
The rank is a number from 0 (Royal Flush) to 7461 (High Card 75432). These 7462 ranks can be grouped into nine categories
(Straight Flush (including Royal Flush), Four of a Kind, Full House, Flush, Straight, Three of a Kind, Two Pair, Pair,
High Card), CheckRank contains functionality to also determine which of these categories the hand belongs to.

During construction, the CheckRank reads the ranks.csv file and prepares the flushes and non-flushes rank lookup tables.
The tables are maps between unique products of five prime numbers and their ranks. It is needed to separate flushes and
non-flushes into different tables (this is the only suit difference which affects the strength of the hand), because
rank-wise the content of the tables is the same for (non-straight Flushes and High Cards), and for (Straight Flushes
and non-flush Straights).

Evaluation of the rank is done as follows. Suppose the five-card hand h={c1,...,c5} is given. First the bitwise AND is
calculated for (c1>>8)&...&(c5>>8) to determine whether the hand is a flush (the output is non-zero iff the hand is a flush).
Then the (non-)flushes are searched for in the (non-)flushes table, by looking up the number (c1*...*c5)&255.

* test.cpp contains the test poker hand evaluation. For the test we chose to reproduce the known poker non-transitivity
property, on the example of 22>AKo>JTs>22.
