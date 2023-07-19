# HW3 - Hash Chains Threadpooling


#### A single hash chain
1. Calculate the SHA1 hash of a random value
2. Keep hashing the previous hash until we find a hash with *x* leading zero bits

#### The problem
- We create N hashes that need to be separately chained until the required x leading zero bits are found
- Problem generation is designed to take a significant amount of work item Each generated problem gets pushed onto a queue to be picked up for work

#### Your job
- Calculate N hash chains in parallel.
- Start solving hash chains while new problems are still generated!

#### Speed up Requirement :
- Your solution should have a speedup ≥ 10
#### Build the program
- Makefile:  
make
#### Usage of the program
- Sequential:  
./sequential_implementation -n 1000 -p 8 - s 11
- Parallel:  
./student_submission -n 1000 -p 8 - s 11