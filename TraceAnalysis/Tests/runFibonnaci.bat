@echo off
clang -S -O3 -emit-llvm Fibonnaci\fibonnaci.c -o Fibonnaci\fibonnaci.ll
TraceAnalysis.exe Fibonnaci\fibonnaci.ll -o Fibonnaci\fibonnaci.bc
lli --force-interpreter -march=avr -mcpu=atmega328p Fibonnaci\fibonnaci.bc > Fibonnaci\fibonnaci.trace.txt
EnergyAnalysis.exe Fibonnaci\fibonnaci.ll -trace=Fibonnaci\fibonnaci.trace.txt -clockspeed=16
REM copy /y Fibonnaci\Fibonnaci.ll K:\Dev\EnergyAnalysis\EnergyAnalysis\Tests
REM copy /y Fibonnaci\Fibonnaci.trace.txt K:\Dev\EnergyAnalysis\EnergyAnalysis\Tests