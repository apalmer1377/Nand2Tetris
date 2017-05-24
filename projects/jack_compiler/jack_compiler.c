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
            fullname[0] = '\0';
            strcpy(fullname,argv[1]);
        }
        free(fullname);
        free(argdir);
        free(rfile);
        return 0;
    }

    printf("%s\n",fname);
    compileFile(fname);
    //free(argdir);
    //free(rfile);

    return 0;

}

void compileFile(char * fname) {
    FILE * f = fopen(fname,"r");
    if (f == NULL)
        exit(1);

    char * cname = strip_extension(fname);
    char * token = (char *) malloc(MAX_LEN);
    char * vm = (char *) malloc(MAX_LEN);
    vm[0] = '\0';
    char c;
    int i = 0;
    struct classDec ** newClass = (struct classDec **) malloc(10*sizeof(struct classDec *));
    while ((c = parseToken(f , token)) != EOF) {

        if (strcmp(token,"class") == 0) {
            parseClass(f,token,newClass+i);
            newClass[i]->next = NULL;
            if (i > 0)
                newClass[i-1]->next = newClass[i];
            i++;
        }

    }

    fclose(f);
    
    struct tableVar ** class_scope = (struct tableVar **) malloc(709 * sizeof(struct tableVar *));
    struct tableVar ** sub_scope = (struct tableVar **) malloc(709 * sizeof(struct tableVar *));


    struct classDec * temp = *newClass;
    while (temp != NULL) {
        compileClass(vm,temp,cname,class_scope,sub_scope);
        printf("next\n");
        temp = temp->next;
    }

    printf("hereee\n");
    printf("%s\n",vm);
    printf("here\t%s\n",cname);

    char * wname = (char *) malloc(strlen(cname) + 4);
    strcpy(wname,cname);
    strcat(wname,".vm");

    FILE * w = fopen(wname,"w");
    fputs(vm,w);
    fclose(w);


    free(class_scope);
    free(sub_scope);
}

void compileClass(char * vm, struct classDec * class, char * classname, struct tableVar ** class_scope, struct tableVar ** sub_scope) {
    int d = 1;

    int j;
    for (j=0;j < 709; j++) {
        if (class_scope[j] != NULL)
            class_scope[j] = NULL;
    }

    class_scope[0] = (struct tableVar *) malloc(sizeof(struct tableVar));
    *(class_scope[0]) = (struct tableVar) {classname,"class",CLASSNAME,0,0};

    struct var * temp = class->vars;
    long h;
    int s = 0,f = 0;
    while (temp != NULL) {
        h = hash(temp->name, 1);
        if (strcmp(temp->vtype,"field") == 0) {
            insert_hash(class_scope, temp, FIELD, 1, f++);
        } else {
            insert_hash(class_scope, temp, STATIC, 1, s++);
        }
        temp = temp->next;
    }

    struct subDec * sub = class->subs;
    while (sub != NULL) {
        insert_func(class_scope,sub);
        sub = sub->next;
    }

    sub = class->subs;
    while (sub != NULL) {
        printf("sub\n");
        compileSub(vm, sub, class_scope,&d,f,sub_scope);
        printSub(sub);
        sub = sub->next;
    }

    printf("done\n");

}

void compileSub(char * vm, struct subDec * sub, struct tableVar **class_scope,int * d, int c, struct tableVar ** sub_scope) {
    int j;
    for (j=0;j < 709; j++) {
        if (sub_scope[j] != NULL)
            sub_scope[j] = NULL;
    }

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

    strcat(vm,"function ");
    strcat(vm,class_scope[0]->name);
    strcat(vm,".");
    strcat(vm,sub->name);
    strcat(vm," ");
    strcat(vm,itoa(i));
    strcat(vm,"\n");

    if (sub->type == CONSTRUCTOR) {
        compileConstant(vm,2);
        compileCall(vm,NULL,"Memory.alloc",1);
        compilePushPop(vm,POINTER,0,1);
    } else if (sub->type == METHOD) {
        compilePushPop(vm,ARG,0,0);
        compilePushPop(vm,POINTER,0,1);
    }

    struct command * comm = sub->comm;
    while (comm != NULL) {
        compileCommand(vm, comm, class_scope, sub_scope, d);
        comm = comm->next;
    }
}

