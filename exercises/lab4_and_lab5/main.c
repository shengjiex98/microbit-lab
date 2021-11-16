/* Template for lab experiment 4 - microbit LEDs
 * Sept. 2021, Philipp H. Kindt <philipp.kindt@informatik.tu-chemnitz.de>
 *
 */

// From C standard library
#include <stdint.h>
#include <string.h>

// From Nordic SDK
#include "nrf_gpio.h"
#include "nordic_common.h"

// From blessed (microbit-lab)
#include "delay.h"
#include "timer.h"


/* External symbols from myGPIODriver.S (assembly) for Exercise 5.
 * Please ignore this for Exercise 4.
 */

void gpio_on(uint32_t pin);
void gpio_off(uint32_t pin);
void gpio_write_assembly(uint32_t pin, uint32_t value);

/* The columns and rows of the LED matrix are connected to GPIO pins.
 * There are two GPIOs, and each has multiple pins it can drive. Hence, every
 * signal of the LED matrix is defined by a tuple of one GPIO and a corresponding pin.
 * Below. We first define the GPIOs and pins of each LED signal.
 * Next, we combine them to a tuple
 */

/* GPIOs of columns of the LED matrix*/
#define IOT_MICROBIT_LED_COL1_GPIO   0
#define IOT_MICROBIT_LED_COL2_GPIO   0
#define IOT_MICROBIT_LED_COL3_GPIO   0
#define IOT_MICROBIT_LED_COL4_GPIO   1          //this is an exception - it's the only LED matrix signal connected to GPIO 2
#define IOT_MICROBIT_LED_COL5_GPIO   0

/* Pins of columns of the LED matrix*/
#define IOT_MICROBIT_LED_COL1_PIN   28
#define IOT_MICROBIT_LED_COL2_PIN   11
#define IOT_MICROBIT_LED_COL3_PIN   31
#define IOT_MICROBIT_LED_COL4_PIN   5
#define IOT_MICROBIT_LED_COL5_PIN   30

/* GPIOs of rows of the LED matrix*/
#define IOT_MICROBIT_LED_ROW1_PIN   21
#define IOT_MICROBIT_LED_ROW2_PIN   22
#define IOT_MICROBIT_LED_ROW3_PIN   15
#define IOT_MICROBIT_LED_ROW4_PIN   24
#define IOT_MICROBIT_LED_ROW5_PIN   19

/* Pins of rowas of the LED matrix*/
#define IOT_MICROBIT_LED_ROW1_GPIO   0
#define IOT_MICROBIT_LED_ROW2_GPIO   0
#define IOT_MICROBIT_LED_ROW3_GPIO   0
#define IOT_MICROBIT_LED_ROW4_GPIO   0
#define IOT_MICROBIT_LED_ROW5_GPIO   0

/* Combine column and row in one macro.
The result is a number 0..31 for the case of GPIO0, and
a number of 32 + 0..31 for GPIO1. The resulting number is understood
by the nrf_gpio_pin_write()-function.
*/
#define IOT_MICROBIT_LED_COL1 NRF_GPIO_PIN_MAP(IOT_MICROBIT_LED_COL1_GPIO,IOT_MICROBIT_LED_COL1_PIN)
#define IOT_MICROBIT_LED_COL2 NRF_GPIO_PIN_MAP(IOT_MICROBIT_LED_COL2_GPIO,IOT_MICROBIT_LED_COL2_PIN)
#define IOT_MICROBIT_LED_COL3 NRF_GPIO_PIN_MAP(IOT_MICROBIT_LED_COL3_GPIO,IOT_MICROBIT_LED_COL3_PIN)
#define IOT_MICROBIT_LED_COL4 NRF_GPIO_PIN_MAP(IOT_MICROBIT_LED_COL4_GPIO,IOT_MICROBIT_LED_COL4_PIN)
#define IOT_MICROBIT_LED_COL5 NRF_GPIO_PIN_MAP(IOT_MICROBIT_LED_COL5_GPIO,IOT_MICROBIT_LED_COL5_PIN)

#define IOT_MICROBIT_LED_ROW1 NRF_GPIO_PIN_MAP(IOT_MICROBIT_LED_ROW1_GPIO,IOT_MICROBIT_LED_ROW1_PIN)
#define IOT_MICROBIT_LED_ROW2 NRF_GPIO_PIN_MAP(IOT_MICROBIT_LED_ROW2_GPIO,IOT_MICROBIT_LED_ROW2_PIN)
#define IOT_MICROBIT_LED_ROW3 NRF_GPIO_PIN_MAP(IOT_MICROBIT_LED_ROW3_GPIO,IOT_MICROBIT_LED_ROW3_PIN)
#define IOT_MICROBIT_LED_ROW4 NRF_GPIO_PIN_MAP(IOT_MICROBIT_LED_ROW4_GPIO,IOT_MICROBIT_LED_ROW4_PIN)
#define IOT_MICROBIT_LED_ROW5 NRF_GPIO_PIN_MAP(IOT_MICROBIT_LED_ROW5_GPIO,IOT_MICROBIT_LED_ROW5_PIN)

/*Create an array of cols and an array of rows.
  We can identify the N'th col/row GPIO/pin tuple by  cols[N] or rows[n]
  */

