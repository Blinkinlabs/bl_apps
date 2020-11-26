# Arduino API

This project demonstrates an Arduino-style API for accessing the major ARM and FPGA functions.

Desired features:
-Dependent on as few external sources as possible (for example, no FreeRTOS)
-No extra build cruft


Working-ish functions:
-digitalRead/digitalWrite
-delay/delayUs
-analogRead

Not yet working:
-pinconfig (works for FPGA mode only)
-AttachInterrupt() / DetachInterrupt() for GPIO pins, FPGA sources
