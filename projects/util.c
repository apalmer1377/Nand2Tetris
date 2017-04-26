#include "parser.h"

char parseToken(FILE * f, char * arr) {
    char c;
    int i = 0;
    while ((c = getc(f)) != EOF) {
        if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
            if (i == 0)
                continue;
            break;
        }
        if (isInArray(c,OPS) || isInArray(c,SYMBOLS)) {
            if (i == 0) {
                arr[i++] = c;
                if (c == '/') {
                    c = getc(f);
                    if (c == '/') {
                        skipLine(f);
                        return parseToken(f,arr);
                    }
                    if (c == '*') {
                        c = getc(f);
                        if (c == '*') {
                            skipComment(f,arr);
                            return parseToken(f,arr);
                        }
                        fseek(f,-1,SEEK_CUR);
                    }
                    fseek(f,-1,SEEK_CUR);
                    c = '/';
                }
            } else
                fseek(f,-1,SEEK_CUR);
            break;
        }
        arr[i++] = c;
    }
    arr[i] = '\0';
    return c;
}

int isInArray(char c,char * arr) {
    int i;
    for (i=0; i < strlen(arr);i++) {
        if (c == arr[i])
            return 1;
    }
    return 0;
}

void skipLine(FILE * f) {
    char c;
    while ((c = getc(f)) != EOF) {
        if (c == '\n' || c == '\r')
           break; 
    }
}

void skipComment(FILE * f, char * temp) {
    char c;
    while ((c = getc(f)) != EOF) {
        if (c == '*') {
            c = getc(f);
            if (c == '/')
                break;
            fseek(f,-1,SEEK_CUR);
        }
    }
}

void printLet(struct letStatement * state) {
    printf("let %s",state->varName);
    if (state->to != NULL) {
        printf("[");
        printExpression(state->to);
        printf("]");
    } 
    printf(" = ");
    printExpression(state->fro);
    printf("\n");
}

void printDo(struct doStatement * state) {
    printf("do ");
    printTerm(state->sub);
    printf("\n");
}

void printExpression(struct expression * exp) {
    printTerm(exp->t);

    if (exp->op != NULL) {
        int i = 0;
        while (exp->op[i] != NULL) {
            printf(" %c ",exp->op[i]->oper);
            printTerm(exp->op[i]->t);
            i++;
        }
    }
}

void printTerm(struct term * t) {
    switch(t->type) {
        case INT:
            printf("%i ",t->intValue);
            break;
        case STRING:
            printf("\"");
            printf(t->value);
            printf("\"");
            break;
        case SUB:
            printf(t->value);
        case EXP:
            printf("(");
            printExpression(*(t->exValue));
            printf(")");
            break;
        case VAR:
            printf(t->value);
            if (t->exValue != NULL) {
                printf("[");
                printExpression(*(t->exValue));
                printf("]");
            }
            break;
    }
}

