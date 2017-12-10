#ifndef VSLOPE
#define VSLOPE 100

#ifndef VZERO
#define VZERO 100

#ifndef ISLOPE
#define ISLOPE 100

#ifndef IZERO
#define IZERO 100

#ifndef TSLOPE
#define TSLOPE 100

#ifndef TZERO
#define TZERO 100

#ifndef VREADNUM
#define VREADNUM 5

#ifndef IREADNUM
#define IREADNUM 1

#ifndef TREADNUM
#define TREADNUM 2
#endif

float vconv(uint16_t volt);
float iconv(uint16_t curr);
float tconv(uint16_t temp);