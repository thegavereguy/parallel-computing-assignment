# change the following variables to the desired values

export BENCHMARK_CONFIDENCE_INTERVAL=0.30
export BENCH_SAMPLES=10
export RUN_CACHE_BENCHMARKS=1
export BUILD_OPTIMIZED=3
export CLEAR_RESULTS=0

threads=( 1 2 4 8 )

######################################################

if [ -z "$BUILD_OPTIMIZED" ]; then
	echo "BUILD_OPTIMIZED is not set. Setting it to 0";
	export BUILD_OPTIMIZED=0;
	build_optimized=0;
else
	build_optimized=$BUILD_OPTIMIZED;
fi

mkdir results > /dev/null

if [ ${CLEAR_RESULTS} == 1 ]; then
	echo "Clearing previous results";
	rm -rf results/*
else
  echo "Keeping previous results";
fi

mkdir results/benchmarks > /dev/null
mkdir results/cache > /dev/null

# Print information about the cpu
lscpu > results/cpu_info.txt

# Creta a build directory if it does not exist
mkdir build > /dev/null
cd build

# Remove the previous build
# make clean

# Compile the code
#
if [ ${build_optimized} == 1 ]; then
	echo "Building with -O1 optimization";
	cmake  -DBUILD_OPTIMIZED_O1=ON -DBUILD_OPTIMIZED_O2=OFF -DBUILD_OPTIMIZED_VEC=OFF ..;
elif [ ${build_optimized} == 2 ]; then
	echo "Building with -O2 optimization";
	cmake  -DBUILD_OPTIMIZED_O1=OFF -DBUILD_OPTIMIZED_O2=ON -DBUILD_OPTIMIZED_VEC=OFF ..;
elif [ ${build_optimized} == 3 ]; then
	echo "Building with SIMD optimization";
	cmake  -DBUILD_OPTIMIZED_O1=OFF -DBUILD_OPTIMIZED_O2=OFF -DBUILD_OPTIMIZED_VEC=ON ..;
elif [ ${build_optimized} == 0 ]; then
	echo "Building with no optimization";
	cmake  -DBUILD_OPTIMIZED_O1=OFF -DBUILD_OPTIMIZED_O2=OFF -DBUILD_OPTIMIZED_VEC=OFF ..;
else
	echo "Invalid BUILD_OPTIMIZED value.";
	exit -2;
fi

make -j4
cd ..

for i in ${threads[@]}; do
	export OMP_NUM_THREADS=$i
	# Run the benchmarks and save the results
	./scripts/run_benchmarks.sh
done

