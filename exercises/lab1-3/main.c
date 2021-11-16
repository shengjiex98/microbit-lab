/* Template for lab experiment 1-3 (i.e., the introductory presentation) .
 * This is a simple demo program to demonstrate how to flash and debug the microbit. 
 *  It toggles one LED on the microbit screen.
 *
 * Oct. 2021, Philipp H. Kindt <philipp.kindt@informatik.tu-chemnitz.de>
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




/* First, we define multiple signals related to the LED screen*/

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


/* A function to just "burn" a certain number of CPU cycles to delay the execution.*/

void delay_cycles(uint64_t cycles){
	for(uint64_t cnt = 0; cnt < cycles; cnt++){
	  asm("nop");	//do nothing
	}
}

int main(void)
{

	// the code below initializes the LED matrix.

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

    //set the column signal of COL1 to 0 (i.e., low voltage). This will activate column 1.
    //If a corresponding row has a 1, the LED in COL1 will shine. We set all other columns to 1 for disactivating them.
    nrf_gpio_pin_write(IOT_MICROBIT_LED_COL1,0);
    nrf_gpio_pin_write(IOT_MICROBIT_LED_COL2,1);
    nrf_gpio_pin_write(IOT_MICROBIT_LED_COL3,2);
    nrf_gpio_pin_write(IOT_MICROBIT_LED_COL4,3);
    nrf_gpio_pin_write(IOT_MICROBIT_LED_COL5,4);

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



     //wait forever
     while(1){

		//toggle LEDs
		nrf_gpio_pin_toggle(IOT_MICROBIT_LED_ROW1);

		//wait some time
		delay_cycles(800000);
     }

     //will never reach this
     return 0;
}
