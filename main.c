

/**************************************************************************************************
Compile commands also see batch file
    nasm -f win64 asmFuncMaxSPArray.asm -o asmFuncMaxSPArray.obj
    nasm -f win64 avxFuncMaxSPArray.asm -o avxFuncMaxSPArray.obj
    nasm -f win64 avx2FuncMaxSPArray.asm -o avx2FuncMaxSPArray.obj
    gcc main.c asmFuncMaxSPArray.obj avxFuncMaxSPArray.obj avx2FuncMaxSPArray.obj -o main.exe
    .\main.exe

    gcc main.c -o main.exe
    ./main.exe

**************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

extern void asmFuncMaxSPArray (int n, float* spArray0, float* spArray1, float* maxSPArray, int* idxArray);
extern void avxFuncMaxSPArray (int n, float* spArray0, float* spArray1, float* maxSPArray, int* idxArray);
extern void avx2FuncMaxSPArray(int n, float* spArray0, float* spArray1, float* maxSPArray, int* idxArray);

void funcMaxSPArray(int n, float* spArray0, float* spArray1, float* maxSPArray, int* idxArray){
    int index;
    for (int i = 0; i < n; i++){
        idxArray[i] = (spArray0[i] >= spArray1[i] ? 0 : 1);
        maxSPArray[i] = (!idxArray[i] ? spArray0[i] : spArray1[i]);
    }
}

void printSPArray (int n, float* spArray, char* arrayName){
    printf("%s: ", arrayName);
    for (int i = 0; i < n; i++){
        printf("%.2f ", spArray[i]);
    }
    printf("\n");
}

int main(void){
    unsigned int seed = 20;
    srand(seed);

    int n = 10;
    int limit = 5;

    float *spArray0     = malloc(n * sizeof spArray0);
    float *spArray1     = malloc(n * sizeof spArray1);
    
    float *maxSPArray   = malloc(n * sizeof maxSPArray);
    int  *idxArray     = malloc(n * sizeof idxArray);
    
    if (spArray0 == NULL || spArray1 == NULL){
        free(spArray0);
        free(spArray1);
        free(maxSPArray);
        free(idxArray);
        return EXIT_FAILURE;
    }
    
    for (int i = 0; i < n; i++){
        spArray0[i] = (float)rand() / (float)(RAND_MAX) * 10;
        spArray1[i] = (float)rand() / (float)(RAND_MAX) * 10;
    }
    
    funcMaxSPArray(n, spArray0, spArray1, maxSPArray, idxArray);
    
    // printSPArray(n, spArray0, "Array 0");
    // printSPArray(n, spArray1, "Array 1");
    // printSPArray(n, maxSPArray, "Max Array");

    asmFuncMaxSPArray(n, spArray0, spArray1, maxSPArray, idxArray);
    avxFuncMaxSPArray(n, spArray0, spArray1, maxSPArray, idxArray);
    avx2FuncMaxSPArray(n, spArray0, spArray1, maxSPArray, idxArray);


    free(spArray0);
    free(spArray1);
    free(maxSPArray);
    free(idxArray);
}
