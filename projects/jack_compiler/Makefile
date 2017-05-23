FILES := jack_analyzer.c tokenizer.c xml_outputter.c class.c sub.c command.c expression.c util.c jack_compiler.h
CFILES := jack_compiler.c class.c sub.c command.c expression.c util.c jack_compiler.h
CC := GCC

run: $(FILES)
	cc -o jack_analyzer $(FILES)

clean:
	rm -rf *vm *.xml jack_analyzer jack_compiler

compiler: $(CFILES)
	cc -o jack_compiler $(CFILES)
