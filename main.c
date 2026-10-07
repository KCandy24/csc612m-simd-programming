#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>

// extern void asmFuncMaxSPArray (int n, float* spArray0, float* spArray1, float* maxSPArray, bool* idxArray)
// extern void avxFuncMaxSPArray (int n, float* spArray0, float* spArray1, float* maxSPArray, bool* idxArray)
// extern void avx2FuncMaxSPArray(int n, float* spArray0, float* spArray1, float* maxSPArray, bool* idxArray)

void funcMaxSPArray(int n, float* spArray0, float* spArray1, float* maxSPArray, bool* idxArray){
    bool index;
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
    bool  *idxArray     = malloc(n * sizeof idxArray);
    
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
    
    printSPArray(limit, spArray0, "Array 0");
    printSPArray(limit, spArray1, "Array 1");
    printSPArray(limit, maxSPArray, "Max Array");

    free(spArray0);
    free(spArray1);
    free(maxSPArray);
    free(idxArray);
}