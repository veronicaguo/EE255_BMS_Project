/*!
  EE255 BMS main loop.
*/

//! include all libraries
#include <Arduino.h>
#include <stdint.h>
#include "Linduino.h"
#include "LT_SPI.h"
#include "LTC68031.h"
#include <SPI.h>
//#include "cv_charge.h"
//#include "cc_charge.h"
//#include "conversion.h"
//#include "PWM_GEN.h"

float duty = 0;
float cell_voltage_conv[12];
int mode;
int i;
int j;
uint16_t cell_voltage_meas[50][12];

// initialization variables
int baud = 9600;
const uint8_t TOTAL_IC = 1; 
uint16_t cell_voltage[TOTAL_IC][12];
uint16_t temp_voltage[TOTAL_IC][3];
uint8_t tx_cfg[TOTAL_IC][6];
uint8_t rx_cfg[TOTAL_IC][7];

// conversion variables
uint16_t vsum = 0;
uint16_t vavg[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float vreal[12];
float VSLOPE = 100;
float VZERO = 100;

void setup() {
  Serial.begin(baud);
  Serial.println("Time Cell1 Cell2 Cell3 Cell4");
}

int main(){
	
	// PWM setup and initialization
	//pwmsetup();

	// LTC setup and initialization
	LTC_setup();	
	LTC_initialize();	

	// main loop
	while(1){
		// PWM assignment
		//pwmgen(duty);

		// measure cell voltages and temp voltages
    LTC_cell_voltage_meas();
    
		for (i = 0; i < 50; i++){
      for (j = 0; j < 12; j++){
        cell_voltage_meas[i][j] = cell_voltage[0][j];
      }      
		}
    /*
    for (i = 0; i < 4; i++){
      cell_voltage_conv[i] = *(vconv(cell_voltage_meas) + i);
      Serial.println(cell_voltage_conv[i]);
    }		 
    */
    Serial.print(millis());
    Serial.print(" ");
    for (i = 0; i < 12; i++){
      Serial.print(cell_voltage[0][i]*0.0015,4);
      Serial.print(" ");
    }
    Serial.println();

		//Serial.println(outputvals);

		//duty = check_state(cell_voltage, cell_current, mode);	
    delay(100);
	}

	return 0;
}

//! Initialization



//! Initializes hardware and variables

void LTC_setup(){
  //spi_enable(SPI_CLOCK_DIV4);
  SPI.begin(); //! baud rate
  SPISettings settingsA(baud, MSBFIRST, SPI_MODE0);
  LTC6803_initialize(); //! initialize LTC6803 hardware
  init_cfg(); //!initializa 6803 configuration
}

void LTC_initialize(void){
  run_command(1);
  run_command(2);
}

void LTC_cell_voltage_meas(void){
  run_command(3);
  run_command(4);
}

void run_command(uint32_t cmd){
  //! execute the following comands according to user input
    switch (cmd){
    
      case 1:

        LTC6803_wrcfg(TOTAL_IC,tx_cfg);
        break;

      case 2:

        LTC6803_rdcfg(TOTAL_IC,rx_cfg);
        break;

      case 3: //! start cell voltage ADC conversion

        LTC6803_stcvad();
        delay(3);
        break;

      case 4: //! read cell voltages

        LTC6803_rdcv(TOTAL_IC,cell_voltage); //! set to read back all cell voltage registers
        break;

      case 5: //! start temp voltage ADC conversion

        LTC6803_sttmpad();
        delay(3);
        break;

      case 6: //! real temp voltages

        LTC6803_rdtmp(TOTAL_IC,temp_voltage); // Set to read back all temp registers
        break;
      
  }
}

//! ************************************* Helper functions *************************************

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
}

//voltage conversion
float * vconv(uint16_t volt[50][12])
{
  for (i = 0; i < 12; i++) {
    for (j = 0; j < 50; j++) {
      vsum += volt[j][i];
    }
    vavg[i] = vsum/50;
    vsum = 0;
  }
  for (i=0; i<12; i++) {
    vreal[i] = ((float)vavg[i] - VZERO) * VSLOPE;
  }
  return vreal;
}

