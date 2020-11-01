#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <ctime>
#include <cmath>
#include "sort.hpp"
using namespace std;

static void BM_bubble_sort(benchmark::State &state)
{
    for (auto _ : state)
    {
        srand(time(0));
        vector<int> test(state.range(0));
        for (int i = 0; i < state.range(0); ++i)
        {
            test[i] = rand() % 10000;
        }
        bubble_sort(test);
    }
    state.SetComplexityN(state.range(0));
}

static void BM_insertion_sort(benchmark::State &state)
{
    for (auto _ : state)
    {
        srand(time(0));
        vector<int> test(state.range(0));
        for (int i = 0; i < state.range(0); ++i)
        {
            test[i] = rand() % 10000;
        }
        insertion_sort(test);
    }
    state.SetComplexityN(state.range(0));
}

static void BM_selection_sort(benchmark::State &state)
{
    for (auto _ : state)
    {
        srand(time(0));
        vector<int> test(state.range(0));
        for (int i = 0; i < state.range(0); ++i)
        {
            test[i] = rand() % 10000;
        }
        selection_sort(test);
    }
    state.SetComplexityN(state.range(0));
}

static void BM_merge_sort(benchmark::State &state)
{
    for (auto _ : state)
    {
        srand(time(0));
        vector<int> test(state.range(0));
        for (int i = 0; i < state.range(0); ++i)
        {
            test[i] = rand() % 10000;
        }
        merge_sort(test);
    }
    state.SetComplexityN(state.range(0));
}

static void BM_quick_sort_extra(benchmark::State &state)
{
    for (auto _ : state)
    {
        srand(time(0));
        vector<int> test(state.range(0));
        for (int i = 0; i < state.range(0); ++i)
        {
            test[i] = rand() % 10000;
        }
        quick_sort_extra(test);
    }
    state.SetComplexityN(state.range(0));
}

static void BM_quick_sort_inplace(benchmark::State &state)
{
    for (auto _ : state)
    {
        srand(time(0));
        vector<int> test(state.range(0));
        for (int i = 0; i < state.range(0); ++i)
        {
            test[i] = rand() % 10000;
        }
        quick_sort_inplace(test);
    }
    state.SetComplexityN(state.range(0));
}

static void BM_std_sort(benchmark::State &state)
{
    for (auto _ : state)
    {
        srand(time(0));
        vector<int> test(state.range(0));
        for (int i = 0; i < state.range(0); ++i)
        {
            test[i] = rand() % 10000;
        }
        sort(test.begin(), test.end());
    }
    state.SetComplexityN(state.range(0));
}

static void custom_args(benchmark::internal::Benchmark *b)
{
    int i = 10;
    while (i <= 1e6)
    {
        b->Arg(i);
        i += pow(10, (int)log10(i));
    }
}

BENCHMARK(BM_bubble_sort)->RangeMultiplier(10)->Apply(custom_args)->Complexity();        //用于注册测试函数
BENCHMARK(BM_insertion_sort)->RangeMultiplier(10)->Apply(custom_args)->Complexity();     //用于注册测试函数
BENCHMARK(BM_selection_sort)->RangeMultiplier(10)->Apply(custom_args)->Complexity();     //用于注册测试函数
BENCHMARK(BM_merge_sort)->RangeMultiplier(10)->Apply(custom_args)->Complexity();         //用于注册测试函数
BENCHMARK(BM_quick_sort_extra)->RangeMultiplier(10)->Apply(custom_args)->Complexity();   //用于注册测试函数
BENCHMARK(BM_quick_sort_inplace)->RangeMultiplier(10)->Apply(custom_args)->Complexity(); //用于注册测试函数
BENCHMARK(BM_std_sort)->RangeMultiplier(10)->Apply(custom_args)->Complexity();           //用于注册测试函数
BENCHMARK_MAIN();                                                                        //程序入口