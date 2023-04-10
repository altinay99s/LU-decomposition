#include <time.h>
#include "lss_20_08.h"

// lss_20_08_in.txt lss_20_08_out.txt
// gcc -x c lss_20_08.c main_lss_20_08.c
// ./a.exe
// cmake-build-debug/FIN.TXT cmake-build-debug/FOUT.TXT


int Input(int *n, double **A, double **B, FILE *fin) {

    fseek(fin, 0, SEEK_END);
    long pos = ftell(fin);//кол-во байт между началом и указателем
    if (pos <= 0) {
        if (command_error == 1)
            fprintf(stderr, "Input file is empty \n");
        fclose(fin);
        exit(-4);
    }
    fseek(fin, 0, SEEK_SET);

    if (!fscanf(fin, "%d", n)) {
        if (command_error == 1)
            fprintf(stderr, "Invalid values specified in input file \n");
        fclose(fin);
        exit(-5);
    }

    if ((*n) <= 0) {
        if (command_error == 1)
            fprintf(stderr, "Invalid values specified in input file \n");
        fclose(fin);
        exit(-5);
    }

    (*A) = (double *) malloc((*n) * (*n) * sizeof(double));
    (*B) = (double *) malloc(*n * sizeof(double));

    for (int i = 0; i < (*n); i++)
        for (int j = 0; j < (*n); j++)
            if (!fscanf(fin, "%lf", &(*A)[i * (*n) + j])) {
                if (command_error == 1)
                    fprintf(stderr, "Invalid values specified in input file \n");
                fclose(fin);
                exit(-5);
            }
    for (int i = 0; i < (*n); i++)
        if (!fscanf(fin, "%lf", &(*B)[i])) {
            if (command_error == 1)
                fprintf(stderr, "Invalid values specified in input file \n");
            fclose(fin);
            exit(-5);
        }
    fclose(fin);
    if (command_debug == 1)
        printf("File was read \n");


    return 0;
}

int OutPut(int n, double *X, FILE *fout, int code) {
    if (code == 0) {
        fprintf(fout, "%d\n", n);
        for (int i = 0; i < n; i++)
            fprintf(fout, "%1.9lf\n", X[i]);
    } else if(code==1) fprintf(fout, "%d", 0);

    if (command_debug == 1)
        printf("File was write \n");

    return 0;
}

int main(int argc, char *argv[]) {

    int command_print_matrix = 0;
    int command_execution_time = 0;
    command_debug = 0;
    command_error = 0;
    int start_time = clock();
    FILE *fin, *fout;
    int count_files = 0;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'd')
                command_debug = 1;
            else if (argv[i][1] == 'e')
                command_error = 1;
            else if (argv[i][1] == 'p')
                command_print_matrix = 1;
            else if (argv[i][1] == 't')
                command_execution_time = 1;
            else if (argv[i][1] == 'h' || argv[i][1] == '?')
                printf("/home/ivk/lss/lss\n"
                       "Usage: lss [input_file_name] [output_file_name] [options]\n"
                       "Where options include:\n"
                       "  -d    print debug messages [default OFF]\n"
                       "  -e    print errors [default OFF]\n"
                       "  -p    print matrix [default OFF]\n"
                       "  -t    print execution time [default OFF]\n"
                       "  -h, -?     print this and exit\n"
                       "Default input_file_name value is lss_00_00_in.txt, default output_file_name value is lss_00_00_out.txt.\n\n");
            else {
                if(command_error==1)
                    fprintf(stderr,"No such command");
                exit(-3);
            }
        } else {
            if (i == 1) {
                fin = fopen(argv[i], "r");
                count_files += 1;
            }
            else if (i == 2) {
                fout = fopen(argv[i], "w");
                count_files += 1;
            } else {
                if(command_error==1)
                    fprintf(stderr,"No such command");
                exit(-3);
            }
        }
    }
    if (count_files == 0) {
        if ((fin = fopen("C:\\programs\\LU-decomposition\\lss_20_08_in.txt", "r")) == NULL) {
            fprintf(stderr, "Failed to open input file \n");
            exit(-2);
        }
        if ((fout = fopen("C:\\programs\\LU-decomposition\\lss_20_08_out.txt", "w")) == NULL) {
            fprintf(stderr, "Failed to open output file \n");
            exit(-2);
        }
//        if ((fin = fopen("lss_20_08_in.txt", "r")) == NULL) {
//            fprintf(stderr, "Failed to open input file \n");
//            exit(-2);
//        }
//        if ((fout = fopen("lss_20_08_out.txt", "w")) == NULL) {
//            fprintf(stderr, "Failed to open output file \n");
//            exit(-2);
//        }
    } else if (count_files != 2) {
        if(command_error==1)
            fprintf(stderr,"You have added 1 file");
        exit(-6);
    }

    int n;
    double *A, *B, *X, *tmp;

    Input(&n, &A, &B, fin);
    tmp = (double *) malloc(lss_memsize_20_08(n) * sizeof(double));
    X = (double *) malloc(n * sizeof(double));

    if (command_print_matrix) {
        printf("matrix A: \n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                printf("%lf ", A[n * i + j]);
            printf("\n");
        }
        printf("\n");
        printf("matrix B: \n");
        for (int i = 0; i < n; i++) {
            printf("%lf ", B[i]);
        }
        printf("\n\n");
    }

    int code_status = lss_20_08(n, A, B, X, tmp);

    free(A);
    free(B);
    free(tmp);

    if(code_status==-1) {
        int end_time = clock();
        double search_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
        if (command_execution_time == 1)
            printf("Execution time: %lf seconds", search_time);
        fclose(fout);
        free(X);
        return -1;
    }

    OutPut(n, X, fout, code_status);
    fclose(fout);
    free(X);

    int end_time = clock();
    double search_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    if (command_execution_time == 1)
        printf("Execution time: %lf seconds", search_time);

    return 0;
}


