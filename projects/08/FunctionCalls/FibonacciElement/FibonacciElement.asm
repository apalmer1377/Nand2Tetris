@261
D=A
@SP
M=D
@300
D=A
@LCL
M=D
@400
D=A
@ARG
M=D
(Sys.init)
@4
D=A
@SP
A=M
M=D
@SP
M=M+1
@0
D=A
@ARG
D=M+D
@TARG
M=D
@SP
M=M-1
A=M
D=M
@TARG
A=M
M=D
@FCONT1zz
D=A
@RET1zz
M=D
@Main.fibonacci
0;JMP
(FCONT1zz)
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
@RET1zz
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
@0
D=A
@ARG
D=M+D
@TARG
M=D
@SP
M=M-1
A=M
D=M
@TARG
A=M
M=D
@FCONT2zz
D=A
@RET2zz
M=D
@Main.fibonacci
0;JMP
(FCONT2zz)
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
@0
D=A
@ARG
D=M+D
@TARG
M=D
@SP
M=M-1
A=M
D=M
@TARG
A=M
M=D
@FCONT3zz
D=A
@RET3zz
M=D
@Main.fibonacci
0;JMP
(FCONT3zz)
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
@RET3zz
A=M
0;JMP
(END)
@END
0;JMP