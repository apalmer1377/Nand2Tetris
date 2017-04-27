#include "jack_compiler.h"

void parseSub(FILE * f,char * token, struct subDec ** sub) {
    initSub(f,token,sub);
    struct var ** params = (struct var **) malloc(MAX_LEN*sizeof(struct var *));
    int i = 0;
    while (token[0] != ')') {
        parseParam(f,token,params + i);
        if (i > 0)
            params[i-1]->next = params[i];
        i++;
    }
    
    if (params != NULL)
        (*sub)->paramList = params[0];

    parseToken(f,token);
    parseToken(f,token);

    i = 0;
    int j = 0;
    struct var ** vars = (struct var **) malloc(MAX_LEN*sizeof(struct var *));
    struct command ** s = (struct command **) malloc(MAX_LEN*sizeof(struct command *));
    while (token[0] != '}') {

        if (strcmp(token,"var") == 0) {
            i = parseVars(f,token,vars,i);
            continue;
        }
        
        /*
        if (j == 0) {
            struct var * v = vars[0];
            while (v != NULL) {
                printVar(v);
                v = v->next;
            }
        }*/

        parseCommand(f,token,s+j);
        //printCommand(s[j]);
        if (j > 0) {
            s[j-1]->next = s[j];
        } 
        j++;

    }

    (*sub)->decs = *vars;
    (*sub)->comm = *s;
}

void initSub(FILE *f, char * token, struct subDec ** sub) {
    (*sub) = (struct subDec *) malloc(SUB_SIZE);

    (*sub)->type = FUNCTION;
    if (strcmp(token,"function") == 0)
        (*sub)->type = FUNCTION;
    if (strcmp(token,"constructor") == 0)
        (*sub)->type = CONSTRUCTOR;
    if (strcmp(token,"method") == 0)
        (*sub)->type = METHOD;

    char * throwaway = (char *) malloc(strlen(token) + 1);
    strcpy(throwaway,token);
    parseToken(f,token);

    (*sub)->varType = (char *) malloc(strlen(token) + 1);
    strcpy((*sub)->varType,token);

    parseToken(f,token);
    (*sub)->name = (char *) malloc(strlen(token) + 1);
    strcpy((*sub)->name,token);

    (*sub)->paramList = NULL;
    (*sub)->decs = NULL;
    (*sub)->comm = NULL;
    (*sub)->next = NULL;

    parseToken(f,token);
    parseToken(f,token);
}

void parseParam(FILE * f,char * token,struct var ** param) {
    *param = (struct var *) malloc(VAR_SIZE);

    char * type = (char *) malloc(strlen(token) + 1);
    strcpy(type,token);

    parseToken(f,token);
    char * name = (char *) malloc(strlen(token) + 1);
    strcpy(name,token);

    (*param)->vtype = NULL;
    (*param)->type = type;
    (*param)->name = name;
    (*param)->next = NULL;

    parseToken(f,token);
    if (token[0] == ',')
        parseToken(f,token);

}
