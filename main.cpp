/*
 * main.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: ducky
 */

#include "mbed.h"
#include "rtos.h"
#include "mbed_rpc.h"

PwmOut led0(D9);
DigitalOut led1(D10);

InterruptIn sw2(D8);
DigitalOut led2(LED1);

DigitalOut led_cc(A0);

DigitalOut uled1(A6);
DigitalOut uled2(A5);
DigitalOut uled3(A4);
DigitalOut uled4(A3);

void led1_thread(void const *args) {
  while (true) {
    uled1 = !uled1;
    Thread::wait(1000);
  }
}

void led2_thread(void const *args) {
  while (true) {
    uled3 = !uled3;
    Thread::wait(333);
  }
}

int main() {
  led_cc = 0;

  Serial uart(SERIAL_TX, SERIAL_RX);
  uart.baud(115200);
  uart.printf("\r\n\r\nBuilt " __DATE__ " " __TIME__ " (" __FILE__ ")\r\n");

  RPC::add_rpc_class<RpcDigitalOut>();

  Thread thread1(led1_thread);
  Thread thread2(led2_thread);

  const size_t RPC_BUFSIZE = 256;
  char rpc_inbuf[RPC_BUFSIZE], rpc_outbuf[RPC_BUFSIZE];
  char* rpc_inptr = rpc_inbuf;  // next received byte pointer

  int step = 0;

  while (1) {
    while (uart.readable()) {
      char rx = uart.getc();
      if (rx == '\n' || rx == '\r') {
        *rpc_inptr = '\0';  // optionally append the string terminator
        bool rpc_rtn = RPC::call(rpc_inbuf, rpc_outbuf);
        uart.printf("%s >> (%i) %s\r\n", rpc_inbuf, rpc_rtn, rpc_outbuf);
        rpc_inptr = rpc_inbuf;  // reset the received byte pointer
      } else {
        *rpc_inptr = rx;
        rpc_inptr++;  // advance the received byte pointer
        if (rpc_inptr >= rpc_inbuf + RPC_BUFSIZE) {
          // you should emit some helpful error on overflow
          rpc_inptr = rpc_inbuf;  // reset the received byte pointer, discarding what we have
        }
      }
    }

    step += 1;
    if (step > 128) {
      step = 0;
      led1 = !led1;
    }
    led0 = (step * step) / 16384.f;

    wait(0.01);
  }
  return 0;
}

