# tinywb
Decoder for the digital output of the DIY-EFI tinyWB Air Fuel Ratio LSU4.9 lambda sensor controller.

The documentation on the DIY-EFI website is just wrong. I can only assume that in general people connect the 0-5V analog output to their EFI ECU. 

So this was based on decoding the data stream using a Kingst logic analyzer and referencing the DIY-EFI documentation. 

This is the code fragment from a larger project that uses wrappers round an STM32CubeMX built project - this implements an interrupt driven serial port that does not suffer the constraints oth 
