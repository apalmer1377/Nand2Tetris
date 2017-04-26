

#ifndef PARSE
#define PARSE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LEN 256
#define OPS "+-*/&|<>="
#define SYMBOLS "(){}[];,"
#define CLASS_VAR_SIZE sizeof(struct classVar *)
#define VAR_SIZE sizeof(struct var *)
#define SUB_SIZE sizeof(struct subDec *)
#define COMM_SIZE sizeof(struct command *)

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
    char * vtype;
    char * type;
    char * name;
    struct var * next;
} var;

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
    char * type;
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
} classDec;

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

#endif
