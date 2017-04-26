#include "parser.h"

int main(int argc, char **argv) {
    char * fname = argv[1];
    printf("%s\n",fname);
    parseFile(fname);
    return 0;
}

void parseFile(char *file) {
    FILE * f = fopen(file,"r");
    if (f == NULL)
        exit(1);

    char * token = malloc(MAX_LEN);
    char * temp = token;
    void * ptr = NULL;
    void * fin = NULL;
    char c;
    while ((c = parseToken(f , token)) != EOF) {
        if (strcmp(token,"let") == 0) {
            ptr = (struct letStatement *) realloc(ptr,sizeof(struct letStatement *));
            parseLet(f,token,ptr);
            continue;
        } 
        if (strcmp(token,"class") == 0) {
            parseClass(f,token);
            continue;  
        }
        printf("%s\n",token); 

    }
    return;
}

void parseExpression(FILE* f, struct expression **exp, char* token, char end) {
    struct term * t = (struct term *) malloc(sizeof(struct term *));
    struct opterm ** ot = (struct opterm **) malloc(10*sizeof(struct opterm *));
    (*exp) = (struct expression *) malloc(sizeof(struct expression *));

    parseTerm(f,token,t);
    
    char c;
    int j = 0;
    //while ((c = parseToken(f,token)) != EOF) {
    while (token[0] != end) {
        if (end == ',' && token[0] == ')')
            break;

        if (isInArray(token[0],OPS)) {
            parseOpTerm(f,token,ot,j++);
        }

    }

    (*exp)->t = t;
    (*exp)->op = ot;

    return;
}

void parseTerm(FILE* f,char * token, struct term * t) {
    //char c = parseToken(f,token);
    char * temp = (char*) malloc(strlen(token)+1);
    int i = atoi(token);

    t->type = INT;
    t->value = NULL;
    t->isInt = 1;
    t->intValue = i;
    t->exValue = NULL;

    if (token[0] == '(') {
        t->type = EXP;
        struct expression **texp = (struct expression **) malloc(sizeof(struct expression *));
        parseToken(f,token);
        parseExpression(f,texp,token,')');
        t->exValue = texp;
        return;
    }

    if (i == 0 && strcmp(token,"0") != 0){
        t->isInt = 0;
        t->type = VAR;
        if (token[0] == '"') {
            t->type = STRING;
            parseString(f,token);
        }
        strcpy(temp,token);
        t->value = temp;
    }    

    parseToken(f,token);
    char c = token[0];
    if (c == '[' || c == '(') {
        parseToken(f,token);
        struct expression **texp = (struct expression **) malloc(sizeof(struct expression *));
        if ( c == '['){
            parseExpression(f,texp,token,']');
            i = 1;
        }else {
            t->type = SUB;
            i = 0;
            while (token[0] != ')') {       
                parseExpression(f,texp+i,token,',');
                i++; 
            }
        }
        if ( i > 0) 
            t->exValue = texp;
        else
            t->exValue = NULL;
        parseToken(f,token);
        
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
    token[i] = '\0';
    return;
}

void parseOpTerm(FILE* f,char * token, struct opterm ** op,int i) {
    op[i] = (struct opterm *) malloc(sizeof(struct opterm *));
    op[i]->oper = token[0]; 
    struct term* t = (struct term *) malloc(sizeof(struct term *));
    parseToken(f,token);
    parseTerm(f,token,t);
    op[i]->t = t;
}
