/*
 * task_sensor.h
 *
 *  Created on: Nov 6, 2018
 *      Author: mose
 */

#ifndef TASK_SENSOR_H_
#define TASK_SENSOR_H_

#define TASK_SENSOR_STARTUPDELAY 2
#define TASK_SENSOR_INTERVALTIME 65535

void sensor_init();
void sensor_run();

#endif /* TASK_SENSOR_H_ */