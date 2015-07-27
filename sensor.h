/*
 * Generic sensor interface
 *
 */

#ifndef __SENSOR_H
#define __SSENSOR_H

//########################################################################
//Function that senses 
//########################################################################


void sensor_setup();
int sensor_sense(char* valueArray);

#endif
