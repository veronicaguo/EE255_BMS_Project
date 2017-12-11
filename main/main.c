/*!
  EE255 BMS main loop.
*/

float duty = 0;
float cell_voltage;
float temp_voltage;
float cell_current;
int mode;
int i;
uint16_t cell_voltage_meas[50][4];
uint16_t temp_voltage_meas[50][2];
uint16_t cell_current_meas[50];

void main(){
	
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
			cell_voltage_meas[i] = LTC_cell_voltage_meas();
			temp_voltage_meas[i] = LTC_temp_voltage_meas();
			// need to create ``LTC_cell_current_meas()`` in initialization.c
			cell_current_meas[i] = LTC_cell_current_meas();
		}
		cell_voltage = vconv(cell_voltage_meas);
		temp_voltage = tconv(temp_voltage_meas);
		cell_current = iconv(cell_current_meas);

		duty = check_state(cell_voltage, cell_current, mode)	
	}
}