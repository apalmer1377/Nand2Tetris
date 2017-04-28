# Nand2Tetris
Projects and tests for "Elements of Computing Systems," Nisan &amp; Shocken

Unless noted below, none of the files in this repository are my own work, nor doI take ownership of any of them.

As I work through each chapter, I will update my progress below, and mention which files represent my own work.

Chapter 1: projects/01/\*.hdl, all files passed tests

Chapter 2: projects/02/\*.hdl, all files passed tests

Chapter 3: projects/03/\*.hdl, all files passed tests

Chapter 4: projects/04/\*.asm, all files passed tests

Chapter 5: projects/05/\*.hdl, all files passed tests

Chapter 6: projects/06/\*.hack,assembler.scala, Assembler written in Scala passed all tests

Chapter 7: projects/07/\*.asm, translator.c, Partial VMTranslator written in C passed stage 1 and stage 2tests 
    \*\*Might revisit this as memory allocation and general code quality is not great\*\*

Chapter 8: projects/08/\*.asm, translator.c, Complete VMTranslator written in C passed stage 1 and stage 2 tests

Chapter 9: none

Chapter 10: jack\_compiler.c, xml\_formatter.c, class.c, sub.c, command.c, expression.c, util.c, jack\_compiler.h, Makefile -- both tokenizer and XML outputter written in C working.
    \*\*NOTE: The XML outputted by the program is slightly different than the examples given, for two reasons:
                1. The four "keyword constants" I marked as such, so that in the XML, "null" "this" "true" and "false" are enclosed in "<keywordConstant>" tags.  In the examples, these are just "<keyword>" tags.
                This was more of a personal preference, and doesn't affect the actual performance.
                2. When variables are declared as a list (e.g. "var int i,j;"), the program outputs each declaration on its own, rather than maintaining the original structure.  This was an oversight on my part,
                but ultimately doesn't affect the end compilation.
