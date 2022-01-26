# multi-threading
Implementations of some multi-threading applications.

In this small project. I implemented three applications in multi-threading. They are spinlocks, parallelizing works using multi-threading, and multi-threading with condition variables. Relevants files are in each folder.
## Spinlocks
Spinlock is implemented using __atomic_test_and_set and __atomic_clear instructions.

## Multi-threading
A library can have many articles, each articles can have multitude of words.So, given a target word, how many times it occurs in the whole library?. This problem
is trivial with small number of words and articles, but maybe slow with a large nunber of words. I tried to solve this by dividing the amount of works among a number of threads. Each thread handles its own little "library", then the main thread would aggregate the result. I tested it by benchmarking my solution with a single-threaded program.

## Multi-threading with CV
Producer-Consumer is a classic problem in multi-threading. For example, a server may have some resources, users (consumers) would want to consume them. One approach
to maintain the stability of systems is to ensure at any point in time, the ratio of producers to consumers must be higher than a value. For example, with a ratio of 2 we would want the number of producers must be always at least twice as much as consumers. Here I demonstrate a simple multi-threaded program with arbitrary 
number of producers and consumers. To ensure the ratio is respected when consumers enter or and producers terminate, I utilized condition variables along with mutexes.

