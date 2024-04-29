

#include "libTinyWB/tinyWB.h"

#include "libSerial/serialCalls.H"

#define CHARS_PER_FRAME 9
#define START_CODE 1

/// @brief time between character bursts
static const uint32_t GAP_TIME_MS = 40;


/// @brief Time of last reception of a character
static uint32_t lastCharTime = 0;

static uint8_t characterIndex = 0;

/// @note Documentation from  DIY-EFI
/// Data Rate: 115200 baud
/// Length: 8 bytes
/// Data Format :
/// byte0 = AFR*10 (so 148 = 14.8 AFR)
/// byte1 = Temp in C of the sensor
/// byte2-7 = not used
///  The TinyWB is based on the SLCFree by 14point7
/// actually this document is wrong - below is the decode from using protocol analyser. 
/// when the lambda sensor is not connected, the unit outputs something like 1,200 gap of 50mS and repeats#
/// 
/// when the lambda sensor is connected, the unit outputs this pattern

//[0] 0.0000000000,1,,   sync ?
//[1] 0.0000867750,200,, afr
//[2] 0.0001735250,41,,  temperature
//[3] 0.0002603000,2,,   filler
//[4] 0.0003471000,3,,   filler
//[5] 0.0004338750,4,,   filler
//[6] 0.0005206750,5,,   filler
//[7] 0.0006074500,6,,   filler
//[8] 0.0006942250,200,, afr again ? at byte 8.



// store first two characters, ignore the rest
static char afrCode = 0;
static char tempCode = 0;

char pollTinyWB(struct usart_ctl *instance, float *AFRvalue, float *temperature)
{
    int valid = 0;
    // PollSerial returns non zero if characters are in the serial input buffer 
    while (PollSerial(instance))
    {
        // deal with time-based synchronisation - 8 byte bursts with gaps
        uint32_t now = HAL_GetTick();
        if (now - lastCharTime > GAP_TIME_MS)
        {
            characterIndex = 0;
        }
        lastCharTime = now;
        // GetCharSerial is implemented as a blocking read from a USART : but because it is 
        // only called when PollSerial is true, it is never going to block. 
        char c = GetCharSerial(instance);
        if (characterIndex < CHARS_PER_FRAME)

            switch (characterIndex)
            {
            case 0:
                if (c != 1)
                    goto fail;
                characterIndex++;
                break;
            // for characters 1 and 2 store in buffer
            case 1:
                afrCode = c;
                characterIndex++;
                break;
            case 2:
                tempCode = c;
                characterIndex++;
                break;

            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                if (c != characterIndex - 1)
                    goto fail;
                characterIndex++;
                break;

            case CHARS_PER_FRAME - 1:
                *AFRvalue = afrCode / 10.0;
                *temperature = tempCode;
                valid = 1; // have valid readings
               
            fail:
                characterIndex = 0;
                break;
            }
    }

    return valid;
}
