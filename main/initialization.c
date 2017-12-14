//! include all libraries
#include <Arduino.h>
#include <stdint.h>
#include "Linduino.h"
#include "LT_SPI.h"
#include "UserInterface.h"
#include "LTC68031.h"
#include <SPI.h>

int baud = 9600;

//! # of ICs in the daisy chain
const uint8_t TOTAL_IC = 1; 

//! array to store cell voltages
uint16_t cell_voltage[1][4];
/*!<
  The cell codes will be stored in the cell_codes[][12] array in the following format:

  |  cell_codes[0][0]| cell_codes[0][1] |  cell_codes[0][2]|  cell_codes[0][3]| 
  |------------------|------------------|------------------|------------------|
  |IC1 Cell 1        |IC1 Cell 2        |IC1 Cell 3        |  IC1 Cell 4      |
****/

//! array to store temperature voltages
uint16_t temp_voltage[1][2];

//! array to store current values




uint8_t tx_cfg[1][6];
/*!<
  The tx_cfg[][6] stores the LTC6803 configuration data that is going to be written
  to the LTC6803 ICs on the daisy chain. The LTC6803 configuration data that will be
  written should be stored in blocks of 6 bytes. The array should have the following format:

 |  tx_cfg[0][0]| tx_cfg[0][1] |  tx_cfg[0][2]|  tx_cfg[0][3]|  tx_cfg[0][4]|  tx_cfg[0][5]|
 |--------------|--------------|--------------|--------------|--------------|--------------|
 |IC1 CFGR0     |IC1 CFGR1     |IC1 CFGR2     |IC1 CFGR3     |IC1 CFGR4     |IC1 CFGR5     |
*/

uint8_t rx_cfg[1][7];
/*!<
  the rx_cfg[][8] array stores the data that is read back from a LTC6803-1 daisy chain.
  The configuration data for each IC is stored in blocks of 7 bytes. Below is an table illustrating the array organization:

|rx_config[0][0]|rx_config[0][1]|rx_config[0][2]|rx_config[0][3]|rx_config[0][4]|rx_config[0][5]|rx_config[0][6]  |
|---------------|---------------|---------------|---------------|---------------|---------------|-----------------|
|IC1 CFGR0      |IC1 CFGR1      |IC1 CFGR2      |IC1 CFGR3      |IC1 CFGR4      |IC1 CFGR5      |IC1 PEC          |
*/



//! Initializes hardware and variables
void LTC_setup(){

	Serial.begin(baud); //! baud rate
	LTC6803_initialize(); //! initialize LTC6803 hardware
	init_cfg();	//!initializa 6803 configuration
	print_menu();
	// current_reading
}


//! main loop -> probably should move it to the outer main MAIN LOOP??
void LTC_initialize(){

  // if (Serial.available()) //! check for user input 
  // { 
  //   uint32_t user_command;
  //   user_command = read_int(); //! read the user command
  //   Serial.println(user_command);
  //   run_command(user_command);
  // }

  run_command(1);
  run_command(2);
}


uint16_t LTC_cell_voltage_meas(){
  run_command(3);
  run_command(4);
  return cell_voltage;
}


uint16_t LTC_temp_voltage_meas(){
  run_command(5);
  run_command(6);
  return temp_voltage;
}


 /*!************************ Functions in the main loop ******************************/

//! Prints the main menu
void print_menu(){
	  Serial.println(F("Please enter LTC6803 Command"));
  	Serial.println(F("Write Configuration: 1"));
 	  Serial.println(F("Read Configuration: 2"));
  	Serial.println(F("Start Cell Voltage Conversion: 3"));
  	Serial.println(F("Read Cell Voltages: 4"));
  	Serial.println(F("Start Temp Voltage Conversion: 5"));
  	Serial.println(F("Read Temp Voltages: 6"));
  	Serial.println(F("loop cell voltages: 7"));
  	Serial.println(F("Please enter command: "));
  	Serial.println();
}

