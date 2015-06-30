# Hardware-Programming-Language-Compiler-and-Simulator


The goal is to build a digital logic simulator using the C++ programming language that is able to simulate 
nontrivial digital systems including central processing units (CPU).

Our simulator simulates digital logics specified as synchronous circuits. A synchronous circuit is made up 
of inputs/outputs, wires, logic gates like and/or/not, and flip-flops driven by the global clock signal. It
represents a finite state machine (FSM) where the current state is the boolean values stored in the flip-flops 
and and there is exactly one state transition per clock cycle. For each clock cycle, the next state and the 
current outputs are computed from the current inputs and the current state using the wires and the logic gates. 
Therefore, one of the tasks of our simulator, named logic simulation, is to determine the current inputs and 
the current state and to compute the current outputs and the next state repeatedly.

Given that hardware designers may use our simulator to simulate many different synchronous circuits, we should
allow them to specify the circuits in a way that is convenient and familiar. While there are many established 
tools including schematic drawing programs and hardware description languages, it is extremely difficult for 
us to build one because of our limited knowledge on both digital logic simulation and C++. Therefore, we decide
to design our own language called EasyVL for the designers to specify the circuits in a textual form. EasyVL is
a simplification of one of the most popular hardware description languages called Verilog so that we can build
a simulator in one semester. Nevertheless, it is powerful enough so that we can simulate any synchronous circuit
once it is specified in EasyVL.
