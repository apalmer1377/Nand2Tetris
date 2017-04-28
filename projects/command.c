#include "jack_compiler.h"

void parseCommand(FILE *f, char * token, struct command ** s) {
    *s = (struct command *) malloc(sizeof(struct command));
    (*s)->next = NULL;

    //printf("COMMAND %s\n",token);

    if (strcmp(token,"let") == 0) {
        (*s)->type = LET;
        (*s)->state = (struct letStatement *) malloc(sizeof(struct letStatement));
        parseLet(f,token,(*s)->state);
    } else if (strcmp(token,"if") == 0) {
        (*s)->type = IF;
        (*s)->state = (struct ifStatement *) malloc(sizeof(struct ifStatement));
        parseIf(f,token,(*s)->state);
    } else if (strcmp(token,"while") == 0) {
        (*s)->type = WHILE;
        (*s)->state = (struct whileStatement *) malloc(sizeof(struct whileStatement));
        parseWhile(f,token,(*s)->state);
    } else if (strcmp(token,"do") == 0) {
        (*s)->type = DO;
        (*s)->state = (struct doStatement *) malloc(sizeof(struct doStatement));
        parseDo(f,token,(*s)->state);
    } else if (strcmp(token,"return") == 0) {
        (*s)->type = RETURN;
        (*s)->state = (struct returnStatement *) malloc(sizeof(struct returnStatement));
        parseReturn(f,token,(*s)->state);
    } else {
        printf("ERROR ERROR ERROR\t%s\n",token);
    }

    //parseToken(f,token);
}

void parseLet(FILE *f, char * token, struct letStatement * state) {
    state->to = NULL;

    parseToken(f,token);
    char * vname = (char *) malloc(strlen(token) + 1);
    strcpy(vname,token);
    state->varName = vname;

    struct expression ** to = (struct expression **) malloc(sizeof(struct expression *));
    struct expression ** fro = (struct expression **) malloc(sizeof(struct expression *));
    parseToken(f,token);
    if ( token[0]  == '[') {
        parseToken(f,token);
        parseExpression(f,to,token,']');
        parseToken(f,token);
        state->to = *to;
    }

    parseToken(f,token);
    parseExpression(f,fro, token,';');
    state->fro = *fro;

    parseToken(f,token);

    free(to);
    free(fro);
}

void parseWhile(FILE *f, char * token, struct whileStatement * state) {
    parseToken(f,token);
    parseToken(f,token);

    struct expression ** exp = (struct expression **) malloc(sizeof(struct expression *));
    parseExpression(f,exp,token,')');

    parseToken(f,token);
    parseToken(f,token);

    state->cond = *exp;
    state->commands = NULL;
    int i = 0;
    struct command ** comms = (struct command **) malloc(64*sizeof(struct command *));

    while (token[0] != '}') {
        parseCommand(f,token,comms+i); 
        if (i > 0)
            comms[i-1]->next = comms[i];
        i++;
    }

    if (comms != NULL)
        state->commands = comms[0];

    parseToken(f,token);

    free(exp);
    free(comms);
}

void parseIf(FILE *f, char * token, struct ifStatement * state) {
    parseToken(f,token);
    parseToken(f,token);

    struct expression ** exp = (struct expression **) malloc(sizeof(struct expression *));
    parseExpression(f,exp,token,')');

    parseToken(f,token);
    parseToken(f,token);

    state->cond=*exp;
    state->head = NULL;
    state->elseHead = NULL;

    int i = 0;
    struct command ** comms = (struct command **) malloc(MAX_LEN*sizeof(struct command *));

    while (token[0] != '}') {
        parseCommand(f,token,comms+i);
        if (i > 0)
            comms[i-1]->next = comms[i];
        i++;
    }

    if (comms != NULL)
        state->head = comms[0];

    parseToken(f,token);

    if (strcmp(token,"else") == 0) {
        parseToken(f,token);
        parseToken(f,token);
        i=0;
        struct command ** ncomms = (struct command **) malloc(MAX_LEN*sizeof(struct command *));
        while (token[0] != '}') {
            parseCommand(f,token,ncomms+i);
            if (i > 0)
                ncomms[i-1]->next = ncomms[i];
            i++;
        }

        if (ncomms != NULL)
            state->elseHead = ncomms[0];

        parseToken(f,token);
        free(ncomms);
    }

    free(comms);

}

void parseDo(FILE *f, char * token, struct doStatement * state) {
    state->sub = (struct term *) malloc(sizeof(struct term));
    parseToken(f,token);
    state->sub->unaryOp = 0;
    parseTerm(f,token,state->sub);
    parseToken(f,token);
}

void parseReturn(FILE *f, char * token, struct returnStatement * state) {
    int o = SEEK_CUR;
    parseToken(f,token);
    if (token[0] == ';') {
        state->ret = NULL;
        parseToken(f,token);
        return;
    }
    fseek(f,0,o);
    struct expression ** exp = (struct expression **) malloc(sizeof(struct expression *));
    parseExpression(f,exp,token,';');
    state->ret = *exp;
    //free(exp);

    parseToken(f,token);
}
