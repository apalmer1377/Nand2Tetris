#include "jack_compiler.h"

void parseExpression(FILE* f, struct expression **exp, char* token, char end) {
    struct term * t = (struct term *) malloc(sizeof(struct term));
    t->unaryOp = 0;
    struct opterm ** ot = (struct opterm **) malloc(8*sizeof(struct opterm *));
    (*exp) = (struct expression *) malloc(sizeof(struct expression));
    (*exp)->next = NULL;
    (*exp)->op = NULL;

    parseTerm(f,token,t);

    char c;
    int j = 0;
    
    while (token[0] != end) {
        if (end == ',' && token[0] == ')')
            break;

        if (isInArray(token[0],OPS)) {
            parseOpTerm(f,token,ot+j);
            if (j > 0)
                ot[j-1]->next = ot[j];
            j++;
        }

    }

    (*exp)->t = t;
    if (j>0)
        (*exp)->op = *ot;

    free(ot);

    return;
}

void parseTerm(FILE* f,char * token, struct term * t) {
    //char * temp = (char*) malloc(65);
    int i = atoi(token);

    t->type = INT;
    t->value = NULL;
    t->intValue = i;
    t->exValue = NULL;

    if (token[0] == '-' || token[0] == '~') {
        t->unaryOp = token[0];
        parseToken(f,token);
        parseTerm(f,token,t);

        return;
    }

    if (token[0] == '(') {
        t->type = EXP;
        struct expression **texp = (struct expression **) malloc(sizeof(struct expression *));
        parseToken(f,token);
        parseExpression(f,texp,token,')');
        t->exValue = texp[0];
        parseToken(f,token);
        free(texp);
        return;
    }

    if (i == 0 && strcmp(token,"0") != 0){
        t->type = VAR;
        if (token[0] == '"') {
            t->type = STRING;
            parseString(f,token);
        }
        //char * temp = (char*) malloc(strlen(token)+1);
        t->value = (char*) malloc(65);
        strcpy(t->value,token);
        //strcpy(temp,token);
        //t->value = temp;
    }    

    parseToken(f,token);
    char c = token[0];
    if (c == '[' || c == '(') {
        parseToken(f,token);
        struct expression **texp = (struct expression **) malloc(sizeof(struct expression *));
        if ( c == '['){
            parseExpression(f,texp,token,']');
            i = 1;
        } else {
            texp = (struct expression **) realloc(texp,10*sizeof(struct expression *));
            t->type = SUB;
            i = 0;
            while (token[0] != ')') {       
                if (token[0] == ',')
                    parseToken(f,token);
                parseExpression(f,texp+i,token,',');
                if (i > 0)
                    texp[i-1]->next = texp[i];
                i++; 
            }
        }

        if (i > 0) 
            t->exValue = texp[0];

        parseToken(f,token);
        free(texp);
        
    }

}

void parseString(FILE * f, char * token) {
    int i = 0;
    char c;
    while ((c = getc(f)) != EOF) {
        if (c == '"')
            break;
        token[i++] = c;
    }
    printf("\n");
    token[i] = '\0';
    return;
}

void parseOpTerm(FILE* f,char * token, struct opterm ** op) {
    (*op) = (struct opterm *) malloc(sizeof(struct opterm));
    (*op)->oper = token[0]; 
    (*op)->t = (struct term *) malloc(sizeof(struct term));
    (*op)->next = NULL;
    parseToken(f,token);
    (*op)->t->unaryOp = 0;
    parseTerm(f,token,(*op)->t);
}
