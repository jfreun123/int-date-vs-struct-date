// starter
#include "add.hpp"

// benchmark
#include <benchmark/benchmark.h>

static void BM_Add(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(starter::add(1, 2));
  }
}
BENCHMARK(BM_Add);

BENCHMARK_MAIN();
