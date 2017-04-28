#include "jack_compiler.h"

void outputClass(char * buff,struct classDec * class) {
    strcat(buff,"<class>\n");
    outputType(buff,"class");
    outputType(buff,class->name);
    outputSymbol(buff,"{");

    struct var * temp = class->vars;
    while (temp != NULL) {
        outputVar(buff,temp);
        temp = temp->next;
    }

    struct subDec * sub = class->subs;
    while (sub != NULL) {
        outputSub(buff,sub);
        sub = sub->next;
    }

    outputSymbol(buff,"}");
    strcat(buff,"</class>\n");
    free(temp);
    free(sub);
}

void outputSub(char * buff, struct subDec * sub) {
    strcat(buff,"<subroutineDec>\n<keyword> ");
    switch(sub->type) {
        case CONSTRUCTOR:
            strcat(buff,"constructor");
            break;
        case FUNCTION:
            strcat(buff,"function");
            break;
        case METHOD:
            strcat(buff,"method");
            break;
    }
    strcat(buff," </keyword>\n");
    outputType(buff,sub->varType);
    outputType(buff,sub->name);
    outputSymbol(buff,"(");

    strcat(buff,"<parameterList>\n");

    struct var * temp = sub->paramList;
    if (temp != NULL) {
        outputParam(buff,temp);
        temp = temp->next;
        while (temp != NULL) {
            outputSymbol(buff,",");
            outputParam(buff,temp);
            temp = temp->next;
        }
    }

    strcat(buff,"</parameterList>\n");
    outputSymbol(buff,")");
    strcat(buff,"<subroutineBody>\n");
    outputSymbol(buff,"{");

    temp = sub->decs;
    while (temp != NULL) {
        outputVar(buff,temp);
        temp = temp->next;
    }

    free(temp);


    strcat(buff,"<statements>\n");
    struct command * comm = sub->comm;
    while (comm != NULL) {
        outputCommand(buff,comm);
        comm = comm->next;
    }
    strcat(buff,"</statements>\n");
    outputSymbol(buff,"}");
    strcat(buff,"</subroutineBody>\n</subroutineDec>\n");
    free(comm);

}

void outputVar(char * buff, struct var *v) {
    if (strcmp(v->vtype,"static") == 0 || strcmp(v->vtype,"field") == 0)
        strcat(buff,"<classVarDec>\n");
    else
        strcat(buff,"<varDec>\n");

    outputType(buff,v->vtype);
    outputParam(buff,v);
    outputSymbol(buff,";");

    if (strcmp(v->vtype,"static") == 0 || strcmp(v->vtype,"field") == 0)
        strcat(buff,"</classVarDec>\n");
    else
        strcat(buff,"</varDec>\n");

}

void outputParam(char * buff, struct var * v) {
    outputType(buff,v->type);
    outputType(buff,v->name);
}

void outputCommand(char * buff, struct command * c) {
    switch(c->type) {
        case LET:
            strcat(buff,"<letStatement>\n");
            outputLet(buff,c->state);
            strcat(buff,"</letStatement>\n");
            break;
        case IF:
            strcat(buff,"<ifStatement>\n");
            outputIf(buff,c->state);
            strcat(buff,"</ifStatement>\n");
            break;
        case WHILE:
            strcat(buff,"<whileStatement>\n");
            outputWhile(buff,c->state);
            strcat(buff,"</whileStatement>\n");
            break;
        case DO:
            strcat(buff,"<doStatement>\n");
            outputDo(buff,c->state);
            strcat(buff,"</doStatement>\n");
            break;
        case RETURN:
            strcat(buff,"<returnStatement>\n");
            outputReturn(buff,c->state);
            strcat(buff,"</returnStatement>\n");
            break;
    }
}

void outputLet(char * buff, struct letStatement * state) {
    outputType(buff,"let");
    outputType(buff,state->varName);
    if (state->to != NULL) {
        outputSymbol(buff,"[");
        outputExpression(buff,state->to);
        outputSymbol(buff,"]");
    } 
    outputSymbol(buff,"=");
    outputExpression(buff,state->fro);
    outputSymbol(buff,";");
}

void outputIf(char * buff, struct ifStatement * state) {
    outputType(buff,"if");
    outputSymbol(buff,"(");
    outputExpression(buff,state->cond);
    outputSymbol(buff,")");
    outputSymbol(buff,"{");
    struct command * temp = state->head;
    strcat(buff,"<statements>\n");
    while (temp != NULL) {
        outputCommand(buff,temp);
        temp = temp->next;
    } 
    strcat(buff,"</statements>\n");
    if (state->elseHead != NULL) {
        outputSymbol(buff,"}");
        outputType(buff,"else");
        outputSymbol(buff,"{");
        temp = state->elseHead;
        strcat(buff,"<statements>\n");
        while (temp != NULL) {
            outputCommand(buff,temp);
            temp = temp->next;
        }
        strcat(buff,"</statements>\n");

    }
    outputSymbol(buff,"}");
    free(temp);
}

