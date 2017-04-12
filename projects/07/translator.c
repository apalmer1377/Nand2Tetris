#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

char *get_filename_ext(char*);
void parsefile(char*,char*);
void parseline(char*,char*);
int get_next_word(char*,char*,int*);
void parsearith(char*,char*);
void parsearg(int,char*);
void parselocal(int,char*);
void parsestatic(int,char*);
void parseconstant(int,char*);
void parsethis(int,char*);
void parsethat(int,char*);
void parsepointer(int,char*);
void parsetemp(int,char*);
void parsepush(char*,char*,char*);

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
    FILE *fwrite = fopen("Test.asm","w");
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
    int i;
    while (fgets(str,100,rf) != NULL) {
        parseline(str,out);
    }
}

void parseline(char* line, char* out) {
    if ((line[0] == '/' && line[1] == '/') || line[0] == '\n' || line[0] == ' ' || line[0] == '\0' || line[0] == 13) {
        return;
    }
    int j = 0;
    int *i = &j;
    char *temp = (char*) malloc(strlen(line)*sizeof(char));
    get_next_word(line,temp,i);
    char *command = (char*) malloc(strlen(temp)*sizeof(char));
    strcpy(command,temp);
    if (strcmp(command,"push") != 0 &&  strcmp(command,"pop") != 0){
        parsearith(command,out);
    } else {
        get_next_word(line,temp,i);
        char * segment = (char*) malloc(strlen(temp)*sizeof(char));
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
    while (line[*start] != ' ' && line[*start] != '\0' && line[*start] != '\n' && line[*start] != 13) {
        buff[b] = line[*start];
        (*start)++;
        b++;
    }
    buff[b] = '\0';
    (*start)++;
    return *start;
}

void parsearith(char *oper, char *out) {
    char *str = (char*) malloc(100*sizeof(char));
    strcat(str,"@SP\nM=M-1\nA=M\nD=M\n@SP\n");
    if (strcmp(oper, "add") == 0){
        strcat(str,"M=M-1\nA=M\nD=M+D\n");
    } else if (strcmp(oper, "sub") == 0)
        strcat(str,"M=M-1\nA=M\nD=M-D\n");
    else if (strcmp(oper, "neg") == 0)
        strcat(str,"D=-D\nM=D");
    else if (strcmp(oper, "eq") == 0)
        strcat(str,"M=M-1\nA=M\nD=D-M\nD;JEQ\n");
    else if (strcmp(oper, "gt") == 0)
        strcat(str,"\n");
    else if (strcmp(oper, "lt") == 0)
         strcat(str,"\n");
    else if (strcmp(oper, "and") == 0)
        strcat(str,"\n");
    else if (strcmp(oper, "or") == 0)
        strcat(str,"\n");
    else
        strcat(str,"A=!A\nM=M+1\n");
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
