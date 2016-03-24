/*
 * main.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: ducky
 */

#include "mbed.h"

PwmOut led0(D9);
DigitalOut led1(D10);

InterruptIn sw2(D8);
DigitalOut led2(LED1);

void flip() {
  led2 = !led2;
}

int main() {
  int step = 0;

  led2 = 1;
  led2 = 0;

  Serial uart(SERIAL_TX, SERIAL_RX);

  uart.baud(115200);

  sw2.rise(&flip);

  while (1) {
    step += 1;
    if (step > 128) {
      step = 0;
      led1 = !led1;
    }
    led0 = (step * step) / 16384.f;

    wait(0.01);

    uart.putc('x');
  }
  return 0;
}