void run_command(uint32_t cmd){
	int8_t error = 0;
	char input = 0;
	//! execute the following comands according to user input
  	switch (cmd){
    
    	case 1:

	      LTC6803_wrcfg(TOTAL_IC,tx_cfg);
	      print_config();
	      break;

	    case 2:

	      error = LTC6803_rdcfg(TOTAL_IC,rx_cfg);
	      if (error == -1){
	        Serial.println(F("A PEC error was detected in the received data"));
	      }
	      print_rxconfig();
	      break;

	    case 3: //! start cell voltage ADC conversion

	      LTC6803_stcvad();
	      delay(3);
	      Serial.println(F("cell conversion completed"));
	      Serial.println();
	      break;

	    case 4: //! read cell voltages

	      error = LTC6803_rdcv(TOTAL_IC,cell_voltage); //! set to read back all cell voltage registers
	      if (error == -1){
	        Serial.println(F("A PEC error was detected in the received data."))
	       }
	      print_cells();
	      break;

	    case 5: //! start temp voltage ADC conversion

	      LTC6803_sttmpad();
	      delay(3);
	      Serial.println(F("temp conversion completed"));
	      Serial.println();
	      break;

	    case 6: //! real temp voltages

	      error = LTC6803_rdtmp(TOTAL_IC,temp_voltage); // Set to read back all temp registers
	      if (error == -1){
	        Serial.println(F("A PEC error was detected in the received data."));
	      }
	      print_temp();
	      break;

	    case 7: 
	    /*!
	    Continuously measures the LTC6803 cell voltages and print the results to the serial port.
	    The loop can be exited by sending the MCU a 'm' character over the serial link.
	    */

	      Serial.println(F("transmit 'm' to quit"));

	      //LTC6803_wrcfg(TOTAL_IC,tx_cfg);
	      while (input != 'm'){

	        if (Serial.available() > 0){
	          input = read_char();
	        }

	        LTC6803_stcvad();
	        delay(10);

	        error = LTC6803_rdcv(TOTAL_IC,cell_codes);
	        if (error == -1){
	          Serial.println(F("A PEC error was detected in the received data."));
	        }
	        print_cells();


	        if (error == -1){
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

//! Initializes the configuration array
void init_cfg()
{
  for (int i = 0; i<TOTAL_IC; i++)
  {
    tx_cfg[i][0] = 0xF1;
    tx_cfg[i][1] = 0x00 ;
    tx_cfg[i][2] = 0x00 ;
    tx_cfg[i][3] = 0x00 ;
    tx_cfg[i][4] = 0x00 ;
    tx_cfg[i][5] = 0x00 ;
  }


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


//!Prints the configuration data that is going to be written to the LTC6803 to the serial port.
void print_config()
{
  int cfg_pec;

  Serial.println("Written Configuration: ");
  for (int current_ic = 0; current_ic<TOTAL_IC; current_ic++)
  {
    Serial.print(" IC ");
    Serial.print(current_ic+1,DEC);
    Serial.print(": ");
    Serial.print("0x");
    serial_print_hex(tx_cfg[current_ic][0]);
    Serial.print(", 0x");
    serial_print_hex(tx_cfg[current_ic][1]);
    Serial.print(", 0x");
    serial_print_hex(tx_cfg[current_ic][2]);
    Serial.print(", 0x");
    serial_print_hex(tx_cfg[current_ic][3]);
    Serial.print(", 0x");
    serial_print_hex(tx_cfg[current_ic][4]);
    Serial.print(", 0x");
    serial_print_hex(tx_cfg[current_ic][5]);
    Serial.print(", Calculated PEC: 0x");
    cfg_pec = pec8_calc(6,&tx_cfg[current_ic][0]);
    serial_print_hex((uint8_t)(cfg_pec>>8));
    Serial.print(", 0x");
    serial_print_hex((uint8_t)(cfg_pec));
    Serial.println();
  }
  Serial.println();
}


//! Prints the configuration data that was read back from the LTC6803 to the serial port.
void print_rxconfig()
{
  Serial.println("Received Configuration ");
  for (int current_ic=0; current_ic<TOTAL_IC; current_ic++)
  {
    Serial.print(" IC ");
    Serial.print(current_ic+1,DEC);
    Serial.print(": 0x");
    serial_print_hex(rx_cfg[current_ic][0]);
    Serial.print(", 0x");
    serial_print_hex(rx_cfg[current_ic][1]);
    Serial.print(", 0x");
    serial_print_hex(rx_cfg[current_ic][2]);
    Serial.print(", 0x");
    serial_print_hex(rx_cfg[current_ic][3]);
    Serial.print(", 0x");
    serial_print_hex(rx_cfg[current_ic][4]);
    Serial.print(", 0x");
    serial_print_hex(rx_cfg[current_ic][5]);
    Serial.print(", Received PEC: 0x");
    serial_print_hex(rx_cfg[current_ic][6]);

    Serial.println();
  }
  Serial.println();
}

//! Prints Data in a Hex Format
void serial_print_hex(uint8_t data)
{
  if (data < 16)
  {
    Serial.print("0");
    Serial.print((byte)data,HEX);
  }
  else
    Serial.print((byte)data,HEX);
}

