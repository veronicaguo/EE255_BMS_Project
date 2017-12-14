/*!
  EE255 BMS main loop.
*/

//! include all libraries
#include <Arduino.h>
#include <stdint.h>
#include "Linduino.h"
#include "LT_SPI.h"
#include "UserInterface.h"
#include "LTC68031.h"
#include <SPI.h>
#include "initialization.h"
#include "cv_charge.h"
#include "cc_charge.h"
#include "conversion.h"
#include "PWM_GEN.h"

float duty = 0;
float cell_voltage[4];
float temp_value[2];
float* output = (float *) malloc(7*sizeof(float));
float cell_current;
int mode;
int i;
uint16_t cell_voltage_meas[50][4];
uint16_t temp_voltage_meas[50][2];
uint16_t cell_current_meas[50];

int main(){
	
	// PWM setup and initialization
	pwmsetup();

	// LTC setup and initialization
	LTC_setup();	
	LTC_initialize();	

	// main loop
	while(1){
		// PWM assignment
		pwmgen(duty);

		// measure cell voltages and temp voltages
		for (i = 0; i < 50; i++){
			cell_voltage_meas[i] = *LTC_cell_voltage_meas();
			temp_voltage_meas[i] = *LTC_temp_voltage_meas();
			// need to create ``LTC_cell_current_meas()`` in initialization.c
			cell_current_meas[i] = *LTC_cell_current_meas();
		}
		cell_voltage = vconv(cell_voltage_meas);
		temp_value = tconv(temp_voltage_meas);
		cell_current = iconv(cell_current_meas);

		memcpy(output, cell_voltage, 4*sizeof(float));
		memcpy(output+4, temp_value, 2*sizeof(float));
		memcpy(output+6, temp_value, 1*sizeof(float));

		Serial.println(output);

		duty = check_state(cell_voltage, cell_current, mode);	
	}

	return 0;
}