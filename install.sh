#!/bin/bash
# script to automate the installation of
# the EdMedPhys project Geant4 executable

mkdir test_dir
cd test_dir
git clone https://github.com/gsmith23/EdMedPhysics.git
cd EdMedPhysics
source set_environment.sh
cmake .
make -f Makefile
