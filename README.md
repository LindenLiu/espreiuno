# Espreuino
The Arduino based PID and control center for Espresso machine.

# Design Principals
1. Extensible: Different people have different sets of hardware, the software should be extensible to adapting different hardware. Like different display, different temperature sensors etc.
2. Efficient: It is a software running on the micro controller so efficiency is important. The code should be small and fast. Which means we may use some of the compilation tricks to include only minimal code on the chip. Functions should be trimmable. 


