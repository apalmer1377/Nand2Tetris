(Sys.init)
@4
D=A
@SP
A=M
M=D
@SP
M=M+1
@RARG1zz
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@5
D=A
@1
D=A+D
@SP
D=M-D
@ARG
M=D
@SP
D=M
@LCL
M=D
@Main.fibonacci
0;JMP
(RARG1zz)
(Sys.init.WHILE)
@Sys.init.WHILE
0;JMP
(Main.fibonacci)
@0
D=A
@ARG
A=M
A=A+D
D=M
@SP
A=M
M=D
@SP
M=M+1
@2
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
D=D
@START9zz
0;JMP
(TRUE9zz)
D=-1
@CONT9zz
0;JMP
(FALSE9zz)
D=0
@CONT9zz
0;JMP
(START9zz)
@SP
M=M-1
A=M
D=M-D
@TRUE9zz
D;JLT
@FALSE9zz
0;JMP
(CONT9zz)
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@Main.fibonacci.IF_TRUE
D;JNE
@Main.fibonacci.IF_FALSE
0;JMP
(Main.fibonacci.IF_TRUE)
@0
D=A
@ARG
A=M
A=A+D
D=M
@SP
A=M
M=D
@SP
M=M+1
@5
D=A
@LCL
A=M-D
D=M
@reta
M=D
@5
D=A
@LCL
D=M-D
@rett
M=D
@SP
A=M-1
D=M
@ARG
A=M
M=D
@ARG
D=M+1
@SP
M=D
@rett
M=M+1
A=M
D=M
@LCL
M=D
@rett
M=M+1
A=M
D=M
@ARG
M=D
@rett
M=M+1
A=M
D=M
@THIS
M=D
@rett
M=M+1
A=M
D=M
@THAT
M=D
@reta
A=M
0;JMP
(Main.fibonacci.IF_FALSE)
@0
D=A
@ARG
A=M
A=A+D
D=M
@SP
A=M
M=D
@SP
M=M+1
@2
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=M-D
@SP
A=M
M=D
@SP
M=M+1
@RARG2zz
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@5
D=A
@1
D=A+D
@SP
D=M-D
@ARG
M=D
@SP
D=M
@LCL
M=D
@Main.fibonacci
0;JMP
(RARG2zz)
@0
D=A
@ARG
A=M
A=A+D
D=M
@SP
A=M
M=D
@SP
M=M+1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=M-D
@SP
A=M
M=D
@SP
M=M+1
@RARG3zz
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@5
D=A
@1
D=A+D
@SP
D=M-D
@ARG
M=D
@SP
D=M
@LCL
M=D
@Main.fibonacci
0;JMP
(RARG3zz)
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=M+D
@SP
A=M
M=D
@SP
M=M+1
@5
D=A
@LCL
A=M-D
D=M
@reta
M=D
@5
D=A
@LCL
D=M-D
@rett
M=D
@SP
A=M-1
D=M
@ARG
A=M
M=D
@ARG
D=M+1
@SP
M=D
@rett
M=M+1
A=M
D=M
@LCL
M=D
@rett
M=M+1
A=M
D=M
@ARG
M=D
@rett
M=M+1
A=M
D=M
@THIS
M=D
@rett
M=M+1
A=M
D=M
@THAT
M=D
@reta
A=M
0;JMP
(END)
@END
0;JMP