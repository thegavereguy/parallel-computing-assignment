#!/bin/bash

array=("mat_trans_par" "mat_trans_par_col" "mat_trans_seq");

for i in ${array[@]}; do
		eval "./tests/tests" \"[$i]\" "-r csv" "--benchmark-samples=10"> results/"$i.txt";
done


