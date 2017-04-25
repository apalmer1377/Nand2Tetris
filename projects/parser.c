#include "parser.h"

int main(int argc, char **argv) {
    parseFile("10/ArrayTest/Main.jack");
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
        if (strcmp(token,"//") == 0) {
            skipLine(f);
            continue;
        }
        if (strcmp(token,"/**") == 0) {
            skipComment(f,token);
            continue;
        } 
        if (strcmp(token,"let") == 0) {
            ptr = (struct letStatement *) realloc(ptr,sizeof(struct letStatement *));
            parseLet(f,token,ptr);
            continue;
        } 
        if (strcmp(token,"class") == 0) {
            parseClass(f,token);
            continue;  
        }
        if (strcmp(token,"while") == 0) {
            //parseWhile(f,token);
            continue;
        }

        printf("%s\n",token); 

    }
    return;
}

char parseToken(FILE * f, char * arr) {
    char c;
    int i = 0;
    while ((c = getc(f)) != EOF) {
        if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
            if (i == 0)
                continue;
            break;
        } else if (c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ';' || c == '"' || c == ',' || c == '=') {
            if (i == 0) 
                arr[i++] = c;
            else
                fseek(f,-1,SEEK_CUR);
            break;
        }
        arr[i++] = c;
    }
    arr[i] = '\0';
    return c;
}

void parseExpression(FILE* f, struct expression **exp, char* token, char end) {
    struct term * t = (struct term *) malloc(sizeof(struct term *));
    struct opterm ** ot = (struct opterm **) malloc(10*sizeof(struct opterm *));
    (*exp) = (struct expression *) malloc(sizeof(struct expression *));

    parseTerm(f,token,t);
    
    char c;
    int j = 0;
    while ((c = parseToken(f,token)) != EOF) {
        if (token[0] == end)
            break; 
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
    char c = parseToken(f,token);
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

    if (c == '[' || c == '(') {
        parseToken(f,token);
        struct expression **texp = (struct expression **) malloc(sizeof(struct expression *));
        if ( c == '[')
            parseExpression(f,texp,token,']');
        else {
            t->type = SUB;
            parseExpression(f,texp,token,',');
            i = 1;
            while (token[0] != ')') {       
                texp = (struct expression **) realloc(texp,sizeof(texp) + sizeof(struct expression *));
                parseExpression(f,texp+i,token,',');
                i++; 
            }
        }
        t->exValue = texp;
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
    //op[i]->t = (struct term *) malloc(sizeof(struct term *));
    parseTerm(f,token,t);
    op[i]->t = t;
}

void parseClass(FILE *f, char * token) {
    struct classDec * newClass = (struct classDec *) malloc(sizeof(struct classDec *));
    parseToken(f,token);
    char * className = (char *) malloc(strlen(token) + 1); 
    strcpy(className,token);

    newClass->name = className;
    newClass->vars = NULL;
    newClass->subs = NULL;
    char c;
    c= parseToken(f,token);
    if (c != '{')
        printf("error\n");

    int i = 0;
    while ((c = parseToken(f,token)) != '}') {
        if (strcmp(token,"static") == 0 || strcmp(token,"field") == 0) {
            newClass->vars = (struct classVar **) realloc(newClass->vars,sizeof(newClass->vars) + sizeof(struct classVar *));
        } 
    }

    return;
}



