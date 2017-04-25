#include "parser.h"

void parseLet(FILE *f, char * token, struct letStatement * state) {
    //struct letStatement * state = (struct letStatement *) malloc(sizeof(struct letStatement *));
    state->to = NULL;

    parseToken(f,token);
    //printf("let %s\n",token);
    char * vname = (char *) malloc(strlen(token) + 1);
    strcpy(vname,token);
    state->varName = vname;

    struct expression ** to = (struct expression **) malloc(sizeof(struct expression *));
    struct expression ** fro = (struct expression **) malloc(sizeof(struct expression *));
    char c = parseToken(f,token);
    if ( token[0]  == '[') {
        parseExpression(f,to,token,']');
        parseToken(f,token);
        state->to = *to;
    }

    parseExpression(f,fro, token,';');
    state->fro = *fro;

    printLet(state);
}

void parseWhile(FILE *f, char * token) {
    struct whileStatement * state = (struct whileStatement *) malloc(sizeof(struct whileStatement *));
    parseToken(f,token);

    struct expression ** exp = (struct expression **) malloc(sizeof(struct expression));
    parseExpression(f,exp,token,')');

    state->cond = *exp;
    state->commands = NULL;
    char c;
    while ((c = parseToken(f,token)) != EOF) {
        if (token[0] == '}')
            break;

    }


}