uint32_t cols[] = {IOT_MICROBIT_LED_COL1, IOT_MICROBIT_LED_COL2, IOT_MICROBIT_LED_COL3, IOT_MICROBIT_LED_COL4, IOT_MICROBIT_LED_COL5};
uint32_t rows[] = {IOT_MICROBIT_LED_ROW1, IOT_MICROBIT_LED_ROW2, IOT_MICROBIT_LED_ROW3, IOT_MICROBIT_LED_ROW4, IOT_MICROBIT_LED_ROW5};


//heart-shaped pattern
uint8_t pixels[5][5] = {{0,1,0,1,0},{1,0,1,0,1},{1,0,0,0,1},{0,1,0,1,0},{0,0,1,0,0}};



/*This function will be called periodically be the timer*/
/* Please place your revised code here.*/
void update_display(){
  uint32_t column = 0;
      nrf_gpio_pin_write(cols[column],0);                                     //activate current column
      for(uint8_t row = 0; row < 5; row++){
         nrf_gpio_pin_write(rows[row],pixels[column][row]);
      }
}


int main(void)
{
    /* Every GPIO pin can either read from outside (input) or
     * drive an output signal. In our case, we want to drive the LED signals.
     * Hence, we have to configure all GPIO pins of the LED matrix as output
     */
    nrf_gpio_cfg_output(IOT_MICROBIT_LED_COL1);
    nrf_gpio_cfg_output(IOT_MICROBIT_LED_COL2);
    nrf_gpio_cfg_output(IOT_MICROBIT_LED_COL3);
    nrf_gpio_cfg_output(IOT_MICROBIT_LED_COL4);
    nrf_gpio_cfg_output(IOT_MICROBIT_LED_COL5);
    nrf_gpio_cfg_output(IOT_MICROBIT_LED_ROW1);
    nrf_gpio_cfg_output(IOT_MICROBIT_LED_ROW2);
    nrf_gpio_cfg_output(IOT_MICROBIT_LED_ROW3);
    nrf_gpio_cfg_output(IOT_MICROBIT_LED_ROW4);
    nrf_gpio_cfg_output(IOT_MICROBIT_LED_ROW5);

    //set all column signals to 1 (i.e., high voltage
    nrf_gpio_pin_write(IOT_MICROBIT_LED_COL1,1);
    nrf_gpio_pin_write(IOT_MICROBIT_LED_COL2,1);
    nrf_gpio_pin_write(IOT_MICROBIT_LED_COL3,1);
    nrf_gpio_pin_write(IOT_MICROBIT_LED_COL4,1);
    nrf_gpio_pin_write(IOT_MICROBIT_LED_COL5,1);

    //GIPO pins can be driven with different power levels. We here chose the highest drive power for brighter LEDs
    NRF_P0->PIN_CNF[IOT_MICROBIT_LED_ROW1_PIN] |= NRF_GPIO_PIN_H0H1<<GPIO_PIN_CNF_DRIVE_Pos;
    NRF_P0->PIN_CNF[IOT_MICROBIT_LED_ROW2_PIN] |= NRF_GPIO_PIN_H0H1<<GPIO_PIN_CNF_DRIVE_Pos;
    NRF_P0->PIN_CNF[IOT_MICROBIT_LED_ROW3_PIN] |= NRF_GPIO_PIN_H0H1<<GPIO_PIN_CNF_DRIVE_Pos;
    NRF_P0->PIN_CNF[IOT_MICROBIT_LED_ROW4_PIN] |= NRF_GPIO_PIN_H0H1<<GPIO_PIN_CNF_DRIVE_Pos;
    NRF_P0->PIN_CNF[IOT_MICROBIT_LED_ROW5_PIN] |= NRF_GPIO_PIN_H0H1<<GPIO_PIN_CNF_DRIVE_Pos;
    NRF_P0->PIN_CNF[IOT_MICROBIT_LED_COL1_PIN] |= NRF_GPIO_PIN_H0H1<<GPIO_PIN_CNF_DRIVE_Pos;
    NRF_P0->PIN_CNF[IOT_MICROBIT_LED_COL2_PIN] |= NRF_GPIO_PIN_H0H1<<GPIO_PIN_CNF_DRIVE_Pos;
    NRF_P0->PIN_CNF[IOT_MICROBIT_LED_COL3_PIN] |= NRF_GPIO_PIN_H0H1<<GPIO_PIN_CNF_DRIVE_Pos;
    NRF_P1->PIN_CNF[IOT_MICROBIT_LED_COL4_PIN] |= NRF_GPIO_PIN_H0H1<<GPIO_PIN_CNF_DRIVE_Pos;
    NRF_P0->PIN_CNF[IOT_MICROBIT_LED_COL5_PIN] |= NRF_GPIO_PIN_H0H1<<GPIO_PIN_CNF_DRIVE_Pos;



    /* The timer is a piece of hardware that generates an interrupt
     * with a fixed period. The interrupt will be handled by the timer driver of blessed.
     * We now first initialize the timer driver, then create a timer handle and then start the timer with a period of 1000 milliseconds.
     * After every timeout, the update_screen function will be called
     */
     timer_init();

     /*Declare a handle for a timer*/
     int16_t timer1;

     //initialize and start the timer
     timer1 = timer_create(TIMER_REPEATED);
     timer_start(timer1, TIMER_MILLIS(1000), update_display);

     //Just to write some text on the console that the program is running
     log_init();
     log_printf("Hello World\n");

     //wait forever
     while(1){
       ;
     }

     //will never reach this
     return 0;
}
