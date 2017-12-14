#include "conversion.h"

/*
Conversion Variables:
VSLOPE
VZERO
ISLOPE
IZERO
TSLOPE
TZERO
*/
int vcol; //volt size array
int vrow;
int icol; //current size array
int irow;
int tcol; //temp size array
int trow;
int vsum = 0;
int vavg[4] = {0, 0, 0, 0};
int isum = 0;
int iavg[1] = {0};
int tsum = 0;
int tavg[2] = {0, 0};
float vreal[4];
float ireal[2];
float treal[1];
int i;
int j;

/*
a = sizeof(array[0])/sizeof(array[0][0]);
b = sizeof(array)/sizeof(array[0]);
printf("%d %d\n",a,b);
*/

//voltage conversion
float vconv(float volts)
{
	vcol = sizeof(volts[0])/sizeof(volts[0][0]);
	vrow = sizeof(volts)/sizeof(volts[0]);
	for (i = 0; i<vcol; i++) {
		for (j = 0; j<vrow; j++) {
			vsum += volts[j][i];
		}
		vavg[i] = vsum/vrow;
		vsum = 0;
	}
	for (i=0; i<VREADNUM; i++) {
		vreal[i] = vavg[i]*VSLOPE + VZERO;
	}
	return vreal;
}

float iconv(float curr)
{
	icol = sizeof(curr[0])/sizeof(curr[0][0]);
	irow = sizeof(curr)/sizeof(curr[0]);
	for (i = 0; i<icol; i++) {
		for (j = 0; j<irow; j++) {
			isum += curr[j][i];
		}
		iavg[i] = isum/irow;
		isum = 0;
	}
	for (i=0; i<IREADNUM; i++) {
		ireal[i] = iavg[i]*ISLOPE + IZERO;
	}
	return ireal;
}

float tconv(float temp)
{
	tcol = sizeof(temp[0])/sizeof(temp[0][0]);
	trow = sizeof(temp)/sizeof(temp[0]);
	for (i = 0; i<tcol; i++) {
		for (j = 0; j<trow; j++) {
			tsum += temp[j][i];
		}
		tavg[i] = tsum/trow;
		tsum = 0;
	}
	for (i=0; i<TREADNUM; i++) {
		treal[i] = tavg[i]*TSLOPE + TZERO;
	}
	return treal;
}