#include "jack_compiler.h"

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
    //printf("%s\n",arr);
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
    if (state->elseHead != NULL) {
        printf("} else {\n");
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
    //printf("%s\n",exp->t->value);
    printTerm(exp->t);

    struct opterm * temp = exp->op;
    while (temp != NULL) {
        printf(" %c ",temp->oper);
        printTerm(temp->t);
        temp = temp->next;
    }
    free(temp);
}

void printTerm(struct term * t) {
    if (t->unaryOp)
        printf("%c",t->unaryOp);
    switch(t->type) {
        case INT:
            printf("%i",t->intValue);
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
            if (t->exValue != NULL) {
                struct expression * temp = (t->exValue);
                printExpression(temp);
                temp = temp->next;
                while (temp != NULL) {
                    printf(",");
                    printExpression(temp);
                    temp = temp->next;
                }
            }
            printf(")");
            break;
        case VAR:
            printf(t->value);
            if (t->exValue != NULL) {
                printf("[");
                printExpression((t->exValue));
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
        printVar(temp);
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
    switch(sub->type) {
        case CONSTRUCTOR:
            printf("constructor");
            break;
        case FUNCTION:
            printf("function");
            break;
        case METHOD:
            printf("method");
            break;
    }
    printf(" %s %s(",sub->varType,sub->name);

    struct var * temp = sub->paramList;
    if (temp != NULL) {
        printParam(temp);
        temp = temp->next;
        while (temp != NULL) {
            printf(", ");
            printParam(temp);
            temp = temp->next;
        }
    }
    printf(") {\n");

    temp = sub->decs;
    while (temp != NULL) {
        printVar(temp);
        temp = temp->next;
    }

    free(temp);


    struct command * comm = sub->comm;
    while (comm != NULL) {
        printCommand(comm);
        comm = comm->next;
    }
    printf("}\n");
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

int isKeyword(char * word) {
    char * keywords[] = {"class","contructor","function","method","field","static","var","int","char","boolean","void","true","false","null","this","let","do","if","else","while","return"};
    int i;
    for (i=0;i<21;i++) {
        if (strcmp(word,keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int isKeywordConstant(char * word) {
    char * keyconsts[] = {"true","false","null","this"};
    int i;
    for (i=0;i<4;i++) {
        if (strcmp(word,keyconsts[i]) == 0)
            return 1;
    }
    return 0;

}

char * itoa(int i) {
  char * res = malloc(8*sizeof(int));
  sprintf(res, "%d", i);
  return res;
}

void reverse(char * s)
{
    int length = strlen(s) ;
    int c, i, j;

    for (i = 0, j = length - 1; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

char *get_filename_ext(char *filename) {
    char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

char *get_short_fname(char *filename) {
    char *dot = strrchr(filename, '/');
    if (!dot || dot == filename) return filename;
    return dot + 1;
}

char * strip_extension(char * filename) {
    char * slash = strrchr(filename,'/');
    if (slash)
        return strip_extension(slash+1);
    char * dot = strrchr(filename, '.');
    char * ret = (char *) malloc(dot - filename + 1);
    int i;
    for (i=0;i<(dot-filename);i++)
        ret[i] = filename[i];

    ret[dot-filename] = '\0';
    return ret;
}

