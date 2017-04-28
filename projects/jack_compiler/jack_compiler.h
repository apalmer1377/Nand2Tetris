

#ifndef PARSE
#define PARSE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>


#define MAX_LEN 256
#define OPS "+-*/&|<>=~"
#define SYMBOLS "(){}[];,\""
#define CLASS_SIZE sizeof(struct classDec)
#define VAR_SIZE sizeof(struct var)
#define SUB_SIZE sizeof(struct subDec)
#define COMM_SIZE sizeof(struct command)

enum subType {
    CONSTRUCTOR,
    FUNCTION,
    METHOD
};

enum commandType {
    LET,
    IF,
    WHILE,
    DO,
    RETURN,
    CLASS,
    DEC
};

enum termType {
    NONE,
    INT,
    STRING,
    KEYWORD,
    VAR,
    SUB,
    EXP
};

struct var {
    char * vtype;
    char * type;
    char * name;
    struct var * next;
} var;

struct term {
    enum termType type;
    char * value;
    int intValue;
    char unaryOp;
    struct expression * exValue;
} term;

struct expression {
    struct term * t;
    struct opterm * op;
    struct expression * next;
} expression;

struct command {
    enum commandType type;
    void * state;
    struct command * next;
} command;

struct letStatement {
    char * varName;
    struct expression * to;
    struct expression * fro;
} letStatement;

struct ifStatement {
    struct expression * cond;
    struct command * head;
    struct command * elseHead;
} ifStatement;

struct whileStatement {
    struct expression * cond;
    struct command * commands;
} whileStatement;

struct doStatement {
    struct term * sub;
} doStatement;

struct returnStatement {
    struct expression * ret;
} returnStatement;

struct subDec {
    enum subType type;
    char * varType;
    char * name;
    struct var * paramList;
    struct var * decs;
    struct command * comm;
    struct subDec * next;
} subDec;

struct classDec {
    char *name;
    struct var * vars;
    struct subDec * subs;
    struct classDec * next;
} classDec;

struct opterm {
    char oper;
    struct term * t;
    struct opterm * next;
} opterm;

void parseFile(char*,char*,char*);
char parseToken(FILE*,char*);
void skipLine(FILE*);
void skipComment(FILE*,char*);
void parseClass(FILE*,char*,struct classDec **);
void parseLet(FILE*,char*,struct letStatement *);
void parseExpression(FILE*,struct expression**,char*,char);
void parseTerm(FILE*,char*,struct term*);
void parseOpTerm(FILE*,char*,struct opterm**);
int isInArray(char,char*);
void parseString(FILE*,char*);
void printLet(struct letStatement*);
void printExpression(struct expression*);
void printTerm(struct term*);
void parseWhile(FILE*,char*,struct whileStatement*);
int parseVars(FILE*,char*,struct var**,int);
void parseVar(FILE*,char*,char*,char*,struct var**);
void parseSub(FILE*,char*,struct subDec**);
void initSub(FILE*,char*,struct subDec**);
void parseParam(FILE*,char*,struct var **);
void parseCommand(FILE*,char*,struct command **);
void parseIf(FILE*,char*,struct ifStatement *);
void parseDo(FILE*,char*,struct doStatement *);
void parseReturn(FILE*,char*,struct returnStatement *);
void printWhile(struct whileStatement*);
void printIf(struct ifStatement*);
void printCommand(struct command *);
void printDo(struct doStatement*);
void printVar(struct var *);
void printSub(struct subDec *);
void printClass(struct classDec*);
void printParam(struct var *);
void printReturn(struct returnStatement *);

void tokenizeClass(char*,struct classDec*);
void tokenizeSub(char*,struct subDec*);
void tokenizeVar(char*,struct var*);
void tokenizeParam(char*,struct var*);
void tokenizeCommand(char*,struct command*);
void tokenizeLet(char*,struct letStatement*);
void tokenizeIf(char*,struct ifStatement*);
void tokenizeWhile(char*,struct whileStatement*);
void tokenizeDo(char*,struct doStatement*);
void tokenizeReturn(char*,struct returnStatement*);
void tokenizeExpression(char*,struct expression *);
void tokenizeTerm(char*,struct term *);
void tokenizeType(char*,char*);
void tokenizeSymbol(char*,char*);
void tokenizeOpTerm(char*,struct opterm *);
void tokenizeInt(char*,int);
void tokenizeString(char*,char*);
void tokenizeSubCall(char*,char*);

char * itoa(int);
void reverse(char*);

int isKeyword(char *);
int isKeywordConstant(char*);

char * get_filename_ext(char*);
char * get_short_name(char*);
char * strip_extension(char*);

void outputClass(char*,struct classDec*);
void outputSub(char*,struct subDec*);
void outputVar(char*,struct var*);
void outputParam(char*,struct var*);
void outputCommand(char*,struct command*);
void outputLet(char*,struct letStatement*);
void outputIf(char*,struct ifStatement*);
void outputWhile(char*,struct whileStatement*);
void outputDo(char*,struct doStatement*);
void outputReturn(char*,struct returnStatement*);
void outputExpression(char*,struct expression *);
void outputTerm(char*,struct term *);
void outputType(char*,char*);
void outputSymbol(char*,char*);
void outputOpTerm(char*,struct opterm *);
void outputInt(char*,int);
void outputString(char*,char*);
void outputSubCall(char*,char*);

void compileFile(char*);

#endif
