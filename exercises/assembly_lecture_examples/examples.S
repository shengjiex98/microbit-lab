/* Assembly Examples.
Oct. 2021, Philipp H. Kindt <philipp.kindt@informatik.tu-chemnitz.de>
*/

.extern b
.global examples

.data
a: .word 0x123

.text



// Add 2 Registers Example
add_2r:
mov R1, #0x10			//0x10->R1
mov R2, #0x02			//0x02->R2
add R1, R2				//add
bx r14;



//Read from and write to memory
memory:
mov R2, #0x02			//0x02->R2
ldr R4, =a				//memory address corresponding to symbol a -> R4
ldr R1, [R4]			//Load contents from memory address stored in R4 into R1. R1 will now have the value 0x123
str R2, [R4]			//replace contents of memory address in R4 by what is in R2. Now it is mem(a) = 0x02
ldr R3, [R4]			//load the new contents back to R3. R3 is now 0x02

ldr R3, =#2				//set R3 to #2
sub R4, #4				//R4 = R4 - 4
ldr R1, [R4, #4]		//load from memory address R4 + 4. R1 is now 0x02

bx r14;




//Jump somewhere and jump back (no function call)
jump_jumpback:
nop;
nop;
nop;
B return;		//jump to the "return" label




/*getting large values into registers */
large_value:
MOV R1, #255				//255->R1. 255 is the largest possible of this way
LDR R1, =#0xCAFFEE12		//pseudo-instruction. Set R1 0 0xCAFFEE12
LDR R2, =a					//R2->memory address a
MOVW  R3,#:lower16:a		//R3->lower 16 bit of memory address a
MOVT R3,#:upper16:a			//R3->higher 16 bit of memory address a

bx r14;


/* If-then-else construct */
if_then_else:
MOV R2, #2					//2->R2
MOV R1, #2					//2->R1
CMP R1, R2					//compare R1 and R2
BEQ then_clause				//if R1==R2, jump to "then_clause". Otherwise, just continue
MOV R3, #10					//else 10->R3
b endif;					//jump to endif
then_clause:				//
MOV R3, #4					//4->R3

endif:
bx r14





/* A for-loop*/
for_loop:
.text
mov R1, #10;				//10->R1
loop:
sub R1,R1,#1				//R1 = R1 - 1
cmp R1,#0					//compare R1 and 0
bne loop					//if R1 != 0, jump to "loop"
bx r14




/*modify a Variable b, which has been declared in main.c*/
modify_c_variable:
	ldr R2, =b			//address of variable b (=value of the symbol b) _>R2
	ldr R1, [R2]		//load from address of b. Value of b is now in R1
	add R1, R1, #10		//R1 = R1 + 10
	str R1, [R2]		//write back to the memory address of variable b
	bx r14




/* Call a function without using push/pop */
function_call_manual:
	//increase stack pointer
	MOV R1, R13				//R13 -> R1. We cannot directly use add on R13 (i.e., the SP), because it's a high register
	ADD R1, R1, #4			//R1 = R1 + 4 -> make 4 bytes space on the stack
	MOV R13, R1				//R1->R13

	//put previous return address on the stack
	MOV R1, R14				//R14 -> R1. R14 is the LR
	MOV R2, R13				//R13- > R2. R13 is the SP
	STR R1, [R2]			//R1->mem(R2) => put return address to SP

	//branch and link
	bl myFunction			//jump to myFunction and store PC+4 in R14

	//restore previous return address
	MOV R2, R13				//R13->R2. R13 is the SP.
	LDR R1, [R2]			//r1 = mem(SP)
	MOV R14, R1				//R1->R14

	//decrease stack pointer
	MOV R1, R13				//R13->R1
	SUB R1, R1, #4			//R1 = R1 - 4
	MOV R13, R1				//R1->R13 => stack pointer decreased

	//exit this function
	bx R14



/* Call a function using push/pop */

function_call_pushpop:

	//store old return address on stack
	push {LR}

	//branch and link
	bl myFunction

	//restore previous return address
	POP {R1}
	MOV LR, R1


	//exit this function
	bx R14


myFunction:
bx R14

examples:
	push {LR}

	// Read/Write Memory Example
	bl memory;

	//Large Value Example
	bl large_value

	// Jump + Jump Back Example
	B jump_jumpback;
	return:

	// For Loop Example
	bl for_loop

	// If..Then...Else Example
	bl if_then_else


	// Modify a variable from C
	bl modify_c_variable;



	//Function Call Example (Exhaustive/Manual)
	bl function_call_manual

	//Function Call Example (Push/Pop)
	bl function_call_pushpop

	//code needed to return to main()
	pop {R1}
	mov LR, R1





bx r14;
