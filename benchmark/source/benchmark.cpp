// date headers
#include "date_helper.hpp"
#include "date_v1.hpp"
#include "date_v2.hpp"
#include "date_v3.hpp"
#include "legacy_date_helper.hpp"

// benchmark
#include <benchmark/benchmark.h>

// std
#include <algorithm>
#include <random>
#include <vector>

// Simulates a stream of market settlement dates arriving sequentially.
// DoNotOptimize on `current` is critical: without it the compiler sees a
// constexpr function with a known input and folds the entire call to a
// compile-time constant, making the benchmark measure nothing.

static void BM_GetNextWeekday_V1(benchmark::State &state) {
  date_v1::Date current{2026, 1, 1};
  for (auto _ : state) {
    benchmark::DoNotOptimize(current);
    date_v1::Date next = legacy_date_helper::getNextWeekday(current);
    benchmark::DoNotOptimize(next);
    current = next;
  }
}
BENCHMARK(BM_GetNextWeekday_V1);

static void BM_GetNextWeekday_V2(benchmark::State &state) {
  date_v2::Date current{2026, 1, 1};
  for (auto _ : state) {
    benchmark::DoNotOptimize(current);
    date_v2::Date next = legacy_date_helper::getNextWeekday(current);
    benchmark::DoNotOptimize(next);
    current = next;
  }
}
BENCHMARK(BM_GetNextWeekday_V2);

static void BM_GetNextWeekday_V3(benchmark::State &state) {
  date_v3::Date current = 20260101;
  for (auto _ : state) {
    benchmark::DoNotOptimize(current);
    date_v3::Date next = date_helper::getNextWeekday(current);
    benchmark::DoNotOptimize(next);
    current = next;
  }
}
BENCHMARK(BM_GetNextWeekday_V3);

// ---- sort benchmarks ----
// Generate all dates in [2020-01-01, 2025-12-31], shuffle once, then
// copy+sort each iteration so we always sort unsorted data.

static std::vector<date_v1::Date> makeShuffledV1() {
  std::vector<date_v1::Date> dates;
  date_v1::Date d{2020, 1, 1};
  const date_v1::Date end{2026, 1, 1};
  while (d < end) {
    dates.push_back(d);
    d = legacy_date_helper::nextDay(d);
  }
  // deterministic shuffle
  std::mt19937 rng(42);
  std::shuffle(dates.begin(), dates.end(), rng);
  return dates;
}

static std::vector<date_v2::Date> makeShuffledV2() {
  std::vector<date_v2::Date> dates;
  date_v2::Date d{2020, 1, 1};
  const date_v2::Date end{2026, 1, 1};
  while (d < end) {
    dates.push_back(d);
    d = legacy_date_helper::nextDay(d);
  }
  std::mt19937 rng(42);
  std::shuffle(dates.begin(), dates.end(), rng);
  return dates;
}

static std::vector<date_v3::Date> makeShuffledV3() {
  std::vector<date_v3::Date> dates;
  date_v3::Date d = 20200101;
  const date_v3::Date end = 20260101;
  while (d < end) {
    dates.push_back(d);
    d = date_helper::nextDay(d);
  }
  std::mt19937 rng(42);
  std::shuffle(dates.begin(), dates.end(), rng);
  return dates;
}

static void BM_Sort_V1(benchmark::State &state) {
  const auto shuffled = makeShuffledV1();
  for (auto _ : state) {
    auto copy = shuffled;
    std::sort(copy.begin(), copy.end());
    benchmark::DoNotOptimize(copy);
  }
}
BENCHMARK(BM_Sort_V1);

static void BM_Sort_V2(benchmark::State &state) {
  const auto shuffled = makeShuffledV2();
  for (auto _ : state) {
    auto copy = shuffled;
    std::sort(copy.begin(), copy.end());
    benchmark::DoNotOptimize(copy);
  }
}
BENCHMARK(BM_Sort_V2);

static void BM_Sort_V3(benchmark::State &state) {
  const auto shuffled = makeShuffledV3();
  for (auto _ : state) {
    auto copy = shuffled;
    std::sort(copy.begin(), copy.end());
    benchmark::DoNotOptimize(copy);
  }
}
BENCHMARK(BM_Sort_V3);

BENCHMARK_MAIN();
