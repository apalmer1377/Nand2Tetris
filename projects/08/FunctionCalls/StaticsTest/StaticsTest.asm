@256
D=A
@SP
M=D
@INIT
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
@SP
D=M-D
@ARG
M=D
@SP
D=M
@LCL
M=D
@Sys.init
0;JMP
(INIT)
(Sys.init)
@6
D=A
@SP
A=M
M=D
@SP
M=M+1
@8
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
@2
D=A+D
@SP
D=M-D
@ARG
M=D
@SP
D=M
@LCL
M=D
@Class1.set
0;JMP
(RARG1zz)
@0
D=A
@i
M=D
@5
D=A
@i
M=M+D
@SP
M=M-1
A=M
D=M
@i
A=M
M=D
@23
D=A
@SP
A=M
M=D
@SP
M=M+1
@15
D=A
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
@2
D=A+D
@SP
D=M-D
@ARG
M=D
@SP
D=M
@LCL
M=D
@Class2.set
0;JMP
(RARG2zz)
@0
D=A
@i
M=D
@5
D=A
@i
M=M+D
@SP
M=M-1
A=M
D=M
@i
A=M
M=D
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
@0
D=A+D
@SP
D=M-D
@ARG
M=D
@SP
D=M
@LCL
M=D
@Class1.get
0;JMP
(RARG3zz)
@RARG4zz
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
@0
D=A+D
@SP
D=M-D
@ARG
M=D
@SP
D=M
@LCL
M=D
@Class2.get
0;JMP
(RARG4zz)
(Sys.init.WHILE)
@Sys.init.WHILE
0;JMP
(Class1.set)
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
@Class1.vm0
M=D
@1
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
@Class1.vm1
M=D
@0
D=A
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
(Class1.get)
@0
D=A
@Class1.vm0
D=M
@SP
A=M
M=D
@SP
M=M+1
@1
D=A
@Class1.vm1
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
D=M-D
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
(Class2.set)
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
@Class2.vm0
M=D
@1
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
@Class2.vm1
M=D
@0
D=A
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
(Class2.get)
@0
D=A
@Class2.vm0
D=M
@SP
A=M
M=D
@SP
M=M+1
@1
D=A
@Class2.vm1
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
D=M-D
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