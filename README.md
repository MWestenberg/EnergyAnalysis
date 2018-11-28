# Static energy analysis of annotated LLVM bitcode
controling external hardware components 
This is a research project for my master thesis. In this research we propose a language independent
method to statically analyse algorithms and provide feedback to a programmer on its
energy consumption in terms of Joules. This should allow the programmer to compare
different algorithms in the same or in different languages to make a choice based
on energy consumption. The focus is not on energy consumption in terms of CPU or
memory usage but rather on control systems that control peripherals like climate control
systems, cars, industrial machinery, robots etc. Hardware is modelled as a ?nite
state machine, in which time-dependent and incidental energy consumption can be
incorporated. All calls to hardware are made explicit in the software by annotations
and must be in the form of a component call. The execution-time of non-annotated
instructions will be calculated and function as a multiplier for time-dependent energy
consumption.