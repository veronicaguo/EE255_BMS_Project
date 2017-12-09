/*!
  EE255 BMS state machine.
*/

// charging: mode = 1, discharging: mode = 0
void check_state(float cell_voltage,  float cell_current， uint8_t mode)
{
	if (mode == 1){
		if (cell_voltage <= 4.1){
			constant_curr_char();
		}
		else{
			constant_vol_char();
		}
	}
	else if (mode == 0){
		if (cell_voltage >= 3.0){
			constant_curr_dischar();
		}
		else{
			constant_vol_dischar();
		}
	}
}