# Parallel computing assignment

<!--toc:start-->
- [Parallel computing assignment](#parallel-computing-assignment)
  - [Getting started](#getting-started)
  - [Building](#building)
  - [Execution](#execution)
    - [Manual execution](#manual-execution)
      - [Running the whole project](#running-the-whole-project)
    - [Automatic execution on PBS cluster](#automatic-execution-on-pbs-cluster)
  - [Benchmarks](#benchmarks)
    - [Execution time](#execution-time)
    - [Memory and cache](#memory-and-cache)
      - [Embedded](#embedded)
      - [External](#external)
<!--toc:end-->

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

### Manual execution

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
    - '0': No optimization
    - '1': -O1 optimization
    - '2': -O2 optimization

The number of threads used for each iteration can be modifying the values in the `threads` bash array to adapt the execution to the host machine; the default value of `( 1 2 4 8 16 )`, executed each benchmark routing 5 times, respectively with 1, 2, 4, 8 and 16 threads.  

### Automatic execution on PBS cluster

To start the automated execution of the whole project just submit the PBS script to the system:

**If the directory of the repository is not in the home folder and|or was renamed, make sure to export the PROJECT_DIR environment variable with the directory path relative to the home directory.**
```bash
# if the repo dir is in "/home/name/assignments/enricodallacia_assignment"
export PROJECT_DIR=assignments/enricodallacia_assignment 
```

```bash
qsub scripts/assignment.pbs
```

## Benchmarks
talk about benchmarks
### Execution time
again
### Memory and cache
talk about memory and ache tests.
#### Embedded
cpp_perf
#### External
external perf
## Optimization
