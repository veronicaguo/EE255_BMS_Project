#ifndef CC_CH_ERROR
#define CC_CH_ERROR

#ifndef IDES_CH
#define IDES_CH 2

#ifndef CC_CH_INTEG
#define CC_CH_INTEG

#ifndef DT
#define DT 1e-6

#ifndef DUTY_NEW
#define DUTY_NEW

#ifndef KI_CC_CH
#define KI_CC_CH 1

#ifndef KP_CC_CH
#define KP_CC_CH 1

#ifndef DMAX
#define DMAX 0.6

#ifndef DMIN
#define DMIN 0.4

float CC_Charge(float DUTY, float CURR);