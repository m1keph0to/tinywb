//////////////////////////////////////////////////
/// Header file for tinyWB module
/////////////////////////////////////////////////

#pragma once
#include "main.h"


#if defined __cplusplus
extern "C"
{
#endif

   

    struct usart_ctl; 

    /// @brief Poll the serial port for activity from the tinyWB module
    /// @param uc Serial port structure
    /// @param AFR  Air fuel ratio
    /// @param heaterTemp_ Heater temperature
    /// @return true if the value has been read
    char pollTinyWB(struct usart_ctl *uc, float *AFR, float *heaterTemp_);

#if defined __cplusplus
}
#endif
