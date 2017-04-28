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
    
    if (i > 0)
        (*sub)->paramList = *params;

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
        
        parseCommand(f,token,s+j);
        if (j > 0) {
            s[j-1]->next = s[j];
        } 
        j++;

    }

    if (i>0)
        (*sub)->decs = *vars;

    if (j>0)
        (*sub)->comm = *s;

    free(params);
    free(vars);
    free(s);
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

    (*param)->type = (char *) malloc(strlen(token) + 1);
    strcpy((*param)->type,token);

    parseToken(f,token);
    (*param)->name = (char *) malloc(strlen(token) + 1);
    strcpy((*param)->name,token);

    (*param)->vtype = NULL;
    //(*param)->type = type;
    //(*param)->name = name;
    (*param)->next = NULL;

    parseToken(f,token);
    if (token[0] == ',')
        parseToken(f,token);

}
