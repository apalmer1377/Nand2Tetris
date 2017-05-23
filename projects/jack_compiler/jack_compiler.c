#include "jack_compiler.h"

int main(int argc, char ** argv) {
    struct dirent * rfile;
    DIR * argdir;

    char * fname = argv[1];

    char * ext = get_filename_ext(argv[1]);
    if (strlen(ext) == 0) {
        argdir = opendir(argv[1]);
        char * fullname = (char *) malloc(strlen(argv[1]) + 51);
        strcpy(fullname,argv[1]);
        while ((rfile = readdir(argdir)) != NULL) {
            if (strcmp(get_filename_ext(rfile->d_name),"jack") == 0) {
                strcat(fullname,rfile->d_name);
                compileFile(fullname);
            }
            strcpy(fullname,argv[1]);
        }
        free(fullname);
        free(argdir);
        free(rfile);
        return 0;
    }

    printf("%s\n",fname);
    compileFile(fname);
    free(argdir);
    free(rfile);

    return 0;

}

void compileFile(char * fname) {
    FILE * f = fopen(fname,"r");
    if (f == NULL)
        exit(1);

    char * cname = get_short_fname(fname);
    char * vm = (char *) malloc(MAX_LEN);
    char c;
    int i = 0;
    struct classDec ** newClass = (struct classDec **) malloc(10*sizeof(struct classDec *));
    while ((c = parseToken(f , vm)) != EOF) {

        if (strcmp(vm,"class") == 0) {
            parseClass(f,vm,newClass+i);
            newClass[i]->next = NULL;
            if (i > 0)
                newClass[i-1]->next = newClass[i];
            i++;
        }

    }

    struct classDec * temp = *newClass;
    while (temp != NULL) {
        compileClass(vm,temp,cname);
        temp = temp->next;
    }
    free(temp);

    fclose(f);


}

void compileClass(char * vm, struct classDec * class, char * classname) {
    int d = 1;
    struct tableVar ** class_scope = (struct tableVar **) malloc(709 * sizeof(struct tableVar *));
    class_scope[0] = (struct tableVar *) malloc(sizeof(struct tableVar));
    *(class_scope[0]) = (struct tableVar) {classname,"class",CLASSNAME,0,0};

    printf("here\n");
    struct var * temp = class->vars;
    long h;
    int s = 0,f = 0;
    while (temp != NULL) {
        h = hash(temp->name, 1);
        if (strcmp(temp->vtype,"field") == 0) {
            insert_hash(class_scope, temp, FIELD, 1, f++);
        } else {
            insert_hash(class_scope, temp, FIELD, 1, s++);
        }
        temp = temp->next;
    }

    struct subDec * sub = class->subs;
    while (sub != NULL) {
        compileSub(vm, sub, class_scope,&d);
        sub = sub->next;
    }
}

void compileSub(char * vm, struct subDec * sub, struct tableVar **class_scope,int * d) {
    struct tableVar ** sub_scope = (struct tableVar **) malloc(709 * sizeof(struct tableVar *));

    struct var * temp = sub->paramList;
    int i = 0;
    while (temp != NULL) {
        insert_hash(sub_scope, temp, ARG, 1, i++); 
        temp = temp->next;
    }

    i = 0;
    temp = sub->decs;
    while (temp != NULL) {
        insert_hash(sub_scope, temp, LOCAL, 1, i++);
        temp = temp->next;
    }

    struct command * comm = sub->comm;
    while (comm != NULL) {
        compileCommand(vm, comm, class_scope, sub_scope, *d);
        comm = comm->next;
    }

}

void compileCommand(char * vm, struct command * c, struct tableVar ** class_scope, struct tableVar ** sub_scope, int depth) {

    printf("command\n");

    switch(c->type) {
        case LET:
            compileLet(vm,c->state,class_scope,sub_scope, depth);
            break;
        case IF:
            compileIf(vm,c->state,class_scope,sub_scope, &depth);
            break;
        case WHILE:
            compileWhile(vm,c->state,class_scope,sub_scope, &depth);
            break;
        case DO:
            compileDo(vm,c->state,class_scope,sub_scope, depth);
            break;
        case RETURN:
            compileReturn(vm,c->state,class_scope,sub_scope, depth);
            break;
    }

}

