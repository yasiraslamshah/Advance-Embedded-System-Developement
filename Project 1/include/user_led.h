/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : led_control.h
* Creation Date : 29-03-2019
* Last Modified : Sun 31 March 2019 00:10:12 PM MDT
* Created By : Harsimransingh and Yasir Shah
* Description: header file to control on board leds
* Functions:
*           led_control() to turn on board leds on or off based on the inputs
* References:
_._._._._._._._._._._._._._._._._._._._._.*/

#ifndef __USER_LED_H
#define __USER_LED_H

/*standard libraries*/
#include <stdio.h>
#include <stdlib.h>

/*Macros to select leds*/
#define BLUE  1
#define RED   2
#define GREEN 3

/*Macros for led status*/
#define ON  1
#define OFF 0

/* -------------------------------*/
/**
 * @Synopsis function to control the on board leds. Pin 53,54 and 55 are used. 
 *
 * @Param led_color to select the led number
 * @Param status to take input from user to turn led on or off
 */
/* ---------------------------------*/
void led_control(int led_color, int status);

#endif
