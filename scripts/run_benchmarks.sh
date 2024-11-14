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

for i in ${array[@]}; do
		eval "./build/tests/tests" \"[$i]\" "-r csv" "--benchmark-samples=$BENCHMARK_SAMPLES"> results/"$i"".$OMP_NUM_THREADS.txt";
done

