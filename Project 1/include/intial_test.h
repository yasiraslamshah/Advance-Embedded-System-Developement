/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name : initial_test.h
* Creation Date : 31-03-2019
* Last Modified : Sun 31 March 2019 00:07:10 PM MDT
* Created By : Harsimransingh and Yasir Shah
* Description: header file to run inital tests for sensors
* Functions:
*           intial_test() to test all sensors before creating threads
* References:
_._._._._._._._._._._._._._._._._._._._._.*/


#ifndef _INTIAL_TEST_H
#define _INTIAL_TEST_H

/* -------------------------------*/
/**
 * @Synopsis this function calls the temperature test and light test functions to check if there are functional before moving forward with creating the threads 
 *
 * @Param filename parameter provides the log file name
 *
 * @Returns exit status based on success or failure
 */
/* ---------------------------------*/
int intial_test(char *filename);

#endif
