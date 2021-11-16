/*
log.c - Simplistic uart driver. replaces original blessed uart driver
 Jul 2021, Philipp H. Kindt <philipp.kindt@informatik.tu-chemnitz.de>

 This file may be modified/distributed under the same conditions as blessed.
 */

#include "blessed/log.h"
#include <nrf51822.h>
#include <string.h>
#include <inttypes.h>
#include <nrf_uart.h>
#include <inttypes.h>
#include <nrf_gpio.h>
#include <boards.h>
#include <stdarg.h>
#include <stdio.h>

#define UART_2_USB 1
#if UART_2_USB
  #define TX_PIN_NUMBER NRF_GPIO_PIN_MAP(0,6)
  #define RX_PIN_NUMBER NRF_GPIO_PIN_MAP(1,8)

#else
  #define TX_PIN_NUMBER NRF_GPIO_PIN_MAP(0,2)                           //P1
  #define RX_PIN_NUMBER NRF_GPIO_PIN_MAP(0,3)                           //P0
  #define RTS_PIN_NUMBER NRF_GPIO_PIN_MAP(0,4)                         //P3
#define CTS_PIN_NUMBER NRF_GPIO_PIN_MAP(0,1)                         //P2

#endif

static bool tx_status = 0;         //for transmission. 0 => idle, 1=> transmitting
char uart_TX_buf[SIMPLE_UART_TX_BUF_LEN];
char uart_RX_buf[SIMPLE_UART_TX_BUF_LEN];

static ringBuf rbTx;
static ringBuf rbRx;
static log_cb_type log_rcv_cb;


static char intToStringBuf[12];


static char* uIntToString(uint32_t i){
  uint32_t decimals = 0;
  while((i > 0)||(decimals == 0)){
        intToStringBuf[decimals] = '0' + (i - (i/10)*10);
        i = i/10;
        decimals = decimals + 1;
  }
  char a,b;
  for(uint32_t j = 0; j < decimals/2; j++){
    a = intToStringBuf[j];
    b = intToStringBuf[decimals - j - 1];
    intToStringBuf[j] = b;
    intToStringBuf[decimals - j - 1] = a;
  }
  intToStringBuf[decimals] = '\0';
  return intToStringBuf;
}

static char* intToString(int32_t i){
  uint32_t decimals = 0;
  char a,b;
 if(i >= 0){
    return uIntToString(i);
 }else{
  while((-i > 0)||(decimals == 0)){
        intToStringBuf[decimals+1] = '0' - (i - (i/10)*10);
        i = i/10;
        decimals = decimals + 1;
  }
  for(uint32_t j = 0; j < decimals/2; j++){
    a = intToStringBuf[j+1];
    b = intToStringBuf[decimals - j];
    intToStringBuf[j+1] = b;
    intToStringBuf[decimals - j] = a;
  }
  intToStringBuf[decimals+1] = '\0';
  intToStringBuf[0] = '-';
  return intToStringBuf;


 }
}



int16_t log_init(void){
    nrf_gpio_cfg_output(TX_PIN_NUMBER);
   // nrf_gpio_cfg(TX_PIN_NUMBER,NRF_GPIO_PIN_DIR_OUTPUT,NRF_GPIO_PIN_INPUT_DISCONNECT,NRF_GPIO_PIN_PULLDOWN,NRF_GPIO_PIN_H0H1,NRF_GPIO_PIN_NOSENSE);
    nrf_gpio_cfg_input(RX_PIN_NUMBER, NRF_GPIO_PIN_NOPULL);
#if UART_2_USB
    nrf_uart_configure(NRF_UART0,NRF_UART_PARITY_EXCLUDED,NRF_UART_HWFC_DISABLED);
#else
    nrf_gpio_cfg_output(RTS_PIN_NUMBER);
    nrf_gpio_cfg_input(CTS_PIN_NUMBER, NRF_GPIO_PIN_NOPULL);
    nrf_uart_configure(NRF_UART0,NRF_UART_PARITY_EXCLUDED, NRF_UART_HWFC_ENABLED);
    nrf_uart_hwfc_pins_set(NRF_UART0,RTS_PIN_NUMBER, CTS_PIN_NUMBER);
#endif
    nrf_uart_baudrate_set(NRF_UART0,NRF_UART_BAUDRATE_115200);
    nrf_uart_txrx_pins_set(NRF_UART0,TX_PIN_NUMBER, RX_PIN_NUMBER);
    nrf_uart_enable(NRF_UART0);
    NVIC_EnableIRQ(UART0_IRQn);
    NVIC_SetPriority(UART0_IRQn,IRQ_PRIORITY_LOW);
    NVIC_ClearPendingIRQ(UART0_IRQn);
    nrf_uart_int_enable(NRF_UART0, NRF_UART_INT_MASK_RXDRDY);
    nrf_uart_int_enable(NRF_UART0, NRF_UART_INT_MASK_TXDRDY);

    tx_status = 0;
    ringBuf_init(&rbTx, uart_TX_buf,SIMPLE_UART_TX_BUF_LEN);
    ringBuf_init(&rbRx, uart_RX_buf,SIMPLE_UART_RX_BUF_LEN);
    nrf_uart_task_trigger(NRF_UART0, NRF_UART_TASK_STARTRX);

    log_rcv_cb = NULL;
    return 0;
}
static inline void initTransmission(){
  char c;
  if(tx_status == 0){
     ringBuf_read(&rbTx,&c,1);
     nrf_uart_txd_set(NRF_UART0,c);
     nrf_uart_task_trigger(NRF_UART0, NRF_UART_TASK_STARTTX);
  }
  tx_status = 1;
}

int16_t log_int(int32_t n){
  log_string(intToString(n));
  return 0;

}
int16_t log_uint(uint32_t n){
  log_string(uIntToString(n));
  return 0;

}

int16_t log_char(char c){
  ringBuf_write(&rbTx,&c,1);
  initTransmission();
  return 0;
}


int16_t log_write(const char* buf, uint32_t nBytes){
  for(uint32_t i = 0; i < nBytes; i++){
    log_char(buf[i]);
  }
  return 0;
}

int16_t log_string(const char *str){
  log_write(str, strlen(str));
  return 0;

}

int16_t log_newline(void){
  log_char('\n');
  return 0;

}
int16_t log_printf(const char *format, ...){
            char buf[SIMPLE_UART_TX_BUF_LEN];
             va_list a;
             va_start(a,format);
             vsnprintf(buf,SIMPLE_UART_TX_BUF_LEN,format,a);
             va_end(a);
             log_string(buf);
             return 0;
}

void UART0_IRQHandler(void){
  static char c;
  if(NRF_UART0->EVENTS_TXDRDY != 0){
    NRF_UART0->EVENTS_TXDRDY = 0;

    if(rbTx.nBytes > 0){
      ringBuf_read(&rbTx,&c,1);
      nrf_uart_txd_set(NRF_UART0,c);
      nrf_uart_task_trigger(NRF_UART0, NRF_UART_TASK_STARTTX);
    }else{
      tx_status = 0;
    }
  }

  if(NRF_UART0->EVENTS_RXDRDY != 0){
    while(NRF_UART0->EVENTS_RXDRDY){
      NRF_UART0->EVENTS_RXDRDY = 0;
      c = nrf_uart_rxd_get(NRF_UART0);
      ringBuf_write(&rbRx,&c,1);
    }
    if(log_rcv_cb != NULL){
      log_rcv_cb(&rbRx);
    }

  }

}


/* Code for reception */
void log_set_rcv_cb(log_cb_type cb){
  log_rcv_cb = cb;
}
