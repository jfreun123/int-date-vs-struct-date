// date headers
#include "date_helper.hpp"
#include "legacy_date_helper.hpp"
#include "date_v1.hpp"
#include "date_v2.hpp"
#include "date_v3.hpp"

// benchmark
#include <benchmark/benchmark.h>

// Simulates a stream of market settlement dates arriving sequentially.
// DoNotOptimize on `current` is critical: without it the compiler sees a
// constexpr function with a known input and folds the entire call to a
// compile-time constant, making the benchmark measure nothing.

static void BM_GetNextWeekday_V1(benchmark::State& state) {
    date_v1::Date current{2026, 1, 1};
    for (auto _ : state) {
        benchmark::DoNotOptimize(current);
        date_v1::Date next = legacy_date_helper::getNextWeekday(current);
        benchmark::DoNotOptimize(next);
        current = next;
    }
}
BENCHMARK(BM_GetNextWeekday_V1);

static void BM_GetNextWeekday_V2(benchmark::State& state) {
    date_v2::Date current{2026, 1, 1};
    for (auto _ : state) {
        benchmark::DoNotOptimize(current);
        date_v2::Date next = legacy_date_helper::getNextWeekday(current);
        benchmark::DoNotOptimize(next);
        current = next;
    }
}
BENCHMARK(BM_GetNextWeekday_V2);

static void BM_GetNextWeekday_V3(benchmark::State& state) {
    date_v3::Date current = 20260101;
    for (auto _ : state) {
        benchmark::DoNotOptimize(current);
        date_v3::Date next = date_helper::getNextWeekday(current);
        benchmark::DoNotOptimize(next);
        current = next;
    }
}
BENCHMARK(BM_GetNextWeekday_V3);

BENCHMARK_MAIN();
