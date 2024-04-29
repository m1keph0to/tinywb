# tinywb
Decoder for the digital output of the DIY-EFI tinyWB Air Fuel Ratio LSU4.9 lambda sensor controller.

The documentation on the DIY-EFI website is just wrong. I can only assume that in general people connect the 0-5V analog output to their EFI ECU. 

## Documentation from  DIY-EFI
```
   Data Rate: 115200 baud
   Length: 8 bytes
   Data Format :
   byte0 = AFR*10 (so 148 = 14.8 AFR)
   byte1 = Temp in C of the sensor
   byte2-7 = not used
   The TinyWB is based on the SLCFree by 14point7
```
## Decoded output    
Actually the DIY-EFI document is wrong - below is the decode from using a protocol analyser. (Kingst VIS Logic analyzer in decode mode)

### Idle mode 
When the lambda sensor is not connected, the unit outputs something like 
```
1,
200
gap of 50mS and repeats
```
### Active mode 
When the lambda sensor is connected, the unit outputs this pattern
```
   [0] 0.0000000000,1,,   sync ?
   [1] 0.0000867750,200,, afr
   [2] 0.0001735250,41,,  temperature
   [3] 0.0002603000,2,,   filler
   [4] 0.0003471000,3,,   filler
   [5] 0.0004338750,4,,   filler
   [6] 0.0005206750,5,,   filler
   [7] 0.0006074500,6,,   filler
   [8] 0.0006942250,200,, afr again ? at byte 8.
```

This was based on decoding the data stream using a Kingst logic analyzer and referencing the DIY-EFI documentation. 

## Implementation note 
This repo is  the code fragment from a larger project that uses wrappers round an STM32CubeMX built project - this implements an interrupt driven serial port that does not suffer the constraints of the STM32Cube HAL in terms of e.g. needing to specify how many characters to recieve before waiting for them. 
