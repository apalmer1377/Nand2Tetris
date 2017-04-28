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

    compileFile(fname);
    free(argdir);
    free(rfile);

    return 0;

}

void compileFile(char * fname) {

}
