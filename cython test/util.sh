#!/bin/bash

function clean(){
    rm -rf build *.so wrapper.cpp
}

function compile(){
    python setup.py build_ext --inplace
}

function run(){
    python caller.py
}

if [ "$1" == "clean" ]; then
    clean
elif [ "$1" == "compile" ]; then
    clean
    compile
elif [ "$1" == "crun" ]; then
    clean
    compile
    run
elif [ "$1" == "run" ]; then
    run
else
    echo "Invalid argument(s)"
fi
