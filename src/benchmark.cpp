#include <benchmark/benchmark.h>
#include <cmath>
#include <memory>
#include <vector>
#include <string>

// Test class with const member data
class ConstMemberTest {
private:
    const double x_;
    const double y_;
    mutable double cache_{0.0};  // Mutable allows modification even in const methods
public:
    ConstMemberTest() : x_(3.14159), y_(2.71828) {}
    
    // Uses const member variables
    double heavyComputation() const {
        double result = 0;
        for(int i = 0; i < 100; ++i) {
            result += std::sin(x_) * std::cos(y_);
        }
        return result;
    }

    // Uses mutable cache
    double heavyComputationCached() const {
        if (cache_ == 0.0) {  // Simple cache check
            cache_ = heavyComputation();
        }
        return cache_;
    }
};

// Test class with const methods returning different const-qualified types
class ConstReturnTest {
private:
    std::vector<double> data_{1.0, 2.0, 3.0, 4.0, 5.0};
    std::string name_{"test"};
public:
    // Return by const reference
    const std::vector<double>& getData() const {
        return data_;
    }
    
    // Return by value
    std::vector<double> getDataCopy() const {
        return data_;
    }
    
    // Return by const value (rarely used but possible)
    const std::vector<double> getDataConstCopy() const {
        return data_;
    }
    
    // Const reference to small type
    const std::string& getName() const {
        return name_;
    }
    
    // Small type by value
    std::string getNameCopy() const {
        return name_;
    }
};

// Original test class for baseline comparison
class ConstTest {
private:
    double x_{3.14159};
    double y_{2.71828};
public:
    double heavyComputation() {
        double result = 0;
        for(int i = 0; i < 100; ++i) {
            result += std::sin(x_) * std::cos(y_);
        }
        return result;
    }
    
    double heavyComputation() const {
        double result = 0;
        for(int i = 0; i < 100; ++i) {
            result += std::sin(x_) * std::cos(y_);
        }
        return result;
    }
};

// Benchmark const vs non-const methods
static void BM_ConstVsNonConst(benchmark::State& state) {
    ConstTest test;
    const ConstTest constTest;
    
    for (auto _ : state) {
        if(state.range(0) == 0) {
            benchmark::DoNotOptimize(test.heavyComputation());
        } else {
            benchmark::DoNotOptimize(constTest.heavyComputation());
        }
    }
}

// Benchmark const member variables
static void BM_ConstMembers(benchmark::State& state) {
    ConstMemberTest test;
    
    for (auto _ : state) {
        if(state.range(0) == 0) {
            // Regular computation
            benchmark::DoNotOptimize(test.heavyComputation());
        } else {
            // Cached computation
            benchmark::DoNotOptimize(test.heavyComputationCached());
        }
    }
}

// Benchmark different const return types
static void BM_ConstReturns(benchmark::State& state) {
    const ConstReturnTest test;
    
    for (auto _ : state) {
        switch(state.range(0)) {
            case 0: {
                // Const reference to vector
                const auto& data = test.getData();
                benchmark::DoNotOptimize(&data);  // Use address instead
                break;
            }
            case 1: {
                auto data = test.getDataCopy();
                benchmark::DoNotOptimize(&data);
                break;
            }
            case 2: {
                const auto data = test.getDataConstCopy();
                benchmark::DoNotOptimize(&data);
                break;
            }
            case 3: {
                const auto& name = test.getName();
                benchmark::DoNotOptimize(&name);
                break;
            }
            case 4: {
                auto name = test.getNameCopy();
                benchmark::DoNotOptimize(&name);
                break;
            }
        }
    }
}

// Register benchmarks
BENCHMARK(BM_ConstVsNonConst)->Arg(0)->Arg(1)
    ->Name("Const vs Non-const Method");

BENCHMARK(BM_ConstMembers)->Arg(0)->Arg(1)
    ->Name("Regular vs Cached Const Method");

BENCHMARK(BM_ConstReturns)->Arg(0)->Arg(1)->Arg(2)->Arg(3)->Arg(4)
    ->Name("Different Const Return Types");

BENCHMARK_MAIN();
