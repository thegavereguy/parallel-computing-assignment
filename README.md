# Parallel computing assignment

<!--toc:start-->
- [Parallel computing assignment](#parallel-computing-assignment)
  - [Project structure](#project-structure)
  - [Dependencies](#dependencies)
  - [Getting started](#getting-started)
  - [Building](#building)
  - [Execution](#execution)
    - [Manual execution on local machine](#manual-execution-on-local-machine)
      - [Running the whole project](#running-the-whole-project)
      - [Running a single batch](#running-a-single-batch)
      - [Running a single benchmark](#running-a-single-benchmark)
    - [Automatic execution on PBS cluster](#automatic-execution-on-pbs-cluster)
  - [Benchmarks](#benchmarks)
    - [Execution time](#execution-time)
    - [Cache usage](#cache-usage)
      - [Embedded](#embedded)
      - [External](#external)
  - [Optimization](#optimization)
<!--toc:end-->

## Project structure

- `/`
    - `apps/`
    - `include/lib/`
    - `report/`
    - `scripts/`
    - `src/`
    - `tests/`

## Dependencies

- [Catch2](https://github.com/catchorg/Catch2/) - Testing, benchmarking and reporting for execution time.
- [perf](https://perfwiki.github.io/main/) - General benchmarking.
- [perf-cpp](https://github.com/jmuehlig/perf-cpp) - Embedding calls to perf primitives directly in source code.

## Getting started

To get started, clone the repository on your system:
```
git clone https://github.com/thegavereguy/parallel-computing-assignment pc_assignament_dallacia_enrico
cd pc_assignment_dallacia_enrico
```
----

## Building

To build the project manually, first create the directory where the targets will be built:

```
mkdir build
cd build
```

Then configure the build environment using CMake:

```
cmake ..
```

After that, compile the project itself:

```
make
```

---

## Execution

### Manual execution on local machine

Before attempting to run the project manually, please complete the [build steps](#building).
**If executing manually on the cluster, make sure to start an interactive session before compiling and running the code**

```bash
qsub -I -q short_cpuQ
```

#### Running the whole project

To run the whole project on you local machine execute the `assignment.sh` bash script:
```bash
./scripts/assignment.sh
```

This, by default, will compile the project and execute all the tests and benchmarks (for both time and memory/cache).

The following variables in the script can be modified according to the testers needs:
- `BENCHMARK_CONFIDENCE_INTERVAL`: Sets the percentile of results that will be saved and reported. Tests produce value outside the interval are discarded and execution is repeated. This is useful to filter out results that differ too much from the mean (usually due to system inconsistencies). **Example:** A value of 0.30 reports the 30 percentile of the results.
- `BENCH_SAMPLES`: Sets the number of times that each benchmark is run for each function. A value between 5 and 20 produce usable results.
- `RUN_CACHE_BENCHMARKS`: Sets the behavior of the cache benchmark. It can be one of the following values:
    - `0`: The benchmark is skipped entirely.
    - `1`: The embedded cache benchmark is run (more precise). Check [this](#embedded) for more info.
    - `2`: The external cache benchmark is run (higher compatibility). Check [this](#external) for more info.
- `BUILD_OPTIMIZED`: Sets the type of optimization to apply during the compilation stage. Check [this](#optimization) for more info. It can be one of the following options:
    - `0`: No optimization
    - `1`: -O1 optimization
    - `2`: -O2 optimization
- `CLEAR_RESULTS`:
    - `0`: The results from the last runs are kept or overwritten if the optimization level or thread count is unchanged.
    - `1`: All the previous results are cleared.

The *number of threads* used for each iteration can be modifying the values in the `threads` bash array to adapt the execution to the host machine; the default value of `( 1 2 4 8 16 )`, executed each benchmark routing 5 times, respectively with 1, 2, 4, 8 and 16 threads.  

#### Running a single batch

If running all the benchmarks in one go is not needed, it's possible to run a single batch (runs every function for all the set matrix dimensions, but only for a specified number of thread) by directly using the `run_benchmkarks.sh` bash script.
The operations needed are the same as if running the whole project, but specifying the number of threads to use is now possible by exporting the `OMP_NUM_THREADS` variable, by overwriting the default value of `1`.

#### Running a single benchmark

The executable provided after compiling the benchmarks create using Catch2 (it's default path relative to the repos directory is `./build/tests/benchmarks`) allow running benchmarks limited to one or multiple functions, with the various options described above expressed directly as arguments.

```bash
# Run 15 samples of the benchmark of parallel matrix transposition, with a confidence interval of 0.45 and print the result using the custom csv reporter
./build/tests/benchmarks -r csv --benchmark-samples=15 --benchmark-confidence-interval=0.45
```

### Automatic execution on PBS cluster

To start the automated execution of the whole project just submit the PBS script to the system:

**If the directory of the repository is not in the home folder and|or was renamed, make sure to export the PROJECT_DIR environment variable with the directory path relative to the home directory.**
```bash
# if the repo dir is in "/home/name/assignments/enricodallacia_assignment"
export PROJECT_DIR=assignments/enricodallacia_assignment 
```

Similarly to the execution using the bash script, a number of option can be set manually by exporting the respective environment variables.
Here's the default variable that can be overwritten before submitting the file to the queue:

```bash
export BENCHMARK_CONFIDENCE_INTERVAL=0.30
export BENCH_SAMPLES=10
export RUN_CACHE_BENCHMARKS=1
export BUILD_OPTIMIZED=2
export CLEAR_RESULTS=0
```

After that the execution can be started by submitting the PBS format file to the system:

```bash
qsub scripts/assignment.pbs
```

## Benchmarks
The benchmarks are the main part of the project. They are performed automatically according to the options set in the step preceding the execution.
All results are store in the `results` folder, subdivided by typology.
The conditions in which each benchmark was executed can be determined by the filename. It follows this format: `benchmark_name.optimization_type.thread_count.csv`.
This is valid for all tests, except for cache tests when run on the PBS cluster, which are discussed later. 

(Scrivere del run_benchmark.sh)

### Execution time

This category of tests aims at recording the average time that each execution of the function takes in order to complete at the variation of the size of the matrix to be transposed.
The result files are encoded in CSV format and provide the following information for each execution cycle (the minimum number of time that a function is executed can be set with the `BENCH_SAMPLES` variable, check [here](#running-the-whole-project)).
Consequent runs may be needed if one or more result stray out of the confidence interval, usually due to system inconsistencies.

In order to simplify the benchmark operations, the [Catch2](https://github.com/catchorg/Catch2/) library was used, as it provides tools to run large amount of tests and reporting the required data.
The provided `ADVANCED_BENCHMARK` macro was used to set up the ambient needed to run each function and perform the subsequent cleanup, while making sure to only report the actual execution time of the function without taking any overhead into account.
The data is then reported using a custom `Reporter` that encoded into the CSV format.

### Cache usage
talk about  ache tests.
#### Embedded
cpp_perf
#### External
external perf
## Optimization
 gl