void outputWhile(char * buff, struct whileStatement * state) {
    outputType(buff,"while");
    outputSymbol(buff,"(");
    outputExpression(buff,state->cond);
    outputSymbol(buff,")");
    outputSymbol(buff,"{");
    strcat(buff,"<statements>\n");
    struct command * temp = state->commands;
    while (temp != NULL) {
        outputCommand(buff,temp);
        temp = temp->next;
    }

    strcat(buff,"</statements>\n");
    outputSymbol(buff,"}");
    free(temp);
}

void outputDo(char * buff, struct doStatement * state) {
    outputType(buff,"do");
    state->sub->unaryOp = 'D';
    outputTerm(buff,state->sub);
    outputSymbol(buff,";");
}

void outputReturn(char * buff, struct returnStatement * state) {
    outputType(buff,"return");
    if (state->ret != NULL)
        outputExpression(buff,state->ret);
    outputSymbol(buff,";");
}

void outputExpression(char * buff, struct expression * exp) {
    strcat(buff,"<expression>\n");
    outputTerm(buff,exp->t);

    struct opterm * temp = exp->op;
    while (temp != NULL) {
        outputOpTerm(buff,temp);
        temp = temp->next;
    }
    free(temp);

    strcat(buff,"</expression>\n");
}

void outputOpTerm(char * buff, struct opterm * op) {
    char * t = (char *) malloc(2);
    t[0] = op->oper;
    t[1] = '\0';
    outputSymbol(buff,t);
    outputTerm(buff,op->t);
    free(t);
}

void outputTerm(char * buff, struct term * t) {
    if (t->unaryOp != 'D') {
        strcat(buff,"<term>\n");
        if (t->unaryOp) {
            char * m = (char*) malloc(2);
            m[0] = t->unaryOp;
            m[1] = '\0';
            outputSymbol(buff,m);
            strcat(buff,"<term>\n");
            free(m);
        }
    }
    switch(t->type) {
        case INT:
            outputInt(buff,t->intValue);
            break;
        case STRING:
            outputString(buff,t->value);
            break;
        case SUB:
            outputSubCall(buff,t->value);
        case EXP:
            outputSymbol(buff,"(");
            if (t->type == SUB)
                strcat(buff,"<expressionList>\n");
            if (t->exValue != NULL) {
                struct expression * temp = (t->exValue);
                outputExpression(buff,temp);
                temp = temp->next;
                while (temp != NULL) {
                    outputSymbol(buff,",");
                    outputExpression(buff,temp);
                    temp = temp->next;
                }
                free(temp);
            }
            if (t->type == SUB)
                strcat(buff,"</expressionList>\n");
            outputSymbol(buff,")");
            break;
        case VAR:
            outputType(buff,t->value);
            if (t->exValue != NULL) {
                outputSymbol(buff,"[");
                outputExpression(buff,(t->exValue));
                outputSymbol(buff,"]");
            }
            break;
    }
    if (t->unaryOp != 'D') {
        if (t->unaryOp)
            strcat(buff,"</term>\n");
        strcat(buff,"</term>\n");
    }
}

void outputSymbol(char *buff, char * sym) {
    strcat(buff,"<symbol> ");
    if (strcmp(sym,"<") == 0)
        strcat(buff,"&lt;");
    else if (strcmp(sym,">") == 0)
        strcat(buff,"&gt;");
    else
        strcat(buff,sym);
    strcat(buff," </symbol>\n");
}

void outputType(char *buff, char * type) {
    if (isKeywordConstant(type)) {
        strcat(buff,"<keywordConstant> ");
        strcat(buff,type);
        strcat(buff," </keywordConstant>\n");
        return;
    }

    if (isKeyword(type)) {
        strcat(buff,"<keyword> ");
        strcat(buff,type);
        strcat(buff," </keyword>\n");
        return;
    }

    strcat(buff,"<identifier> ");
    strcat(buff,type);
    strcat(buff, " </identifier>\n");
}

void outputInt(char * buff, int i) {
    strcat(buff,"<integerConstant> ");
    strcat(buff,itoa(i));
    strcat(buff," </integerConstant>\n");
}

void outputString(char * buff, char * c) {
    strcat(buff,"<stringConstant> ");
    strcat(buff,c);
    strcat(buff," </stringConstant>\n");
}

void outputSubCall(char * buff, char * c) {
    char * t = strchr(c,'.');
    if (t != NULL) {
        int s = t - c; 
        char * class = (char *) malloc(s+1);
        char * method = (char *) malloc(strlen(c) - s);
        int i,j;
        for (i=0;i<s;i++) {
            class[i] = c[i];
        }
        class[s] = '\0';

        for (j=s+1;j < strlen(c);j++) {
            method[j-s-1] = c[j];
        }
        method[strlen(c) - s - 1] = '\0';

        outputType(buff,class);
        outputSymbol(buff,".");
        outputType(buff,method);

        free(class);
        free(method);

        return;
    }

    outputType(buff,c);
}
