/*!
LTC2662: 5-Channel 16-/12-Bit 300mA SoftSpan Current Source DACs

@verbatim

The LTC2662 is a family of 5-channel, 16-/12-bit current source digital-to-analog converters with precision
integrated reference and accurate full-scale current. The LTC2662 is guaranteed monotonic and offers independent
SoftSpan programmability of the current output range from 3.125mA to 300mA.


@endverbatim

http://www.linear.com/product/LTC2662

http://www.linear.com/product/LTC2662#demoboards

REVISION HISTORY
$Revision: 6237 $
$Date: 2016-12-20 15:09:16 -0800 (Tue, 20 Dec 2016) $

Copyright (c) 2017, Linear Technology Corp.(LTC)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of Linear Technology Corp.

The Linear Technology Linduino is not affiliated with the official Arduino team.
However, the Linduino is only possible because of the Arduino team's commitment
to the open-source community.  Please, visit http://www.arduino.cc and
http://store.arduino.cc , and consider a purchase that will help fund their
ongoing work.
*/

//! @ingroup Digital_to_Analog_Converters
//! @{
//! @defgroup LTC2662 LTC2662 5-Channel 16-/12-Bit 300mA SoftSpan Current Source DACs
//! @}

/*! @file
    @ingroup LTC2662
    LTC2662: 5-Channel 16-/12-Bit 300mA SoftSpan Current Source DACs
*/

#include <stdint.h>
#include <math.h>
#include <Arduino.h>
#include "Linduino.h"
#include "LT_SPI.h"
#include "LTC2662.h"
#include <SPI.h>

int8_t LTC2662_write(uint8_t cs, uint8_t dac_command, uint8_t dac_address, uint16_t dac_code)
// Write the 16-bit dac_code to the LTC2662
{
  uint8_t data_array[4], rx_array[4];
  int8_t ret;
  LT_union_int16_2bytes data;

  data.LT_int16 = dac_code;                              // Copy DAC code to union
  data_array[3] = 0;                                     // Only required for 32 byte readback transaction
  data_array[2] = dac_command | dac_address;             // Build command / address byte
  data_array[1] = data.LT_byte[1];                       // MS Byte
  data_array[0] = data.LT_byte[0];                       // LS Byte

  spi_transfer_block(cs, data_array, rx_array, (uint8_t) 4);

  return(rx_array[3]); // Returns the fault registers
}

uint16_t LTC2662_current_to_code(float dac_current, float max_output)
// Calculate a LTC2662 DAC code given the desired output current and the maximum output
// output for a given softspan range.
{
  uint16_t dac_code;
  float float_code;
  float_code = 65535.0 * dac_current / max_output;                                                  // Calculate the DAC code
  float_code = (float_code > (floor(float_code) + 0.5)) ? ceil(float_code) : floor(float_code);     // Round
  if (float_code < 0.0) float_code = 0.0;
  if (float_code > 65535.0) float_code = 65535.0;
  dac_code = (uint16_t) (float_code);                                                               // Convert to unsigned integer
  return (dac_code);
}

float LTC2662_code_to_current(uint16_t dac_code, float max_output)
// Calculate the LTC2662 DAC output current given the DAC code and and the minimum
// output for a given softspan range.
{
  float dac_current;
  dac_current = ((float) dac_code / 65535.0) * max_output;     // Calculate the dac_current
  return (dac_current);
}