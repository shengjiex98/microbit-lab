/* Simple GPIO driver in assembly.
Sept. 2021, Philipp H. Kindt <philipp.kindt@informatik.tu-chemnitz.de>
*/
.global gpio_on
.global gpio_off
.global gpio_write_assembly;
.data
	column: .word 0x0;
	columnPrev: .word 0x0;

.text
gpio_on:
	//write your own code here
bx R14

gpio_off:
	//write your own code here
bx R14


gpio_write_assembly:
	//write your own code here
bx R14;
