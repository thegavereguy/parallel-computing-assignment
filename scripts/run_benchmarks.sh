#!/bin/bash

array=("mat_trans_par" "mat_trans_par_col" "mat_trans_seq");

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

echo "Running $BENCHMARK_SAMPLES samples per benchmark with $BENCHMARK_CONFIDENCE_INTERVAL confidence interval";
for i in ${array[@]}; do
		echo "Running benchmark for [$i] with $OMP_NUM_THREADS threads";
		eval "./build/tests/benchmarks" \"[$i]\" "-r csv" "--benchmark-samples=$BENCHMARK_SAMPLES" "--benchmark-confidence-interval=$BENCHMARK_CONFIDENCE_INTERVAL" > results/benchmarks/"$i"".$OMP_NUM_THREADS.csv";
done


