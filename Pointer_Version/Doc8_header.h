#ifndef _DOC8_HEADER_H_
#define _DOC8_HEADER_H_

#include "WPILib.h"
//#include "KinectStick.h"
/*
 * config encoder enabled
 * encoder sarted in contructor
 * added defined speeds for shooter/launcher
 */

#define MAX 2700//max speed for jags in rpm
#define kENCCOUNT 360//pulses per rotation from encoder
#define KP 0.250//volts it is allowed to change the jag speed by per unit (was exp determined)
#define KI 0.015//integral PID is allowed to change KP by
#define KD 0.020//change PID uses to kill oscillation as nearing target voltage
#define kVOLTAGE 12//max voltage
#define RAMP .3//maximum KP + KI per unit <-lower is lower rate of change and vise versa (but KP+KI needs to be within this)
#define REDRUM GetWatchdog().Kill()
#define RPMLow 1000 //Speed settings for speed control on shooter/launcher
#define RPMMid 1200
#define RPMHigh 1400

#endif
