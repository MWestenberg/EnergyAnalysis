@echo off
clang -S -O3 -emit-llvm Fibonnaci\fibonnaci.c -o Fibonnaci\int\fibonnaci.ll
bin\TraceGenerator.exe Fibonnaci\int\fibonnaci.ll -o Fibonnaci\int\fibonnaci.bc
lli --force-interpreter -march=avr -mcpu=atmega328p Fibonnaci\int\fibonnaci.bc > Fibonnaci\int\fibonnaci.trace.txt
bin\EnergyAnalysis.exe Fibonnaci\int\fibonnaci.ll -trace=Fibonnaci\int\fibonnaci.trace.txt -clockspeed=16