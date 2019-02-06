@echo off
clang -S -O3 -emit-llvm SoundSystem\test1.c -o SoundSystem\test1.ll
TraceAnalysis.exe SoundSystem\test1.ll -o SoundSystem\test1.bc
lli --force-interpreter -march=avr -mcpu=atmega328p SoundSystem\test1.bc 10 > SoundSystem\test1.trace.txt
EnergyAnalysis.exe SoundSystem\test1.ll -trace=SoundSystem\test1.trace.txt -clockspeed=16
REM copy /y Vacuum\vacuum.ll K:\Dev\EnergyAnalysis\EnergyAnalysis\Tests
REM copy /y Vacuum\vacuum.trace.txt K:\Dev\EnergyAnalysis\EnergyAnalysis\Tests