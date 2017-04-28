#include "jack_compiler.h"

void parseClass(FILE *f, char * token, struct classDec ** newClass) {

    (*newClass) = (struct classDec *) malloc(CLASS_SIZE);
    parseToken(f,token);

    (*newClass)->name = (char *) malloc(strlen(token) + 1); 
    strcpy((*newClass)->name,token);

    (*newClass)->vars = NULL;
    (*newClass)->subs = NULL;
    char c;
    c= parseToken(f,token);
    if (c != '{')
        printf("error\n");

    int i = 0,j = 0;
    char * vtype = NULL;
    char * type = NULL;
    struct var ** temp = (struct var **) malloc(16*sizeof(struct var *));
    struct subDec ** stemp = (struct subDec **) malloc(16*sizeof(struct subDec *));
    parseToken(f,token);
    while (token[0] != '}') {
        if (strcmp(token,"static") == 0 || strcmp(token,"field") == 0) {

            i = parseVars(f,token,temp,i);
            continue;

        } 

        //parseToken(f,token);

        if (strcmp(token,"constructor") == 0 || strcmp(token,"function") == 0 || strcmp(token,"method") == 0) {

            parseSub(f,token,stemp+j);
            if (j > 0)
                stemp[j-1]->next = stemp[j];
            j++;

            parseToken(f,token);

            //printSub(stemp[j-1]);

            continue;
        }
    }

    if (i>0)
        (*newClass)->vars = *temp;

    if (j>0)
        (*newClass)->subs = *stemp;

    free(temp);
    free(stemp);

    return;
}

int parseVars(FILE *f, char * token, struct var ** vars, int i) {
    char * vtype = (char *) malloc(strlen(token) + 1);
    strcpy(vtype,token);
    parseToken(f,token);
    char * type = (char *) malloc(strlen(token) + 1);
    strcpy(type,token);
    while (token[0] != ';') {
        parseVar(f,token,vtype,type,vars + i);
        if (i > 0)
            vars[i-1]->next = vars[i];
        i++;
    }

    parseToken(f,token);

    free(vtype);
    free(type);

    return i;
}

void parseVar(FILE * f, char * token, char * vvtype, char * ttype, struct var ** cvar) {
    *cvar = (struct var *) malloc(VAR_SIZE);
    parseToken(f,token);


    (*cvar)->vtype = (char *) malloc(strlen(vvtype) + 1);
    (*cvar)->type = (char *) malloc(strlen(ttype) + 1);
    (*cvar)->name = (char *) malloc(strlen(token) + 1);

    strcpy((*cvar)->vtype,vvtype);
    strcpy((*cvar)->type,ttype);
    strcpy((*cvar)->name,token);

    //(*cvar)->vtype = v;
    //(*cvar)->type = t;
    //(*cvar)->name = n;

    (*cvar)->next = NULL;

    parseToken(f,token);
}

