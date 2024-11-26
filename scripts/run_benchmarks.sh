#!/bin/bash

array=("mat_trans_par" "mat_trans_par_col" "mat_trans_seq");
array2=(1 2 0);

# check if the OMP_NUM_THREADS is set
# if not set, set it to 1
if [ -z "$OMP_NUM_THREADS" ]; then
	export OMP_NUM_THREADS=1;
fi

if [ -z "$BENCHMARK_SAMPLES" ]; then
	export BENCHMARK_SAMPLES=10;
fi

if [ -z "$BENCHMARK_CONFIDENCE_INTERVAL" ]; then
	export BENCHMARK_CONFIDENCE_INTERVAL=0.40;
fi

if [ -z "RUN_CACHE_BENCHMARK" ]; then
	export RUN_CACHE_BENCHMARK=0;
fi

if [ -z "RUN_CACHE_BENCHMARKS" ]; then
	for i in ${array2[@]}; do
		echo "Running cache benchmark for [${array[$i]}] with $OMP_NUM_THREADS threads";
		eval "./build/apps/cache" "$BENCHMARK_SAMPLES" "$i" > results/cache/"${array[$i]}"".$OMP_NUM_THREADS.csv";
	done
else
	echo "Skipping cache benchmarks";
fi


echo "Running $BENCHMARK_SAMPLES samples per benchmark with $BENCHMARK_CONFIDENCE_INTERVAL confidence interval";
for i in ${array[@]}; do
		echo "Running benchmark for [$i] with $OMP_NUM_THREADS threads";
		eval "./build/tests/benchmarks" \"[$i]\" "-r csv" "--benchmark-samples=$BENCHMARK_SAMPLES" "--benchmark-confidence-interval=$BENCHMARK_CONFIDENCE_INTERVAL" > results/benchmarks/"$i"".$OMP_NUM_THREADS.csv";
done



