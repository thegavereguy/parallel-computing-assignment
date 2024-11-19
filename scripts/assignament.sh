# Create directory for the results and clear it
mkdir results
rm -rf results/*
mkdir results/benchmarks
mkdir results/memory

# Print information about the cpu
lscpu > results/cpu_info.txt

# Creta a build directory if it does not exist
mkdir build
cd build

# Remove the previous build
make clean

# Compile the code
cmake ..
make -j8
cd ..

# Set the number of threads to be used
export OMP_NUM_THREADS=8

export BENCHMARK_CONFIDENCE_INTERVAL=0.30

# Set the number of samples for the benchmarks
export BENCH_SAMPLES=10

export OMP_NUM_THREADS=1
# Run the benchmarks and save the results
./scripts/run_benchmarks.sh

export OMP_NUM_THREADS=2
# Run the benchmarks and save the results
./scripts/run_benchmarks.sh

export OMP_NUM_THREADS=4
# Run the benchmarks and save the results
./scripts/run_benchmarks.sh

export OMP_NUM_THREADS=8
# Run the benchmarks and save the results
./scripts/run_benchmarks.sh

export OMP_NUM_THREADS=16
# Run the benchmarks and save the results
./scripts/run_benchmarks.sh