void compileLet(char * vm, struct letStatement * state, struct tableVar ** class_scope, struct tableVar ** sub_scope, int depth) {
    int loc;
    struct tableVar * tvar;

    printf("%s\t%i\t%i\t%i\n",state->varName,strlen(state->varName),depth,hash(state->varName,depth));
    loc = find_hash(sub_scope, state->varName, 1, 0);
    if (loc < 0) {
        loc = find_hash(class_scope, state->varName, 1, 0);

        if (loc < 0) {
            printf("ERROR\n");
            return;
        }

        tvar = class_scope[loc];

    } else {
        tvar = sub_scope[loc];
    }

    printf("let\n");

    compileExpression(vm, state->fro, class_scope, sub_scope,0);
    if (state->to != NULL) {
        compileExpression(vm,state->to,class_scope,sub_scope,0);
        compilePushPop(vm,POINTER,1,1);
        compilePushPop(vm,tvar->type,tvar->index,0);
        compilePushPop(vm,THAT,0,1);

        return;
    }

    compilePushPop(vm,tvar->type,tvar->index,1);
}

void compileIf(char * vm, struct ifStatement * state, struct tableVar ** class_scope, struct tableVar ** sub_scope, int * depth) {
    char * a = itoa(*depth);
    compileExpression(vm,state->cond,class_scope,sub_scope,0);
    compileOp(vm,'~',1);
    strcat(vm,"if-goto IF_OK");
    strcat(vm,a);
    strcat(vm,"\n");

    struct command * temp = state->head;
    while (temp != NULL) {
        compileCommand(vm,temp,class_scope,sub_scope,*depth);
        temp = temp->next;
    }

    strcat(vm,"goto END_IF");
    strcat(vm,a);
    strcat(vm,"\nlabel IF_OK");
    strcat(vm,a);
    strcat(vm,"\n");

    temp = state->elseHead;
    while (temp != NULL) {
        compileCommand(vm,temp,class_scope,sub_scope,*depth);
        temp = temp->next;
    }

    strcat(vm,"label END_IF");
    strcat(vm,a);
    strcat(vm,"\n");

    *depth += 1;
}

void compileWhile(char * vm, struct whileStatement * state, struct tableVar ** class_scope, struct tableVar ** sub_scope, int * depth) {
    char * a = itoa(*depth);
    strcat(vm,"label WHILE_START");
    strcat(vm,a);
    strcat(vm,"\n");

    compileExpression(vm,state->cond,class_scope,sub_scope,0);

    strcat(vm,"if-goto WHILE_END");
    strcat(vm,a);
    strcat(vm,"\n");

    struct command * temp = state->commands;
    while (temp != NULL) {
        compileCommand(vm,temp,class_scope,sub_scope,*depth);
        temp = temp->next;
    }

    strcat(vm,"goto WHILE_START");
    strcat(vm,a);
    strcat(vm,"\nlabel WHILE_END");
    strcat(vm,a);
    strcat(vm,"\n");

    *depth += 1;
}

void compileDo(char * vm, struct doStatement * state, struct tableVar ** class_scope, struct tableVar ** sub_scope, int depth) {
    state->sub->unaryOp = 'D';
    compileTerm(vm,state->sub,class_scope,sub_scope,0);
}

void compileReturn(char * vm, struct returnStatement * state, struct tableVar ** class_scope, struct tableVar ** sub_scope, int depth) {
    compileExpression(vm,state->ret,class_scope,sub_scope,0);
    strcat(vm,"return\n");
}

void compileExpression(char * vm, struct expression * exp, struct tableVar ** class_scope, struct tableVar ** sub_scope, int isPop) {

    printf("expression\n");

    compileTerm(vm, exp->t,class_scope,sub_scope,0);
    if (exp->op != NULL)
        compileOpTerm(vm,exp->op,class_scope,sub_scope,0);

}

