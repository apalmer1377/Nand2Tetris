

#ifndef PARSE
#define PARSE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 256
#define OPS "+-*/&|<>="

enum op {
    PLUS,
    MINUS,
    TIMES,
    DIVIDE,
    AND,
    OR,
    LT,
    GT,
    EQ
};

enum sub {
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

enum classVarType {
    STATIC,
    FIELD
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
    char * type;
    char * name;
} var;

struct varDec {
    struct var * variable;
    struct varDec *next;
} varDec;

struct term {
    enum termType type;
    char * value;
    int isInt;
    int intValue;
    struct expression ** exValue;
} term;

struct expression {
    struct term * t;
    struct opterm ** op;
} expression;

struct command {
    enum commandType type;
    void * state;
} command;

struct letStatement {
    char * varName;
    struct expression * to;
    struct expression * fro;
} letStatement;

struct ifStatement {
    struct expression * ifExpression;
    struct command ** head;
    struct command ** elseHead;
} ifStatement;

struct whileStatement {
    struct expression * cond;
    struct command ** commands;
} whileStatement;

struct doStatement {
    struct subDec * sub;
} doStatement;

struct returnStatement {
    struct expression * ret;
} returnStatement;

struct subDec {
    enum sub type;
    char * varType;
    char * name;
    struct var * paramList;
} subDec;

struct classDec {
    char *name;
    struct classVar ** vars;
    struct subroutineDec ** subs;
} classDec;

struct classVar {
    enum classVarType ctype;
    char * type;
    char * name;
} classVar;

struct subroutineDec {

} subroutineDec;

struct opterm {
    char oper;
    struct term * t;
} opterm;


void parseFile(char*);
char parseToken(FILE*,char*);
void skipLine(FILE*);
void skipComment(FILE*,char*);
void parseClass(FILE*,char*);
void parseLet(FILE*,char*,struct letStatement *);
void parseExpression(FILE*,struct expression**,char*,char);
void parseTerm(FILE*,char*,struct term*);
void parseOpTerm(FILE*,char*,struct opterm**,int);
int isInArray(char,char*);
void parseString(FILE*,char*);
void printLet(struct letStatement*);
void printExpression(struct expression*);
void printTerm(struct term*);
void parseWhile(FILE*,char*);

#endif
