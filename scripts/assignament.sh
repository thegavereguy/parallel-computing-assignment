# Create directory for the results and clear it
mkdir results
rm -rf results/*
mkdir results/benchmarks
mkdir results/cache

# Print information about the cpu
lscpu > results/cpu_info.txt

# Creta a build directory if it does not exist
mkdir build
cd build

# Remove the previous build
# make clean

# Compile the code
cmake ..
make -j8
cd ..

export BENCHMARK_CONFIDENCE_INTERVAL=0.30

# Set the number of samples for the benchmarks
export BENCH_SAMPLES=10

export RUN_CACHE_BENCHMARKS=0

threads=( 1 2 4 8 )
for i in ${threads[@]}; do
	export OMP_NUM_THREADS=$i
	# Run the benchmarks and save the results
	./scripts/run_benchmarks.sh
done

