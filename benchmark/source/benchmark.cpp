#include "date_helper.hpp"
#include "date_v1.hpp"
#include "date_v2.hpp"
#include "date_v3.hpp"
#include "legacy_date_helper.hpp"

#include <benchmark/benchmark.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdio>
#include <cstring>
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

// ---- binary serialization benchmarks ----
// Wire format: a single 32-bit int in yyyymmdd encoding — the standard
// representation used by binary protocols (FIX Binary, Cap'n Proto, flat
// memory-mapped files, network buffers).
//
// V3 IS the wire int: serialize = one store, deserialize = one load.
// V1/V2 must pack fields into the int on serialize and unpack on deserialize,
// paying arithmetic that V3 avoids entirely.

static void BM_Serialize_V1(benchmark::State &state) {
  date_v1::Date d{2026, 3, 9};
  std::array<std::byte, sizeof(int)> buf;
  for (auto _ : state) {
    benchmark::DoNotOptimize(d);
    int wire = d.year * 10000 + d.month * 100 + d.day;
    std::memcpy(buf.data(), &wire, sizeof(wire));
    benchmark::DoNotOptimize(buf);
  }
}
BENCHMARK(BM_Serialize_V1);

static void BM_Serialize_V2(benchmark::State &state) {
  date_v2::Date d{2026, 3, 9};
  std::array<std::byte, sizeof(int)> buf;
  for (auto _ : state) {
    benchmark::DoNotOptimize(d);
    int wire = d.year * 10000 + d.month * 100 + d.day;
    std::memcpy(buf.data(), &wire, sizeof(wire));
    benchmark::DoNotOptimize(buf);
  }
}
BENCHMARK(BM_Serialize_V2);

static void BM_Serialize_V3(benchmark::State &state) {
  date_v3::Date d = 20260309;
  std::array<std::byte, sizeof(int)> buf;
  for (auto _ : state) {
    benchmark::DoNotOptimize(d);
    std::memcpy(buf.data(), &d, sizeof(d));
    benchmark::DoNotOptimize(buf);
  }
}
BENCHMARK(BM_Serialize_V3);

static void BM_Deserialize_V1(benchmark::State &state) {
  int wire = 20260309;
  std::array<std::byte, sizeof(wire)> buf;
  std::memcpy(buf.data(), &wire, sizeof(wire));
  for (auto _ : state) {
    benchmark::DoNotOptimize(buf);
    int w;
    std::memcpy(&w, buf.data(), sizeof(w));
    date_v1::Date d{w / 10000, (w / 100) % 100, w % 100};
    benchmark::DoNotOptimize(d);
  }
}
BENCHMARK(BM_Deserialize_V1);

static void BM_Deserialize_V2(benchmark::State &state) {
  int wire = 20260309;
  std::array<std::byte, sizeof(wire)> buf;
  std::memcpy(buf.data(), &wire, sizeof(wire));
  for (auto _ : state) {
    benchmark::DoNotOptimize(buf);
    int w;
    std::memcpy(&w, buf.data(), sizeof(w));
    date_v2::Date d{static_cast<short>(w / 10000),
                    static_cast<char>((w / 100) % 100),
                    static_cast<char>(w % 100)};
    benchmark::DoNotOptimize(d);
  }
}
BENCHMARK(BM_Deserialize_V2);

static void BM_Deserialize_V3(benchmark::State &state) {
  int wire = 20260309;
  std::array<std::byte, sizeof(wire)> buf;
  std::memcpy(buf.data(), &wire, sizeof(wire));
  for (auto _ : state) {
    benchmark::DoNotOptimize(buf);
    date_v3::Date d;
    std::memcpy(&d, buf.data(), sizeof(d));
    benchmark::DoNotOptimize(d);
  }
}
BENCHMARK(BM_Deserialize_V3);

// ---- legacy char* serialization (V3 only) ----
// Some older systems serialize dates as their decimal text representation
// ("20260309") into a char buffer, then parse with sscanf/atoi on read.
// This is a common "it's just a string" shortcut that pays formatting and
// parsing overhead on every I/O call — contrasted with the binary int path
// above which is a single 4-byte store/load with zero arithmetic.

static void BM_Serialize_V3_CharPtr(benchmark::State &state) {
  date_v3::Date d = 20260309;
  char buf[9];
  for (auto _ : state) {
    benchmark::DoNotOptimize(d);
    std::snprintf(buf, sizeof(buf), "%d", d);
    benchmark::DoNotOptimize(buf);
  }
}
BENCHMARK(BM_Serialize_V3_CharPtr);

static void BM_Deserialize_V3_CharPtr(benchmark::State &state) {
  char buf[9] = "20260309";
  for (auto _ : state) {
    benchmark::DoNotOptimize(buf);
    date_v3::Date d = std::atoi(buf);
    benchmark::DoNotOptimize(d);
  }
}
BENCHMARK(BM_Deserialize_V3_CharPtr);

BENCHMARK_MAIN();
