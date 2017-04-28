#include "jack_compiler.h"

void tokenizeClass(char * buff,struct classDec * class) {

    strcat(buff,"<tokens>\n");
    tokenizeType(buff,"class");
    tokenizeType(buff,class->name);
    tokenizeSymbol(buff,"{");

    struct var * temp = class->vars;
    while (temp != NULL) {
        tokenizeVar(buff,temp);
        temp = temp->next;
    }

    struct subDec * sub = class->subs;
    while (sub != NULL) {
        tokenizeSub(buff,sub);
        sub = sub->next;
    }

    tokenizeSymbol(buff,"}");
    strcat(buff,"</tokens>\n");

    free(temp);
    free(sub);

}

void tokenizeSub(char * buff, struct subDec * sub) {
    strcat(buff,"<keyword> ");
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
    tokenizeType(buff,sub->varType);
    tokenizeType(buff,sub->name);
    strcat(buff,"<symbol> ( </symbol>\n");

    struct var * temp = sub->paramList;
    if (temp != NULL) {
        tokenizeParam(buff,temp);
        temp = temp->next;
        while (temp != NULL) {
            strcat(buff,"<symbol> , </symbol>\n");
            tokenizeParam(buff,temp);
            temp = temp->next;
        }
    }
    tokenizeSymbol(buff,")");
    tokenizeSymbol(buff,"{");

    temp = sub->decs;
    while (temp != NULL) {
        tokenizeVar(buff,temp);
        temp = temp->next;
    }
    free(temp);

    struct command * comm = sub->comm;
    while (comm != NULL) {
        tokenizeCommand(buff,comm);
        comm = comm->next;
    }
    tokenizeSymbol(buff,"}");
    free(comm);

}

void tokenizeVar(char * buff, struct var *v) {
    tokenizeType(buff,v->vtype);
    tokenizeParam(buff,v);
    tokenizeSymbol(buff,";");
}

void tokenizeParam(char * buff, struct var * v) {
    tokenizeType(buff,v->type);
    tokenizeType(buff,v->name);
}

void tokenizeCommand(char * buff, struct command * c) {
    switch(c->type) {
        case LET:
            tokenizeLet(buff,c->state);
            break;
        case IF:
            tokenizeIf(buff,c->state);
            break;
        case WHILE:
            tokenizeWhile(buff,c->state);
            break;
        case DO:
            tokenizeDo(buff,c->state);
            break;
        case RETURN:
            tokenizeReturn(buff,c->state);
            break;
    }
}

void tokenizeLet(char * buff, struct letStatement * state) {
    tokenizeType(buff,"let");
    tokenizeType(buff,state->varName);
    if (state->to != NULL) {
        tokenizeSymbol(buff,"[");
        tokenizeExpression(buff,state->to);
        tokenizeSymbol(buff,"]");
    } 
    tokenizeSymbol(buff,"=");
    tokenizeExpression(buff,state->fro);
    tokenizeSymbol(buff,";");
}

void tokenizeIf(char * buff, struct ifStatement * state) {
    tokenizeType(buff,"if");
    tokenizeSymbol(buff,"(");
    tokenizeExpression(buff,state->cond);
    tokenizeSymbol(buff,")");
    tokenizeSymbol(buff,"{");
    struct command * temp = state->head;
    while (temp != NULL) {
        tokenizeCommand(buff,temp);
        temp = temp->next;
    } 
    if (state->elseHead != NULL) {
        tokenizeSymbol(buff,"}");
        tokenizeType(buff,"else");
        tokenizeSymbol(buff,"{");
        temp = state->elseHead;
        while (temp != NULL) {
            tokenizeCommand(buff,temp);
            temp = temp->next;
        }
    }
    tokenizeSymbol(buff,"}");
    free(temp);
}

void tokenizeWhile(char * buff, struct whileStatement * state) {
    tokenizeType(buff,"while");
    tokenizeSymbol(buff,"(");
    tokenizeExpression(buff,state->cond);
    tokenizeSymbol(buff,")");
    tokenizeSymbol(buff,"{");
    struct command * temp = state->commands;
    while (temp != NULL) {
        tokenizeCommand(buff,temp);
        temp = temp->next;
    }
    tokenizeSymbol(buff,"}");
    free(temp);
}

void tokenizeDo(char * buff, struct doStatement * state) {
    tokenizeType(buff,"do");
    tokenizeTerm(buff,state->sub);
    tokenizeSymbol(buff,";");
}

void tokenizeReturn(char * buff, struct returnStatement * state) {
    tokenizeType(buff,"return");
    if (state->ret != NULL)
        tokenizeExpression(buff,state->ret);
    tokenizeSymbol(buff,";");
}

void tokenizeExpression(char * buff, struct expression * exp) {
    tokenizeTerm(buff,exp->t);

    struct opterm * temp = exp->op;
    while (temp != NULL) {
        tokenizeOpTerm(buff,temp);
        temp = temp->next;
    }
    free(temp);
}

void tokenizeOpTerm(char * buff, struct opterm * op) {
    char * t = (char *) malloc(2);
    t[0] = op->oper;
    t[1] = '\0';
    tokenizeSymbol(buff,t);
    tokenizeTerm(buff,op->t);
    free(t);
}

void tokenizeTerm(char * buff, struct term * t) {
    if (t->unaryOp) {
        char * m = (char*) malloc(2);
        m[0] = t->unaryOp;
        m[1] = '\0';
        tokenizeSymbol(buff,m);
        free(m);
    }
    switch(t->type) {
        case INT:
            tokenizeInt(buff,t->intValue);
            break;
        case STRING:
            tokenizeString(buff,t->value);
            break;
        case SUB:
            tokenizeSubCall(buff,t->value);
            //tokenizeType(buff,t->value);
        case EXP:
            tokenizeSymbol(buff,"(");
            if (t->exValue != NULL) {
                struct expression * temp = (t->exValue);
                tokenizeExpression(buff,temp);
                temp = temp->next;
                while (temp != NULL) {
                    tokenizeSymbol(buff,",");
                    tokenizeExpression(buff,temp);
                    temp = temp->next;
                }
                free(temp);
            }
            tokenizeSymbol(buff,")");
            break;
        case VAR:
            tokenizeType(buff,t->value);
            if (t->exValue != NULL) {
                tokenizeSymbol(buff,"[");
                tokenizeExpression(buff,(t->exValue));
                tokenizeSymbol(buff,"]");
            }
            break;
    }
}

void tokenizeSymbol(char *buff, char * sym) {
    strcat(buff,"<symbol> ");
    if (strcmp(sym,"<") == 0)
        strcat(buff,"&lt;");
    else if (strcmp(sym,">") == 0)
        strcat(buff,"&gt;");
    else
        strcat(buff,sym);
    strcat(buff," </symbol>\n");
}

void tokenizeType(char *buff, char * type) {
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

void tokenizeInt(char * buff, int i) {
    strcat(buff,"<integerConstant> ");
    strcat(buff,itoa(i));
    strcat(buff," </integerConstant>\n");
}

void tokenizeString(char * buff, char * c) {
    strcat(buff,"<stringConstant> ");
    strcat(buff,c);
    strcat(buff," </stringConstant>\n");
}

void tokenizeSubCall(char * buff, char * c) {
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

        tokenizeType(buff,class);
        tokenizeSymbol(buff,".");
        tokenizeType(buff,method);

        free(class);
        free(method);

        return;
    }

    tokenizeType(buff,c);
}
