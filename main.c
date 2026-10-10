

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
//#include <time.h>
#include <windows.h>

#define RUNS 30

extern void asmFuncMaxSPArray (int n, float* spArray0, float* spArray1, float* maxSPArray, int* idxArray);
extern void avxFuncMaxSPArray (int n, float* spArray0, float* spArray1, float* maxSPArray, int* idxArray);
extern void avx2FuncMaxSPArray(int n, float* spArray0, float* spArray1, float* maxSPArray, int* idxArray);

// C version
void funcMaxSPArray(int n, float* spArray0, float* spArray1, float* maxSPArray, int* idxArray)
{
    int index;
    for (int i = 0; i < n; i++)
    {
        idxArray[i] = (spArray0[i] >= spArray1[i] ? 0 : 1);
        maxSPArray[i] = (!idxArray[i] ? spArray0[i] : spArray1[i]);
    }
}

void printSPArray (int n, float* spArray, char* arrayName)
{
    printf("%s: ", arrayName);
    for (int i = 0; i < n; i++)
    {
        printf("%.2f ", spArray[i]);
    }
    printf("\n");
}

//we use QueryPerformanceCounter para di na the clock()
double getTimeMs() 
{
    static LARGE_INTEGER frequency;
    LARGE_INTEGER counter;
    if (frequency.QuadPart == 0) QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart * 1000.0 / (double)frequency.QuadPart;
}

//for checking 
int checkOutput(int n, float* maxSPArrayC, int* idxArrayC, float* maxSPArray, int* idxArray, char* kernelName)
{
    for (int i = 0; i < n; i++)
    {
        if (maxSPArray[i] != maxSPArrayC[i] || idxArray[i] != idxArrayC[i])
        {
            printf("%s output is not same (first mismatch at i = %d)\n", kernelName, i);
            return 0;
        }
    }
    printf("%s output is same\n", kernelName);
    return 1;
}


int main()
{
    unsigned int seed = 20;
    srand(seed);

    int powers[] = {20, 26, 28}; // 2^20, 2^26, 2^30
    int limit = 5;

    for (int p = 0; p < 3; p++)
    {
        int n = (1 << powers[p]);
        printf("\n=== n = 2^%d = %d ===\n", powers[p], n);

        float *spArray0     = malloc((size_t)n * sizeof *spArray0);
        float *spArray1     = malloc((size_t)n * sizeof *spArray1);

        float *maxSPArrayC  = malloc((size_t)n * sizeof *maxSPArrayC);  //C array for checking purposes
        int   *idxArrayC    = malloc((size_t)n * sizeof *idxArrayC);

        float *maxSPArray   = malloc((size_t)n * sizeof *maxSPArray);   //ASM arrays
        int   *idxArray     = malloc((size_t)n * sizeof *idxArray);

        if (spArray0 == NULL || spArray1 == NULL || maxSPArrayC == NULL || idxArrayC == NULL || maxSPArray == NULL || idxArray == NULL)
        {
            printf("  allocation failed, skipping this size\n");
            free(spArray0);
            free(spArray1);
            free(maxSPArrayC);
            free(idxArrayC);
            free(maxSPArray);
            free(idxArray);
            continue;
        }

        srand(seed);
        for (int i = 0; i < n; i++)
        {
            spArray0[i] = (float)rand() / (float)(RAND_MAX) * 10;
            spArray1[i] = (float)rand() / (float)(RAND_MAX) * 10;
        }

        double start, total;

        //C PART
        total = 0.0;
        for (int i = 0; i < RUNS; i++)
        {
            start = getTimeMs();
            funcMaxSPArray(n, spArray0, spArray1, maxSPArrayC, idxArrayC);
            total += getTimeMs() - start;
        }
        printf("C version average time: %.4f ms\n", total / RUNS);
        
        printf("\nDone C Version.");
        printf("Starting ASM Version...\n");

        //Non-SIMD x86-64 
        for (int i = 0; i < n; i++)
        {
            maxSPArray[i] = -1.0f;
            idxArray[i] = -1;
        } // so skipped elements get caught
        total = 0.0;
        for (int i = 0; i < RUNS; i++)
        {
            start = getTimeMs();
            asmFuncMaxSPArray(n, spArray0, spArray1, maxSPArray, idxArray);
            total += getTimeMs() - start;
        }
        printf("x86-64 version average time: %.4f ms\n", total / RUNS);
        checkOutput(n, maxSPArrayC, idxArrayC, maxSPArray, idxArray, "x86-64");

        printf("\nDone ASM Version.");
        printf("Starting AVX Version...\n");
        
        //SIMD XMM 
        for (int i = 0; i < n; i++)
        {
            maxSPArray[i] = -1.0f;
            idxArray[i] = -1;
        } 
        total = 0.0;
        for (int i = 0; i < RUNS; i++)
        {
            start = getTimeMs();
            avxFuncMaxSPArray(n, spArray0, spArray1, maxSPArray, idxArray);
            total += getTimeMs() - start;
        }
        printf("SIMD XMM version average time: %.4f ms\n", total / RUNS);
        checkOutput(n, maxSPArrayC, idxArrayC, maxSPArray, idxArray, "SIMD XMM");

        printf("\nDone SIMD XMM Version.");
        printf("Starting SIMD YMM Version...\n");

        //SIMD YMM
        for (int i = 0; i < n; i++)
        { 
            maxSPArray[i] = -1.0f; 
            idxArray[i] = -1; 
        }
        total = 0.0;
        for (int i = 0; i < RUNS; i++)
        {
            start = getTimeMs();
            avx2FuncMaxSPArray(n, spArray0, spArray1, maxSPArray, idxArray);
            total += getTimeMs() - start;
        }
        printf("SIMD YMM version average time: %.4f ms\n", total / RUNS);
        checkOutput(n, maxSPArrayC, idxArrayC, maxSPArray, idxArray, "SIMD YMM");

        printf("\nDone SIMD YMM Version.");

        free(spArray0);
        free(spArray1);
        free(maxSPArrayC);
        free(idxArrayC);
        free(maxSPArray);
        free(idxArray);
    }
}