void compileCommand(char * vm, struct command * c, struct tableVar ** class_scope, struct tableVar ** sub_scope, int * depth) {

    switch(c->type) {
        case LET:
            compileLet(vm,c->state,class_scope,sub_scope, *depth);
            break;
        case IF:
            compileIf(vm,c->state,class_scope,sub_scope, depth);
            printIf(c->state);

            break;
        case WHILE:
            compileWhile(vm,c->state,class_scope,sub_scope, depth);
            break;
        case DO:
            compileDo(vm,c->state,class_scope,sub_scope, *depth);
            printDo(c->state);
            break;
        case RETURN:
            printReturn(c->state);
            compileReturn(vm,c->state,class_scope,sub_scope, *depth);
            printf("done return\n");
            break;
    }

}

void compileLet(char * vm, struct letStatement * state, struct tableVar ** class_scope, struct tableVar ** sub_scope, int depth) {
    int loc;
    struct tableVar * tvar;

    loc = find_hash(sub_scope, state->varName, 1, 0);
    if (loc < 0) {
        loc = find_hash(class_scope, state->varName, 1, 0);

        if (loc < 0) {

            printf("ERROR LET\n");
            return;
        }

        tvar = class_scope[loc];

    } else {
        tvar = sub_scope[loc];
    }

    compileExpression(vm, state->fro, class_scope, sub_scope,0);

    if (state->to != NULL) {
        compileExpression(vm,state->to,class_scope,sub_scope,0);
        compilePushPop(vm,tvar->type,tvar->index,0);
        compileOp(vm,'+',0);
        compilePushPop(vm,POINTER,1,1);
        compilePushPop(vm,THAT,0,1);

    } else {
        compilePushPop(vm,tvar->type,tvar->index,1);
    }
}

void compileIf(char * vm, struct ifStatement * state, struct tableVar ** class_scope, struct tableVar ** sub_scope, int * depth) {
    char * a = itoa(*depth);
    compileExpression(vm,state->cond,class_scope,sub_scope,0);
    printf("IF\n");
    strcat(vm,"if-goto IF_OK");
    strcat(vm,a);
    strcat(vm,"\n");

    strcat(vm,"goto IF_BAD");
    strcat(vm,a);
    strcat(vm,"\n");

    strcat(vm,"label IF_OK");
    strcat(vm,a);
    strcat(vm,"\n");

    *depth += 1;
    struct command * temp = state->head;
    while (temp != NULL) {
        compileCommand(vm,temp,class_scope,sub_scope,depth);
        temp = temp->next;
    }

    strcat(vm,"goto END_IF");
    strcat(vm,a);
    strcat(vm,"\n");

    strcat(vm,"label IF_BAD");
    strcat(vm,a);
    strcat(vm,"\n");

    if (state->elseHead != NULL) {
        temp = state->elseHead;
        while (temp != NULL) {
            compileCommand(vm,temp,class_scope,sub_scope,depth);
            temp = temp->next;
        }
    }

    printf("ENDIF\n");
    strcat(vm,"label END_IF");
    strcat(vm,a);
    strcat(vm,"\n");

}

void compileWhile(char * vm, struct whileStatement * state, struct tableVar ** class_scope, struct tableVar ** sub_scope, int * depth) {

    char * a = itoa(*depth);
    strcat(vm,"label WHILE_START");
    strcat(vm,a);
    strcat(vm,"\n");

    compileExpression(vm,state->cond,class_scope,sub_scope,0);
    compileOp(vm,'~',1);

    strcat(vm,"if-goto WHILE_END");
    strcat(vm,a);
    strcat(vm,"\n");

    *depth += 1;
    struct command * temp = state->commands;
    while (temp != NULL) {
        compileCommand(vm,temp,class_scope,sub_scope,depth);
        temp = temp->next;
    }

    strcat(vm,"goto WHILE_START");
    strcat(vm,a);
    strcat(vm,"\nlabel WHILE_END");
    strcat(vm,a);
    strcat(vm,"\n");

}

void compileDo(char * vm, struct doStatement * state, struct tableVar ** class_scope, struct tableVar ** sub_scope, int depth) {
    state->sub->unaryOp = 'D';
    compileTerm(vm,state->sub,class_scope,sub_scope,0);
    printf("DOOO\n");

    compilePushPop(vm,TEMP,0,1);
}

void compileReturn(char * vm, struct returnStatement * state, struct tableVar ** class_scope, struct tableVar ** sub_scope, int depth) {
    if (state->ret != NULL) {
        compileExpression(vm,state->ret,class_scope,sub_scope,0);
    } else
        compileConstant(vm,0);
    strcat(vm,"return\n");
}

