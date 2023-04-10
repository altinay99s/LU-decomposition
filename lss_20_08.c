#include "lss_20_08.h"

size_t lss_memsize_20_08(int n) {
    return n * n + n;
}

int is_equal(double x, double y) {
    return fabs(x - y) < 1e-15 ? 0 : 1;
}

void copyMatrix(int n, double *matrix1, const double *matrix2) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matrix1[n * i + j] = matrix2[n * i + j];
}

int swapLine(int n, int j, double *matrix) {
    int is_not_null = 0;
    int index;
    double meaning;

    for (int i = j+1; i < n; i++) {
        if (is_equal(matrix[n * i + j], 0.0) != 0) {
            is_not_null = 1;
            index=i;
            break;
        }
    }
    if(is_not_null==1){
        for(int k=0;k<n;k++){
            meaning = matrix[n * j + k];
            matrix[n * j + k] = matrix[n*index+k];
            matrix[n*index+k] = meaning;
        }
        return 1;
    }
    return 0;
}

double divisionByNumber(int n, int index, double *matrix){
    double number = matrix[n*index+index];
    for(int i=index;i<n;i++)
        matrix[n*index+i]/=number;
    return number;
}

void differenceLine(int n, int index1, int index2, double *matrix){
    // index1 какую строку отнимаем
    // index2 от какой строки отнимаем
    // index1<index2

    double number = matrix[index2*n+index1];
    if(is_equal(number,0)==0)
        return;
    for(int j=index1;j<n;j++)
        matrix[n * index2 + j] -= number * matrix[n * index1 + j];

}

double triangularMatrix(int n, double *matrix) {
    double det =1;
    for (int j = 0; j < n-1; j++) {
        if (is_equal((matrix)[n * j + j], 0.0) == 0)
            if(swapLine(n,j,matrix)==0)
                return 0;

        det*=divisionByNumber(n,j,matrix);
        for(int i = j+1; i<n;i++) {
            differenceLine(n, j, i, matrix);
        }
    }
    return det*matrix[n*n-1];
}

void zeroFilling(int n, double *tmp) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            tmp[n * i + j] = 0;
}

double sumOfMultiplication(int i, int j, int n, int m, const double *tmp) {
    double sum = 0;
    for (int k = 0; k < m; k++)
        sum += tmp[n * i + k] * tmp[n * k + j];
    return sum;
}

void factorization(int n, const double *A, double *tmp) {
    zeroFilling(n, tmp);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i <= j)
                tmp[n * i + j] = A[n * i + j] - sumOfMultiplication(i, j, n, i, tmp);
            else
                tmp[n * i + j] = (A[n * i + j] - sumOfMultiplication(i, j, n, j, tmp)) / tmp[n * j + j];
        }
}

void forwardSubstitution(int n, double *tmp, const double *B) {
    tmp[n * n] = B[0];
    double sum;
    for (int i = 1; i < n; i++) {
        sum = 0;
        for (int j = 0; j < i; j++)
            sum += tmp[n * i + j] * tmp[n * n + j];
        tmp[n * n + i] = B[i] - sum;
    }
}

void backSubstitution(int n, double *X, const double *tmp) {
    X[n - 1] = tmp[(n + 1) * n - 1] / tmp[n * n - 1];
    double sum;
    for (int i = 1; i < n; i++) {
        sum = 0;
        for (int j = 0; j < i; j++)
            sum += tmp[n * n - 1 - (n * i + j)] * X[n - 1 - j];
        X[n - 1 - i] = (tmp[(n + 1) * n - 1 - i] - sum) / tmp[n * n - 1 - (n * i + i)];
    }
}

int lss_20_08(int n, double *A, double *B, double *X, double *tmp) {

    if (n == 1) {
        if (is_equal(A[0],0)==0) {
            if (command_error == 1)
                fprintf(stderr, "The method is not applicable to this system \n");
            return -1;
        }
        X[0] = B[0] / A[0];
        if (command_debug == 1)
            printf("System solution found!\n");
        return 0;
    }

    double determinant;
    if (n == 2)
        determinant = A[0] * A[3] - A[1] * A[2];
    else {
        copyMatrix(n,tmp,A);
        determinant = triangularMatrix(n, tmp);
    }


    if (command_debug == 1) {
        printf("Matrix determinant and minor was calculated \n");
        printf("determinant A = %lf \n", determinant);
    }

    if (is_equal(determinant, 0.0) == 0) {
        if (command_error == 1)
            fprintf(stderr, "The method is not applicable to this system \n");
        return -1;
    }

//    for(int i = n-1;i>1;i--) {
//        copyMatrix(n,tmp,A);
//        if (is_equal(triangularMatrix(n, tmp), 0) == 0) {
//            if (command_error == 1)
//                fprintf(stderr, "There is no LU-decomposition for this system \n");
//            return 1;
//        }
//    }

    factorization(n, A, tmp);

    if (command_debug == 1) {
        printf("The factorization operation was performed \n");
        printf("LU: \n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                printf("%lf ", tmp[n * i + j]);
            printf("\n");
        }
    }

    forwardSubstitution(n, tmp, B);
    if (command_debug == 1)
        printf("Forward substitution done!\n");


    backSubstitution(n, X, tmp);
    if (command_debug == 1) {
        printf("Back substitution done!\n");
        printf("System solution found!\n");
    }

    return 0;
}