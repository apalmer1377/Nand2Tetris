// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.
@pos
M=0

(LOOP)
@KBD
D=M
@FILL
D;JGT
@CLEAR
D;JEQ


(FILL)
@pos
D=M
@SCREEN
A=A+D
M=-1
@pos
D=M
@SCREEN
D=D+A
@24576
D=D-A
D=D+1
@LOOP
D;JEQ
@pos
M=M+1
@LOOP
0;JMP

(CLEAR)
@pos
D=M
@SCREEN
A=A+D
M=0
@pos
D=M
@LOOP
D;JEQ
@pos
M=M-1
@LOOP
0;JMP


