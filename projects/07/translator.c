#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

char *get_filename_ext(char*);
void parsefile(char*,char*);
void parseline(char*,char*,int*);
int get_next_word(char*,char*,int*);
void parsearith(char*,char*,int*);
void parsepush(char*,char*,char*);
char *ident(char*,int*);

int main(int argc, char** argv) {

    struct dirent *rfile;
    DIR *argdir;
    char *OUT = (char*) malloc(10000*sizeof(char));

    char *ext = get_filename_ext(argv[1]);
    if (ext == "") {
        DIR *argdir = opendir(argv[1]);
        while ((rfile = readdir(argdir)) != NULL) {
            ext = get_filename_ext(rfile->d_name);
            if (ext == "vm") { 
                parsefile(rfile->d_name,OUT);
            }
        }
    } else {
        parsefile(argv[1],OUT);
    }
    strcat(OUT,"(END)\n@END\n0;JMP");
    OUT[strlen(OUT)] = '\0';
    FILE *fwrite = fopen(argv[2],"w");
    fputs(OUT,fwrite);
    printf("%s\n",OUT);
    free(OUT);
    return 0;

}

char *get_filename_ext(char *filename) {
    char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

void parsefile(char* rfile,char* out) {
    FILE *rf = fopen(rfile,"r");
    char str[100];
    int jj[1];
    jj[0] = 0;
    printf("%i\n",jj[0]);
    int i;
    while (fgets(str,100,rf) != NULL) {
        parseline(str,out,jj);
    }
}

void parseline(char* line, char* out,int* jj) {
    if ((line[0] == '/' && line[1] == '/') || line[0] == '\n' || line[0] == ' ' || line[0] == '\0' || line[0] <= 13) {
        return;
    }
    int i[0];
    *i = 0;
    jj[0] += 1;
    printf("%i\n",*jj);
    char *temp = (char*) malloc((strlen(line)+1)*sizeof(char));
    get_next_word(line,temp,i);
    char *command = (char*) malloc((strlen(temp)+1)*sizeof(char));
    strcpy(command,temp);
    if (strcmp(command,"push") != 0 &&  strcmp(command,"pop") != 0){
        parsearith(command,out,jj);
    } else {
        get_next_word(line,temp,i);
        char * segment = (char*) malloc((strlen(temp)+1)*sizeof(char));
        strcpy(segment,temp);
        get_next_word(line,temp,i);
        char *index = temp;
        if (strcmp(command,"push") == 0)
            parsepush(segment,index,out);

        free(segment);
    }
    free(command);
    free(temp);
    return;
}

int get_next_word(char* line,char *buff,int* start) {
    int b = 0;
    while (line[*start] != ' ' && line[*start] != '\0' && line[*start] != '\n' && line[*start] > 13 && line[*start] != 133) {
        buff[b] = line[*start];
        (*start)++;
        b++;
    }
    buff[b] = '\0';
    (*start)++;
    return *start;
}

void parsearith(char *oper, char *out,int* jumpc) {
    char *str = (char*) malloc(1000*sizeof(char));
    str[0] = '\0';
    strcat(str,"@SP\nM=M-1\nA=M\nD=M\n@SP\n");

    if (strcmp(oper, "add") == 0){
        strcat(str,"M=M-1\nA=M\nD=M+D\n");
    } else if (strcmp(oper, "sub") == 0)
        strcat(str,"M=M-1\nA=M\nD=M-D\n");
    else if (strcmp(oper, "neg") == 0)
        strcat(str,"D=-D\n");
    else if (strcmp(oper, "and") == 0)
        strcat(str,"M=M-1\nA=M\nD=D&M\n");
    else if (strcmp(oper, "or") == 0)
        strcat(str,"M=M-1\nA=M\nD=D|M\n");
    else if (strcmp(oper,"not") == 0)
        strcat(str,"D=!D\n"); else {
        strcat(str,"D=D\n@START");
        //printf("%i\n",*jumpc);
        char * id = (char*) malloc(4*sizeof(char));
        ident(id,jumpc);
        char *tr = (char*) malloc(25*sizeof(char));
        char *fa = (char*) malloc(25*sizeof(char));
        int k;
        tr[0] = '\0';
        fa[0] = '\0';

        strcat(str,id);
        strcat(str,"\n0;JMP\n");

        strcat(tr,"(TRUE");
        strcat(tr,id);
        strcat(tr,")\n");

        strcat(fa,"(FALSE");
        strcat(fa,id);
        strcat(fa,")\n");

        printf("%s%s\n",tr,fa);


        strcat(str,tr);
        strcat(str,"D=-1\n@CONT");
        strcat(str,id);
        strcat(str,"\n0;JMP\n");
        strcat(str,fa);
        strcat(str,"D=0\n@CONT");
        strcat(str,id);
        strcat(str,"\n0;JMP\n");

        strcat(str,"(START");
        strcat(str,id);
        strcat(str,")\n");

        strcat(str,"@SP\nM=M-1\nA=M\nD=M-D\n@TRUE");
        strcat(str,id);
        strcat(str,"\nD;");

        if (strcmp(oper,"eq") == 0) 
            strcat(str,"JEQ");
        else if (strcmp(oper,"gt") == 0)
            strcat(str,"JGT");
        else if (strcmp(oper,"lt") == 0)
            strcat(str,"JLT");
        strcat(str,"\n@FALSE");
        strcat(str,id);
        strcat(str,"\n0;JMP\n(CONT");
        strcat(str,id);
        strcat(str,")\n");

        free(id);
        free(tr);
        free(fa);

    }
    strcat(str,"@SP\nA=M\nM=D\n@SP\nM=M+1\n");
    strcat(out,str);
    free(str);
}

void parsepush(char *segment, char *index,char *out) {
    char *str = (char*) malloc(100*sizeof(char));
    if (strcmp(segment,"constant") == 0) {
        strcat(str,"@");
        strcat(str,index);
        strcat(str,"\n");
        strcat(str,"D=A\n");
        strcat(str,"@SP\n");
        strcat(str,"A=M\nM=D\n");
        strcat(str,"@SP\nM=M+1\n");
    }
    strcat(out,str);
    free(str);
}

char * ident(char* buff,int *i) {
    int temp = *i;
    int j;
    for (j=0;j<3;j++) {
        if (temp > 0) {
            buff[j] = (temp % 10) + '0';
            temp /= 10;
        } else
            buff[j] = 'z';
    }
    buff[3] = '\0';
}
