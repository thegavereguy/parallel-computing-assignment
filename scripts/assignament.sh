# Create directory for the results and clear it

if [ -z "$BUILD_OPTIMIZED" ]; then
	echo "BUILD_OPTIMIZED is not set. Setting it to 0";
	export BUILD_OPTIMIZED=0;
	build_optimized=0;
else
	build_optimized=$BUILD_OPTIMIZED;
fi

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
#
if [ ${build_optimized} == 1 ]; then
	echo "Building with -O1 optimization";
	cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_OPTIMIZED_O1=ON -DBUILD_OPTIMIZED_O2=OFF ..;
elif [ ${build_optimized} == 2 ]; then
	echo "Building with -O2 optimization";
	cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_OPTIMIZED_O1=OFF -DBUILD_OPTIMIZED_O2=ON ..;
elif [ ${build_optimized} == 0 ]; then
	echo "Building with no optimization";
	cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_OPTIMIZED_O1=OFF -DBUILD_OPTIMIZED_O2=OFF ..;
else
	echo "Invalid BUILD_OPTIMIZED value.";
	exit -2;
fi

make -j4
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

