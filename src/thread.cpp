#include <thread>
#include <iostream>
#include <cmath>
#include <chrono>



    struct Timer {
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    std::chrono::duration<float> duration;
    
    Timer() {
        start = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;

        float ms = duration.count() * 1000.0f;
        std::cout << "\nTimer took " << ms << "ms\n";
    }
};


static bool isFinished = false;

void doWork() {
    // time functions
    using namespace std::literals::chrono_literals;

    while (!isFinished) {
        // CLI Loading animation, terminates when .get() reads.
        std::cout << "\rLoading." << std::flush;
        std::this_thread::sleep_for(1s);
        std::cout << "\rLoading.." << std::flush;
        std::this_thread::sleep_for(1s);
        std::cout << "\rLoading..." << std::flush;
        std::this_thread::sleep_for(1s);
    }
}

bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i <= std::sqrt(n); ++i)
        if (n % i == 0) return false;
    return true;
}

long long countPrimes(long long start, long long end) {
    long long count = 0;
    for (int i = start; i < end; ++i)
        if (isPrime(i)) ++count;
    return count;
}


void parseWorker1() {
    long long firstHalf = countPrimes(2, 100);
    std::cout << "1. Total primes: " << firstHalf << std::endl;
}

void parseWorker2() {
    long long secondHalf = countPrimes(101, 202);
    std::cout << "2. Total primes: " << secondHalf << std::endl;
}

void parseWorker3() {
    long long thirdhalf = countPrimes(203, 400);
    std::cout << "3. Total primes: " << thirdhalf << std::endl;
}


int main() {
    Timer timer;
    std::thread worker1(parseWorker1);
    std::thread worker2(parseWorker2);
    std::thread worker3(parseWorker3);
  //  std::thread worker2();

    // thread will run in the background until this completes
  //  std::cin.get();
   // isFinished = true;

    // tells main thread to wait until worker returns from work
    worker1.join();
    worker2.join();
    worker3.join();
    std::cout << "Finished.\n";


   // std::cin.get();

}

// Single Thread
// Total primes: 78498
// Timer took 838.627ms

/*
# 1 thread
Total primes: 78498
Timer took 838.627ms


# 2 Threads
1. Total primes: 41538
2. Total primes: 36960

Timer took 613.547ms
*/