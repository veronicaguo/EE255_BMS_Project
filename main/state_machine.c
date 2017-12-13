/*!
  EE255 BMS state machine.
*/
float vtop = 16.4;
float vbot = 12;
float duty;


// charging: mode = 1, discharging: mode = 0
float check_state(float cell_voltage,  float cell_current， uint8_t mode)
{
	if (mode == 1){
		if (cell_voltage <= vtop){
			duty = cc_charge();
		}
		else{
			duty = cv_charge();
		}
	}
	/*
	else if (mode == 0){
		if (cell_voltage >= vbot){
			duty = constant_curr_dischar();
		}
		else{
			duty = constant_vol_dischar();
		}
	}
	*/
	return duty;

}