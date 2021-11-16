/* Assembly Examples.
Oct. 2021, Philipp H. Kindt <philipp.kindt@informatik.tu-chemnitz.de>
*/


#include <stdint.h>
#include <string.h>


#include <nrf_gpio.h>
#include "nordic_common.h"
#include <inttypes.h>

uint32_t b = 0xcaffe123;

int myFunction(int  a){
 return a + 1;
}
void examples();
int main(void)
{
  int (*ptr)(int);
  ptr = myFunction;
    while(1){
        examples();
    }
  return 0;
}
