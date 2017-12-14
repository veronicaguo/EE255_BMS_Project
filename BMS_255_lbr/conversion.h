#ifndef VSLOPE
#define VSLOPE 100
#endif

#ifndef VZERO
#define VZERO 100
#endif

#ifndef ISLOPE
#define ISLOPE 100
#endif

#ifndef IZERO
#define IZERO 100
#endif

#ifndef TSLOPE
#define TSLOPE 100
#endif

#ifndef TZERO
#define TZERO 100
#endif

#ifndef VREADNUM
#define VREADNUM 4
#endif

#ifndef IREADNUM
#define IREADNUM 1
#endif

#ifndef TREADNUM
#define TREADNUM 2
#endif

float vconv(uint16_t **volt);
float iconv(uint16_t **curr);
float tconv(uint16_t **temp);
