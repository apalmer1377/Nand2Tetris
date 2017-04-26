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

void printIf(struct ifStatement * state) {
    printf("if (");
    printExpression(state->cond);
    printf(") {\n");
    struct command * temp = state->head;
    while (temp != NULL) {
        printCommand(temp);
        temp = temp->next;
    } 
    printf("}");
    if (state->elseHead != NULL) {
        printf(" else {\n");
        temp = state->elseHead;
        while (temp != NULL) {
            printCommand(temp);
            temp = temp->next;
        }
    }
    printf("}\n");
}

void printWhile(struct whileStatement * state) {
    printf("while (");
    printExpression(state->cond);
    printf(") {\n");
    struct command * temp = state->commands;
    while (temp != NULL) {
        printCommand(temp);
        temp = temp->next;
    }
    printf("}\n");
}

void printDo(struct doStatement * state) {
    printf("do ");
    printTerm(state->sub);
    printf("\n");
}

void printReturn(struct returnStatement * state) {
    printf("return ");
    if (state->ret != NULL)
        printExpression(state->ret);
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
            if (t->exValue != NULL)
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

void printCommand(struct command * c) {
    switch(c->type) {
        case LET:
            printLet(c->state);
            break;
        case IF:
            printIf(c->state);
            break;
        case WHILE:
            printWhile(c->state);
            break;
        case DO:
            printDo(c->state);
            break;
        case RETURN:
            printReturn(c->state);
            break;
    }
}

void printClass(struct classDec * class) {
    printf("class %s {\n",class->name);
    
    struct var * temp = class->vars;
    while (temp != NULL) {
        printParam(temp);
        temp = temp->next;
    }

    struct subDec * sub = class->subs;
    while (sub != NULL) {
        printSub(sub);
        sub = sub->next;
    }
    printf("}\n");

}

void printSub(struct subDec * sub) {
    printf("%s %s %s(",sub->type,sub->varType,sub->name);

    struct var * temp = sub->paramList;
    if (temp != NULL) {
        printVar(temp);
        temp = temp->next;
        while (temp != NULL) {
            printf(", ");
            printVar(temp);
            temp = temp->next;
        }
    }
    printf(") {\n");
    free(temp);

    struct command * comm = sub->comm;
    while (comm != NULL) {
        printCommand(comm);
        comm = comm->next;
    }
    free(comm);

}

void printVar(struct var *v) {
    printf("%s ",v->vtype);
    printParam(v);
    printf("\n");
}

void printParam(struct var * v) {
    printf("%s %s",v->type,v->name);
}