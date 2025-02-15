#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include "/home/alberto/Documents/CPP_BOOKS_NOTES/NetworkProgramming/chapter1/printAdapters.h"

using namespace std;

long long measureExecutionTime(void (*func)()) {
    auto start = chrono::high_resolution_clock::now();
    func();
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}

void benchmarkFunction(void (*func)(), const string &name, int iterations) {
    vector<long long> times;
    for (auto i = 0; i < iterations; ++i) {
        long long duration = measureExecutionTime(func);
        times.push_back(duration);
        this_thread::sleep_for(chrono::milliseconds(100)); // Prevent aggressive caching effects
    }
    
    long long total = 0;
    for (auto t : times) total += t;
    double average = static_cast<double>(total) / iterations;
    
    cout << name << " average time over " << iterations << " runs: " << average << " microseconds\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int iterations = 10;
    
    benchmarkFunction(printAdapters_C, "C Version", iterations);
    benchmarkFunction(printAdapters_CPP, "C++ Version", iterations);
    
    return 0;
}