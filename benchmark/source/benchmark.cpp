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

// ---- serialization benchmarks ----
// Text serialization to/from "YYYYMMDD" (8 chars + null) — the dominant
// wire format in FIX, CSV, and settlement systems.
//
// Serialize: date → char[9]
// Deserialize: char[9] → date
//
// Serialize: V1/V2 win — fields are already split, so digit extraction
// requires no decomposition (symmetric advantage to getNextWeekday).
// V3 must first unpack year/month/day from the packed int, paying extra
// div/mod operations before it can write any digits.
//
// Deserialize: V3 wins — parse the 8-char string into one integer and
// it is immediately a valid V3 date. V1/V2 must then decompose that
// integer into three fields, paying the same penalty seen in
// BM_GetNextWeekday_V3.

static void BM_Serialize_V1(benchmark::State &state) {
  date_v1::Date d{2026, 3, 9};
  char buf[9];
  for (auto _ : state) {
    benchmark::DoNotOptimize(d);
    buf[0] = '0' + d.year / 1000;
    buf[1] = '0' + (d.year / 100) % 10;
    buf[2] = '0' + (d.year / 10) % 10;
    buf[3] = '0' + d.year % 10;
    buf[4] = '0' + d.month / 10;
    buf[5] = '0' + d.month % 10;
    buf[6] = '0' + d.day / 10;
    buf[7] = '0' + d.day % 10;
    buf[8] = '\0';
    benchmark::DoNotOptimize(buf);
  }
}
BENCHMARK(BM_Serialize_V1);

static void BM_Serialize_V2(benchmark::State &state) {
  date_v2::Date d{2026, 3, 9};
  char buf[9];
  for (auto _ : state) {
    benchmark::DoNotOptimize(d);
    buf[0] = '0' + d.year / 1000;
    buf[1] = '0' + (d.year / 100) % 10;
    buf[2] = '0' + (d.year / 10) % 10;
    buf[3] = '0' + d.year % 10;
    buf[4] = '0' + d.month / 10;
    buf[5] = '0' + d.month % 10;
    buf[6] = '0' + d.day / 10;
    buf[7] = '0' + d.day % 10;
    buf[8] = '\0';
    benchmark::DoNotOptimize(buf);
  }
}
BENCHMARK(BM_Serialize_V2);

static void BM_Serialize_V3(benchmark::State &state) {
  date_v3::Date d = 20260309;
  char buf[9];
  for (auto _ : state) {
    benchmark::DoNotOptimize(d);
    int tmp = d;
    buf[7] = '0' + tmp % 10; tmp /= 10;
    buf[6] = '0' + tmp % 10; tmp /= 10;
    buf[5] = '0' + tmp % 10; tmp /= 10;
    buf[4] = '0' + tmp % 10; tmp /= 10;
    buf[3] = '0' + tmp % 10; tmp /= 10;
    buf[2] = '0' + tmp % 10; tmp /= 10;
    buf[1] = '0' + tmp % 10; tmp /= 10;
    buf[0] = '0' + tmp % 10;
    buf[8] = '\0';
    benchmark::DoNotOptimize(buf);
  }
}
BENCHMARK(BM_Serialize_V3);

static void BM_Deserialize_V1(benchmark::State &state) {
  const char *buf = "20260309";
  for (auto _ : state) {
    benchmark::DoNotOptimize(buf);
    int n = (buf[0] - '0') * 10000000 + (buf[1] - '0') * 1000000 +
            (buf[2] - '0') * 100000 + (buf[3] - '0') * 10000 +
            (buf[4] - '0') * 1000 + (buf[5] - '0') * 100 +
            (buf[6] - '0') * 10 + (buf[7] - '0');
    date_v1::Date d{n / 10000, (n / 100) % 100, n % 100};
    benchmark::DoNotOptimize(d);
  }
}
BENCHMARK(BM_Deserialize_V1);

static void BM_Deserialize_V2(benchmark::State &state) {
  const char *buf = "20260309";
  for (auto _ : state) {
    benchmark::DoNotOptimize(buf);
    int n = (buf[0] - '0') * 10000000 + (buf[1] - '0') * 1000000 +
            (buf[2] - '0') * 100000 + (buf[3] - '0') * 10000 +
            (buf[4] - '0') * 1000 + (buf[5] - '0') * 100 +
            (buf[6] - '0') * 10 + (buf[7] - '0');
    date_v2::Date d{static_cast<short>(n / 10000),
                    static_cast<char>((n / 100) % 100),
                    static_cast<char>(n % 100)};
    benchmark::DoNotOptimize(d);
  }
}
BENCHMARK(BM_Deserialize_V2);

static void BM_Deserialize_V3(benchmark::State &state) {
  const char *buf = "20260309";
  for (auto _ : state) {
    benchmark::DoNotOptimize(buf);
    date_v3::Date d = (buf[0] - '0') * 10000000 + (buf[1] - '0') * 1000000 +
                      (buf[2] - '0') * 100000 + (buf[3] - '0') * 10000 +
                      (buf[4] - '0') * 1000 + (buf[5] - '0') * 100 +
                      (buf[6] - '0') * 10 + (buf[7] - '0');
    benchmark::DoNotOptimize(d);
  }
}
BENCHMARK(BM_Deserialize_V3);

BENCHMARK_MAIN();
