#include <benchmark/benchmark.h>
#include <cstring>

#include "fractional_cascading.hpp"
#include "naive_binary_search.hpp"
#include "generation.hpp"

constexpr long N = 10000; // number of lists
constexpr long M = 8000; // max size of a list

// From https://stackoverflow.com/questions/34460744/flushing-the-cache-to-prevent-benchmarking-fluctiations
void flush_cache()
{
  // Global variables.
  const size_t bigger_than_cachesize = 20 * 1024 * 1024;
  long* p = new long[bigger_than_cachesize];
  // When you want to "flush" cache.
  memset(p, 0, bigger_than_cachesize*sizeof(*p));
  delete[] p;
}

static void BM_binary_search_count_naive(benchmark::State& state)
{
  // Perform setup here
  static const auto binarySearch = BinarySearch(generate_input(N, M));
  for (auto _ : state)
  {
    // This code gets timed
    state.PauseTiming();
    long key = keysDistribution(rng);
    state.ResumeTiming();
    benchmark::DoNotOptimize(binarySearch.count(key));
  }
}

BENCHMARK(BM_binary_search_count_naive)->Unit(benchmark::kMicrosecond);

static void BM_binary_search_count_fractional_cascading(benchmark::State& state)
{
  // Perform setup here
  static const auto cascade = FractionalCascading(generate_input(N, M));
  for (auto _ : state)
  {
    // This code gets timed
    state.PauseTiming();
    long key = keysDistribution(rng);
    state.ResumeTiming();
    benchmark::DoNotOptimize(cascade.count(key));
  }
}

BENCHMARK(BM_binary_search_count_fractional_cascading)->Unit(benchmark::kMicrosecond);

static void BM_binary_search_count_naive_no_cache(benchmark::State& state)
{
  // Perform setup here
  static const auto binarySearch = BinarySearch(generate_input(N, M));
  for (auto _ : state)
  {
    state.PauseTiming();
    long key = keysDistribution(rng);
    flush_cache();
    state.ResumeTiming();
    // This code gets timed
    benchmark::DoNotOptimize(binarySearch.count(key));
  }
}

BENCHMARK(BM_binary_search_count_naive_no_cache)->Unit(benchmark::kMicrosecond)->Iterations(100);

static void BM_binary_search_count_fractional_cascading_no_cache(benchmark::State& state)
{
  // Perform setup here
  static const auto cascade = FractionalCascading(generate_input(N, M));
  for (auto _ : state)
  {
    state.PauseTiming();
    long key = keysDistribution(rng);
    flush_cache();
    state.ResumeTiming();
    // This code gets timed
    benchmark::DoNotOptimize(cascade.count(key));
  }
}

BENCHMARK(BM_binary_search_count_fractional_cascading_no_cache)->Unit(benchmark::kMicrosecond)->Iterations(100);

static void BM_build_naive(benchmark::State& state)
{
  // Perform setup here
  for (auto _ : state)
  {
    state.PauseTiming();
    auto input = generate_input(N, M);
    state.ResumeTiming();
    // This code gets timed
    benchmark::DoNotOptimize(BinarySearch(std::move(input)));
  }
}

BENCHMARK(BM_build_naive)->Unit(benchmark::kSecond);

static void BM_build_fractional_cascading(benchmark::State& state)
{
  // Perform setup here
  static const auto input = generate_input(N, M);
  for (auto _ : state)
  {
    // This code gets timed
    benchmark::DoNotOptimize(FractionalCascading(input));
  }
}

BENCHMARK(BM_build_fractional_cascading)->Unit(benchmark::kSecond);


BENCHMARK_MAIN();
