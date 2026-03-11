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
BM_GetNextWeekday_V1        12.2 ns         11.6 ns     63974264
BM_GetNextWeekday_V2        15.3 ns         15.2 ns     45757914
BM_GetNextWeekday_V3        14.0 ns         13.9 ns     50154045
BM_Sort_V1               157764 ns       156400 ns         4459
BM_Sort_V2               144113 ns       143552 ns         4654
BM_Sort_V3                59245 ns        59165 ns        11334
BM_Serialize_V1             5.44 ns         5.43 ns    122619861
BM_Serialize_V2             5.91 ns         5.90 ns    113178871
BM_Serialize_V3             8.58 ns         8.57 ns     80127288
BM_Deserialize_V1           4.28 ns         4.27 ns    167363617
BM_Deserialize_V2           4.21 ns         4.21 ns    162257888
BM_Deserialize_V3           1.89 ns         1.88 ns    367245866
```

### Summary

| Operation | Winner | Ratio | Reason |
|---|---|---|---|
| `getNextWeekday` | V1 struct | 1.3× faster than V3 | V3 pays div/mod to unpack; V1 accesses fields directly |
| Sort (~2200 dates) | V3 int | 2.6× faster than V1/V2 | Single integer compare; 3× smaller element (4 vs 12 bytes) |
| Binary serialize | V3 int | fastest | V3 IS the wire int — one store; V1/V2 pay pack arithmetic |
| Binary deserialize | V3 int | fastest | V3 IS the wire int — one load; V1/V2 pay unpack arithmetic |

**Why finance uses int dates:** the hot path is sort, compare, and binary I/O — all V3 wins or ties V2. Calendar arithmetic (`getNextWeekday`, `nextDay`) is computed once at startup into a holiday calendar lookup table, so its overhead is amortised away.

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
