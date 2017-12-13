#ifndef PWMPIN
#define PWMPIN 3
#endif

#ifndef PWMFREQ
#define PWMFREQ 100000
#endif

#ifndef PWM_NUM
#define PWM_NUM 256
#endif

void pwmsetup();

void pwmgen(float duty);