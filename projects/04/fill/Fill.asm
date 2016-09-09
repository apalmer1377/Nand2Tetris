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
@i
M=0
@j
M=0
@k
M=0
(LOOP)
@i
D=M
@j
M=D
@KBD
D=M
@i
M=D
@j
D=D-M
@k
M=D
@i

(FILL)
@pos
D=M
@SCREEN


(CLEAR)
@SCREEN
