//! include all libraries
#include <Arduino.h>
#include <stdint.h>
#include "Linduino.h"
#include "LT_SPI.h"
#include "UserInterface.h"
#include "LTC68031.h"
#include <SPI.h>

//! # of ICs in the daisy chain
const uint8_t TOTAL_IC = 1; 

//! array to store cell voltages
uint16_t cell_voltage[TOTAL_IC][4];
/*!<
  The cell codes will be stored in the cell_codes[][12] array in the following format:

  |  cell_codes[0][0]| cell_codes[0][1] |  cell_codes[0][2]|  cell_codes[0][3]| 
  |------------------|------------------|------------------|------------------|
  |IC1 Cell 1        |IC1 Cell 2        |IC1 Cell 3        |  IC1 Cell 4      |
****/

//! array to store temperature voltages
uint16_t temp_voltage[TOTAL_IC][2];

//! array to store current values

//! Initializes hardware and variables
void setup(){
	Serial.begin(115200); //! what is this
	LTC6803_initialize(); //! initialize LTC6803 hardware
	// initializa 6803 configuration
	print_menu();
	// current_reading
}


//! main loop
void loop(){

  if (Serial.available()) //! check for user input 
  { 
    uint32_t user_command;
    user_command = read_int(); //! read the user command
    Serial.println(user_command);
    run_command(user_command);
  }
}


 /*!************************ Functions in the main loop ******************************/

//! Prints the main menu
void print_menu()
{
  Serial.println(F("Please enter LTC6803 Command"));
  //Serial.println(F("Write Configuration: 1"));
  //Serial.println(F("Read Configuration: 2"));
  Serial.println(F("Start Cell Voltage Conversion: 1"));
  Serial.println(F("Read Cell Voltages: 2"));
  Serial.println(F("Start Temp Voltage Conversion: 3"));
  Serial.println(F("Read Temp Voltages: 4"));
  Serial.println(F("loop cell voltages: 5"));
  Serial.println(F("Please enter command: "));
  Serial.println();
}

void run_command(uint32_t cmd){
  int8_t error = 0;
  char input = 0;

  //! execute the following comands according to user input
  switch (cmd){

    case 1: //! start cell voltage ADC conversion

      LTC6803_stcvad();
      delay(3);
      Serial.println(F("cell conversion completed"));
      Serial.println();
      break;

    case 2: //! read cell voltages

      error = LTC6803_rdcv(TOTAL_IC,cell_voltage); //! set to read back all cell voltage registers
      if (error == -1){
        Serial.println(F("A PEC error was detected in the received data."))
       }
      print_cells();
      break;

    case 3: //! start temp voltage ADC conversion

      LTC6803_sttmpad();
      delay(3);
      Serial.println(F("temp conversion completed"));
      Serial.println();
      break;

    case 4: //! real temp voltages

      error = LTC6803_rdtmp(TOTAL_IC,temp_voltage); // Set to read back all temp registers
      if (error == -1)
      {
        Serial.println(F("A PEC error was detected in the received data."));
      }
      print_temp();
      break;

    case 5: 
    /*!
    Continuously measures the LTC6803 cell voltages and print the results to the serial port.
    The loop can be exited by sending the MCU a 'm' character over the serial link.
    */

      Serial.println(F("transmit 'm' to quit"));

      //LTC6803_wrcfg(TOTAL_IC,tx_cfg);
      while (input != 'm')
      {
        if (Serial.available() > 0)
        {
          input = read_char();
        }

        LTC6803_stcvad();
        delay(10);

        error = LTC6803_rdcv(TOTAL_IC,cell_codes);
        if (error == -1)
        {
          Serial.println(F("A PEC error was detected in the received data."));
        }
        print_cells();


        if (error == -1)
        {
          Serial.println(F("A PEC error was detected in the received data."));
        }
        // print_rxconfig();

        delay(500);
      }
      print_menu();
      break;

    default:
      Serial.println(F("Incorrect Option"));
      break;

  }
}
   

 /*!************************ Helper functions ******************************/

//! prints cell voltage to the serial port
void print_cells(){

  // curernt IC: since we only have one IC, it is 0
  int current_ic = 0;
  for (int i = 0; i<4; i++){
    Serial.print(" Cell");
    Serial.print(i+1, DEC); // print i+1 in decimal (or base 10) format
    Serial.print(":");
    Serial.print(cell_voltage[current_ic][i]*0.0015,4); 

    //! 0.0015是什么, cell_voltage was assigned to be uint_16, how was it converted to decimal in order to use 4 here?? 
    
    Serial.print(",");
  }

  Serial.println(); //! print another carriage return

}


//! Prints GPIO voltage and Vref2 voltage onto the serial port

void print_temp(){
  
  int current_ic = 0;

  for (int i = 0; i<2, i++){
    Serial.print(" Temp");
    Serial.print(i+1, DEC);
    Serial.print(":");
    Serial.print(temp_voltage[current_ic][i]*0.0015,4);
    Serial.print(",");

  }
    Serial.print(" ITemp");
    Serial.print(":");
    Serial.print((temp_codes[current_ic][2]*0.1875)-274.15,4);
    Serial.println();
}