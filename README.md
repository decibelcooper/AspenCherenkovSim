# Compilation
This is a basic GNU makefile build.  Use `make` to build.

## Prerequisites
This has been tested (briefly) with GEANT4 v10.4.2.

# Basic usage
Successful compilation gives bin/ACE, which is a GEANT4-based simulation.

Example:
```shell
./bin/ACE macro/run1k.mac output.tsv
```

The file `output.tsv` contains beam x and y positions followed by a number of
photoelectrons for each event.
