@echo off
clang -S -O3 -emit-llvm SimpleTest\Simple.c -o SimpleTest\Simple.ll
TraceAnalysis.exe SimpleTest\Simple.ll -o SimpleTest\Simple.bc
lli --force-interpreter -march=avr -mcpu=atmega328p SimpleTest\Simple.bc baabbaabaabbbaaaabababbbbaaabbaaabbbbaaaab > SimpleTest\Simple.trace.txt
EnergyAnalysis.exe SimpleTest\Simple.ll -trace=SimpleTest\Simple.trace.txt -clockspeed=16
REM copy /y Vacuum\vacuum.ll K:\Dev\EnergyAnalysis\EnergyAnalysis\Tests
REM copy /y Vacuum\vacuum.trace.txt K:\Dev\EnergyAnalysis\EnergyAnalysis\Tests