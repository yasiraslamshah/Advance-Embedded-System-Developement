/*
 * Author : Yasir Aslam Shah
 * FreeRTOS 8.2 Tiva Demo
 */

#ifndef INIT_H_
#define INIT_H_

void initialize_timer();
void initialize_gpio();
void initialize_i2c();
void initialize_uart();
void timer_handler();
void alert_thread(void *pvParameters);
#endif /* INIT_H_ */
