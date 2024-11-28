# Parallel computing assignment

<!--toc:start-->
- [Parallel computing assignment](#parallel-computing-assignment)
  - [Getting started](#getting-started)
  - [Building](#building)
  - [Execution](#execution)
    - [Manual execution](#manual-execution)
    - [](#)
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

'''bash
qsub -I -q short_cpuQ
'''

### Automatic execution on PBS cluster

To start the automated execution of the whole project just submit the PBS script to the system:

**If the directory of the repository is not in the home folder and|or was renamed, make sure to export the PROJECT_DIR environment variable with the directory path relative to the home directory.**
'''bash
# if the repo dir is in "/home/name/assignments/enricodallacia_assignment"
export PROJECT_DIR=assignments/enricodallacia_assignment 
'''

'''bash
qsub scripts/assignment.pbs
'''

