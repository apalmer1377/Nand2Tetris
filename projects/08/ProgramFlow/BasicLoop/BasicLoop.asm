@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@0
D=A
@i
M=D
@LCL
D=M
@i
M=M+D
@SP
M=M-1
A=M
D=M
@i
A=M
M=D
(.LOOP_START)
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
@0
D=A
@LCL
A=M
A=A+D
D=M
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
D=M+D
@SP
A=M
M=D
@SP
M=M+1
@0
D=A
@i
M=D
@LCL
D=M
@i
M=M+D
@SP
M=M-1
A=M
D=M
@i
A=M
M=D
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
@i
M=D
@ARG
D=M
@i
M=M+D
@SP
M=M-1
A=M
D=M
@i
A=M
M=D
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
@SP
M=M-1
A=M
D=M
@.LOOP_START
D;JNE
@0
D=A
@LCL
A=M
A=A+D
D=M
@SP
A=M
M=D
@SP
M=M+1
(END)
@END
0;JMP