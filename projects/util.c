#include "parser.h"

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
           return; 
    }
}

void skipComment(FILE * f, char * temp) {
    char c;
    while ((c = parseToken(f,temp)) != EOF) {
        if (strcmp(temp,"*/") == 0)
           return; 
    }
    return;
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

