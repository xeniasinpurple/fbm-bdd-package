# fbm-bdd-package

Implementation of a basic Binary Decision Diagram package, which was made for the course Formal Proofing Methods at the University Bremen in the winter semester of 2024/2025.

## Compiling
The package needs the [Gnu C Compiler](https://gcc.gnu.org/) to be built with the given Makefile.

## Usage
In this package you have the ability to create BDDs via the If-Then-Else operator, convert them into KFDDs and then output them as dot-files.
All available functions are described inside ``the bdd_node.c``.
The package also offers the functionality to parse Aiger-files and turn them into BDDs. But due to the missing implementation of a variable ordering (besides the natural one), this process will take an exponential amount of time for all adders greater than 8-bit.