void compileExpression(char * vm, struct expression * exp, struct tableVar ** class_scope, struct tableVar ** sub_scope, int isPop) {

    compileTerm(vm, exp->t,class_scope,sub_scope,0);
    printf("doneterm\n");
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

    int i = 0;

    struct tableVar * tvar;// = (struct tableVar *) malloc(sizeof(struct tableVar));
    if (t->value != NULL) {
        printf("TERRRRM\t%s\n",t->value);
    }
    switch(t->type) {
        case INT:
            compileConstant(vm,t->intValue);
            break;
        case STRING:
            compileString(vm,t->value);
            break;
        case SUB:
        case EXP:
            printf("MERRRRRRRR\n");
            if (t->exValue != NULL) {
                i++;
                struct expression * temp = (t->exValue);
                printf("flerr\n");

                compileExpression(vm,temp,class_scope,sub_scope,isPop);
                printf("flerr\n");
                temp = temp->next;
                while (temp != NULL) {
                    compileExpression(vm,temp,class_scope,sub_scope,isPop);
                    temp = temp->next;
                    i++;
                }
            }
            if (t->type == SUB) {
                printf("SUBBBBBB\n");
                int j = i;
                char * n = strip_extension(t->value); 

                printf("%s\n",n);

                if (strcmp(n,t->value) == 0) {

                    i = find_hash(class_scope,n,1,0);
                    tvar = class_scope[i];
                    if (strcmp(tvar->dtype,"method") == 0) {
                        compilePushPop(vm,POINTER,0,0);
                        j++;
                    }
                    compileCall(vm,(class_scope[0])->name,t->value,j);
                    break;
                }

                i = find_hash(sub_scope, n, 1, 0);
                if (i < 0) {

                    i = find_hash(class_scope, n, 1, 0);

                    if (i < 0) {

                        compileCall(vm,(class_scope[0])->name,t->value,j);
                        break;

                    }

                    tvar = class_scope[i];


                } else {

                    tvar = sub_scope[i];

                }

                char * name = (char *) malloc(strlen(tvar->dtype) + strlen(get_filename_ext(t->value) + 2));
                strcpy(name,tvar->dtype);
                strcat(name,".");
                strcat(name,get_filename_ext(t->value));
               
                compilePushPop(vm,tvar->type,tvar->index,0);
                compileCall(vm,(class_scope[0])->name,name,j+1);
            }
            break;
        case VAR:
            if (isKeywordConstant(t->value)) {
                printf("mekeyword?\n");
                compileKeyword(vm,t->value);
                break;
            }
            i = find_hash(sub_scope, t->value, 1, 0);
            if (i < 0) {
                i = find_hash(class_scope, t->value, 1, 0);

                if (i < 0) {
                    printf("ERROR TERM %i\t%s\n",hash(t->value,1),t->value);
                    printf("%s\n",(class_scope[0])->name);
                    break;
                }

                tvar = class_scope[i];

            } else {
                tvar = sub_scope[i];
            }

            if (t->exValue != NULL) {
                compileExpression(vm,t->exValue,class_scope,sub_scope,0);
                compilePushPop(vm,tvar->type,tvar->index,0);
                compileOp(vm,'+',0);
                compilePushPop(vm,POINTER,1,1);
                compilePushPop(vm,THAT,0,isPop);
                break;
            }
            compilePushPop(vm,tvar->type,tvar->index,isPop);
            break;
        default:
            printf("WHAT DA HELLLLL\n");
            break;
    }

    if (t->unaryOp && (t->unaryOp != 'D')) {
        compileOp(vm,t->unaryOp,1);
    }

    //free(tvar);
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

    if (i < 0) {
        compilePushPop(vm, CONSTANT, -i, 0);
        compileOp(vm,'-',1);
        printf("constant\n");
        return;
    }
    compilePushPop(vm, CONSTANT, i, 0);
    printf("constant\n");
}

void compileString(char * vm, char * str) {
    compileConstant(vm,strlen(str));
    compileCall(vm,NULL,"String.new",1);
    int i;
    for (i=0;i<strlen(str);i++) {
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

void compileKeyword(char * vm, char * keyword) {
    if (strcmp(keyword,"true") == 0)
        compileConstant(vm,-1);
    else if (strcmp(keyword,"null") == 0 || strcmp(keyword,"false") == 0)
        compileConstant(vm,0);
    else {
        compilePushPop(vm,POINTER,0,0);
    }
    printf("keyword\n");
}
