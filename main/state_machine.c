/*!
  EE255 BMS state machine.
*/

float duty;


// charging: mode = 1, discharging: mode = 0
void check_state(float cell_voltage,  float cell_current， uint8_t mode)
{
	if (mode == 1){
		if (cell_voltage <= 4.1){
			duty = cc_charge();
		}
		else{
			duty = cv_charge();
		}
	}
	else if (mode == 0){
		if (cell_voltage >= 3.0){
			duty = constant_curr_dischar();
		}
		else{
			duty = constant_vol_dischar();
		}
	}
	return duty;
}