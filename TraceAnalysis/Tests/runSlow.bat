@echo off
clang -S -O3 -emit-llvm Slow\slow.c -o Slow\slow.ll
TraceAnalysis.exe Slow\slow.ll -o Slow\slow.bc
lli --force-interpreter -march=avr -mcpu=atmega328p Slow\slow.bc > Slow\slow.trace.txt
EnergyAnalysis.exe Slow\slow.ll -trace=Slow\slow.trace.txt -clockspeed=16
REM copy /y Vacuum\vacuum.ll K:\Dev\EnergyAnalysis\EnergyAnalysis\Tests
REM copy /y Vacuum\vacuum.trace.txt K:\Dev\EnergyAnalysis\EnergyAnalysis\Tests