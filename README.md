# int-date-vs-struct-date

Benchmarking three representations of a calendar date in C++23:

| Version | Type | Description |
|---|---|---|
| **V1** | `struct { int year, month, day; }` | Separate `int` fields |
| **V2** | `struct { short year; char month, day; }` | Packed struct (4 bytes) |
| **V3** | `using Date = int; // yyyymmdd` | Single packed integer |

## Results

Machine: Apple M-series, 8 cores, 1400 MHz, clang++ with `-O3 -march=native -funroll-loops -flto=thin`

```
Benchmark                     Time             CPU   Iterations
---------------------------------------------------------------
BM_GetNextWeekday_V1       11.3 ns         11.2 ns     59616580
BM_GetNextWeekday_V2       15.4 ns         15.2 ns     44145934
BM_GetNextWeekday_V3       14.1 ns         14.0 ns     49997500
BM_Sort_V1               146446 ns       145531 ns         4728
BM_Sort_V2               145543 ns       145007 ns         4829
BM_Sort_V3                61343 ns        61140 ns        10979
BM_Serialize_V1             138 ns          137 ns      5041920
BM_Serialize_V2             139 ns          138 ns      5014291
BM_Serialize_V3            70.4 ns         70.0 ns      9901270
BM_Deserialize_V1          90.6 ns         90.3 ns      7664513
BM_Deserialize_V2          89.7 ns         89.5 ns      7647515
BM_Deserialize_V3          88.7 ns         88.3 ns      7943082
```

### Summary

| Operation | Winner | Ratio | Reason |
|---|---|---|---|
| `getNextWeekday` | V1 struct | 1.3× faster than V3 | V3 pays div/mod to unpack; V1 accesses fields directly |
| Sort (~2200 dates) | V3 int | 2.4× faster than V1/V2 | Single integer compare; 3× smaller element (4 vs 12 bytes) |
| Serialize to `"YYYYMMDD"` | V3 int | 2× faster | One `snprintf` conversion (`%08d`) vs three (`%04d%02d%02d`) |
| Deserialize from `"YYYYMMDD"` | Effectively tied | ~1 ns | `sscanf` dominates; arithmetic cost is noise |

**Why finance uses int dates:** the workload is dominated by sort, compare, index, and serialize — all operations where V3 wins. Calendar arithmetic (`getNextWeekday`, `nextDay`) is computed once at startup into a holiday calendar lookup table, not on the hot path.

## Building

```bash
# Configure and build (Debug + static analysis)
cmake --preset=dev
cmake --build --preset=dev

# Run tests
ctest --preset=dev

# Run benchmarks (production flags)
cmake --preset=bench
cmake --build --preset=bench
./build-bench/benchmark/int-vs-struct_benchmark
```

## Sanitizers

```bash
cmake --preset=asan && cmake --build --preset=asan && ctest --preset=asan
cmake --preset=tsan && cmake --build --preset=tsan && ctest --preset=tsan
```

## Code Formatting

```bash
cmake --build build -t format-check
cmake --build build -t format-fix
```

## Project Structure

```
int-date-vs-struct-date/
├── source/
│   ├── date_v1.hpp              # struct { int year, month, day }
│   ├── date_v2.hpp              # struct { short year; char month, day }
│   ├── date_v3.hpp              # using Date = int  (yyyymmdd)
│   ├── date_helper.hpp          # getNextWeekday / nextDay for V3
│   ├── legacy_date_helper.hpp   # getNextWeekday / nextDay for V1/V2 (templated)
│   └── date_common_functions.hpp # dayOfWeek, daysInMonth, isLeapYear
├── test/source/date_test.cpp
├── benchmark/source/benchmark.cpp
└── CMakePresets.json            # dev, asan, tsan, bench presets
```
