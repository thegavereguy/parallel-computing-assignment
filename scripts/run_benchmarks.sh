#!/bin/bash

array=( "mat_trans_seq" "mat_trans_seq_cont" "mat_trans_par" "mat_trans_par_col"  "mat_trans_par_unr" "mat_trans_par_blk" "mat_trans_par_blk_cont" "mat_trans_par_sse" "sym_seq" "sym_seq_cont" "sym_par" "sym_par_col" "sym_par_unr" "sym_par_blk" "sym_par_blk_cont");
array2=(0 1 2 3 4 5 6 7 8 9 10);

# check if the OMP_NUM_THREADS is set
# if not set, set it to 1
if [ -z "$OMP_NUM_THREADS" ]; then
	export OMP_NUM_THREADS=1;
fi

if [ -z "$BENCHMARK_SAMPLES" ]; then
	export BENCHMARK_SAMPLES=10;
fi

if [ -z "$BENCHMARK_CONFIDENCE_INTERVAL" ]; then
	export BENCHMARK_CONFIDENCE_INTERVAL=0.30;
fi

if [ -z "$RUN_CACHE_BENCHMARKS" ]; then
	export RUN_CACHE_BENCHMARKS=0;
	run_cache_benchmarks=0;
else
	run_cache_benchmarks=$RUN_CACHE_BENCHMARKS;
fi

if [ -z "$BUILD_OPTIMIZED" ]; then
	export BUILD_OPTIMIZED=0;
	build_optimized=0;
else
	build_optimized=$BUILD_OPTIMIZED;
fi

if [ ${run_cache_benchmarks} == 1 ]; then
	for i in ${array2[@]}; do
		echo "Running embedded cache benchmark for [${array[$i]}] with $OMP_NUM_THREADS threads";
		eval "./build/apps/cache_bench" "$BENCHMARK_SAMPLES" "$i" > results/cache/"${array[$i]}"".$BUILD_OPTIMIZED.$OMP_NUM_THREADS.csv";
	done
elif [ ${run_cache_benchmarks} == 2 ]; then
	for i in ${array2[@]}; do
		echo "Running external cache benchmark for [${array[$i]}] with $OMP_NUM_THREADS threads";
		eval "perf record  -e LLC-stores,LLC-loads,cache-misses " "--output" "results/cache/""${array[$i]}"".$BUILD_OPTIMIZED.$OMP_NUM_THREADS.data" "./build/apps/run" "1" "$i" ;
		eval "perf stat -e LLC-stores,LLC-loads,cache-misses " "-o results/cache/""${array[$i]}"".$BUILD_OPTIMIZED.$OMP_NUM_THREADS.txt" "./build/apps/run" "1" "$i";
	done
else
	echo "Skipping cache benchmarks";
fi


echo "Running $BENCHMARK_SAMPLES samples per benchmark with $BENCHMARK_CONFIDENCE_INTERVAL confidence interval";
for i in ${array[@]}; do
		echo "Running benchmark for [$i] with $OMP_NUM_THREADS threads";
		eval "./build/tests/benchmarks" \"[$i]\" "-r csv" "--benchmark-samples=$BENCHMARK_SAMPLES" "--benchmark-confidence-interval=$BENCHMARK_CONFIDENCE_INTERVAL" > results/benchmarks/"$i"".$BUILD_OPTIMIZED.$OMP_NUM_THREADS.csv";
done