void compileOpTerm(char * vm, struct opterm * op, struct tableVar ** class_scope, struct tableVar ** sub_scope,int isPop) {

    compileTerm(vm, op->t,class_scope,sub_scope,isPop);
    if (op->next != NULL)
        compileOpTerm(vm,op->next,class_scope,sub_scope,isPop);

    compileOp(vm, op->oper, 0);

}

void compileTerm(char * vm, struct term * t, struct tableVar ** class_scope, struct tableVar ** sub_scope,int isPop) {

    printf("term %s\n",t->value);
    int i = 0;
    struct tableVar * tvar = (struct tableVar *) malloc(sizeof(struct tableVar));
    switch(t->type) {
        case INT:
            compileConstant(vm,t->intValue);
            break;
        case STRING:
            compileString(vm,t->value);
            break;
        case SUB:
        case EXP:
            if (t->exValue != NULL) {
                i++;
                struct expression * temp = (t->exValue);
                compileExpression(vm,temp,class_scope,sub_scope,isPop);
                temp = temp->next;
                while (temp != NULL) {
                    compileExpression(vm,temp,class_scope,sub_scope,isPop);
                    temp = temp->next;
                    i++;
                }
            }
            if (t->type == SUB) 
                compileCall(vm,(class_scope[0])->name,t->value,i);
            break;
        case VAR:
            i = find_hash(sub_scope, t->value, 1, 0);
            if (i < 0) {
                i = find_hash(class_scope, t->value, 1, 0);

                if (i < 0) {
                    printf("ERROR\n");
                    return;
                }

                tvar = class_scope[i];

            } else {
                tvar = sub_scope[i];
            }

            if (t->exValue != NULL) {
                compileExpression(vm,t->exValue,class_scope,sub_scope,0);
                compilePushPop(vm,POINTER,1,1);
                compilePushPop(vm,tvar->type,tvar->index,0);
                compilePushPop(vm,THAT,0,isPop);
                break;
            }
            compilePushPop(vm,tvar->type,tvar->index,isPop);
            break;
    }

    if (t->unaryOp && (t->unaryOp != 'D')) {
        compileOp(vm,t->unaryOp,1);
    }

    free(tvar);
}

void compileOp(char * vm, char op, int isUnary) {
    switch(op) {
        case '+':
            strcat(vm,"add");
            break;
        case '-':
            if (isUnary)
                strcat(vm,"neg");
            else
                strcat(vm,"sub");
            break;
        case '=':
            strcat(vm,"eq");
            break;
        case '>':
            strcat(vm,"gt");
            break;
        case '<':
            strcat(vm,"lt");
            break;
        case '&':
            strcat(vm,"and");
            break;
        case '|':
            strcat(vm,"or");
            break;
        case '~':
            strcat(vm,"not");
            break;
        case '*':
            strcat(vm,"call Math.multiply 2");
            break;
        case '/':
            strcat(vm,"call Math.divide 2");
            break;
    }
    strcat(vm,"\n");
}

void compileConstant(char * vm, int i) {
    strcat(vm,itoa(i));
    strcat(vm,"\n");
}

void compileString(char * vm, char * str) {
    compileConstant(vm,str[0]);
    compileCall(vm,NULL,"String.new",1);
    int i;
    for (i=1;i<strlen(str);i++) {
        compileConstant(vm,str[i]);
        compileCall(vm,NULL,"String.appendChar",2);
    }
}

void compilePushPop(char * vm, enum vmType type, int index, int isPop) {
    if (isPop)
        strcat(vm,"pop ");
    else
        strcat(vm,"push ");
    switch(type) {
        case FIELD:
            break;
        default:
            strcat(vm,getVMType(type));
            break;
    }
    strcat(vm," ");
    strcat(vm,itoa(index));
    strcat(vm,"\n");
}

void compileCall(char * vm, char * classname, char * call, int args) {
    strcat(vm,"call ");
    char * dot = get_filename_ext(call);
    if (strcmp(dot,"") == 0) {
        strcat(vm,classname);
        strcat(vm,".");
    }
    strcat(vm,call);
    strcat(vm," ");
    strcat(vm,itoa(args));
    strcat(vm,"\n");
}
