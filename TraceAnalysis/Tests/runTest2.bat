@echo off
clang -S -O3 -emit-llvm SoundSystem\test2.c -o SoundSystem\test2.ll
TraceAnalysis.exe SoundSystem\test2.ll -o SoundSystem\test2.bc
lli --force-interpreter -march=avr -mcpu=atmega328p SoundSystem\test2.bc 10 > SoundSystem\test2.trace.txt
EnergyAnalysis.exe SoundSystem\test2.ll -trace=SoundSystem\test2.trace.txt -clockspeed=16
REM copy /y Vacuum\vacuum.ll K:\Dev\EnergyAnalysis\EnergyAnalysis\Tests
REM copy /y Vacuum\vacuum.trace.txt K:\Dev\EnergyAnalysis\EnergyAnalysis\Tests