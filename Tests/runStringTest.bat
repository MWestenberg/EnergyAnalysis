@echo off
clang -S -O3 -emit-llvm StringTest\StringTest.c -o StringTest\int\StringTest.ll
bin\TraceGenerator.exe StringTest\int\StringTest.ll -o StringTest\int\StringTest.bc
lli --force-interpreter -march=avr -mcpu=atmega328p StringTest\int\StringTest.bc baabbaabaabbbaaaabababbbbaaabbaaabbbbaaaabbababbbaaabbbbaaabbabbaaab > StringTest\int\StringTest.trace.txt
bin\EnergyAnalysis.exe StringTest\int\StringTest.ll -trace=StringTest\int\StringTest.trace.txt -clockspeed=16