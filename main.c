

/**************************************************************************************************
Compile commands also see batch file
    nasm -f win64 asmFuncMaxSPArray.asm -o asmFuncMaxSPArray.obj
    nasm -f win64 avxFuncMaxSPArray.asm -o avxFuncMaxSPArray.obj
    nasm -f win64 avx2FuncMaxSPArray.asm -o avx2FuncMaxSPArray.obj
    gcc main.c max_sp_array.c asmFuncMaxSPArray.obj avxFuncMaxSPArray.obj avx2FuncMaxSPArray.obj -o main.exe
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

// All kernel functions
extern void funcMaxSPArray      (int n, float* spArray0, float* spArray1, float* maxSPArray, int* idxArray);
extern void asmFuncMaxSPArray   (int n, float* spArray0, float* spArray1, float* maxSPArray, int* idxArray);
extern void avxFuncMaxSPArray   (int n, float* spArray0, float* spArray1, float* maxSPArray, int* idxArray);
extern void avx2FuncMaxSPArray  (int n, float* spArray0, float* spArray1, float* maxSPArray, int* idxArray);

// Function Declarations
void printResults (int n, int limit, double total, float* spArray, int* idxArray, char* version);
double getTimeMs();
int checkOutput(int n, float* maxSPArrayC, int* idxArrayC, float* maxSPArray, int* idxArray, char* kernelName);
void resetValues(int n, float* maxSPArray, int* idxArray, double* total);

int main()
{
    unsigned int seed = 20;
    srand(seed);

    int powers[] = {20, 20, 26, 28};
    int offsets[] = {0, 3, 0, 0};
    int limit = 5;
    int n;

    int twoTo28 = (1 << 28);

    // Allocation and Initialization
    float *spArray0     = malloc((size_t)twoTo28 * sizeof *spArray0);
    float *spArray1     = malloc((size_t)twoTo28 * sizeof *spArray1);

    float *maxSPArrayC  = malloc((size_t)twoTo28 * sizeof *maxSPArrayC);  //C array for checking purposes
    int   *idxArrayC    = malloc((size_t)twoTo28 * sizeof *idxArrayC);

    float *maxSPArray   = malloc((size_t)twoTo28 * sizeof *maxSPArray);   //ASM arrays
    int   *idxArray     = malloc((size_t)twoTo28 * sizeof *idxArray);

    if (spArray0 == NULL || spArray1 == NULL || maxSPArrayC == NULL || idxArrayC == NULL || maxSPArray == NULL || idxArray == NULL)
    {
        printf("Allocation failed, skipping this size\n");
        free(spArray0);
        free(spArray1);
        free(maxSPArrayC);
        free(idxArrayC);
        free(maxSPArray);
        free(idxArray);
        return 0;
    }

    srand(seed);
    n = twoTo28;
    for (int i = 0; i < n; i++)
    {
        spArray0[i] = (float)rand() / (float)(RAND_MAX) * 10;
        spArray1[i] = (float)rand() / (float)(RAND_MAX) * 10;
    }
    

    for (int p = 0; p < 4; p++)
    {
        n = (1 << powers[p]) + offsets[p];
        printf("\n=== n = 2^%d + %d = %d ===\n",
               powers[p], offsets[p], n);

        double start, total;

        //C PART
        total = 0.0;
        for (int i = 0; i < RUNS; i++)
        {
            start = getTimeMs();
            funcMaxSPArray(n, spArray0, spArray1, maxSPArrayC, idxArrayC);
            total += getTimeMs() - start;
        }
        printf("Total Execution Time: %.4f ms\n", total);
        printResults(n, limit, total, maxSPArrayC, idxArrayC, "C");


        //Non-SIMD x86-64 
        printf("\nStarting ASM Version...\n");
        resetValues(n, maxSPArray, idxArray, &total);
        for (int i = 0; i < RUNS; i++)
        {
            start = getTimeMs();
            asmFuncMaxSPArray(n, spArray0, spArray1, maxSPArray, idxArray);
            total += getTimeMs() - start;
        }
        printf("Total Execution Time: %.4f ms\n", total);
        checkOutput(n, maxSPArrayC, idxArrayC, maxSPArray, idxArray, "x86-64");
        printResults(n, limit, total, maxSPArray, idxArray, "x86-64");

        
        //SIMD XMM 
        printf("\nStarting AVX Version...\n");
        resetValues(n, maxSPArray, idxArray, &total);
        for (int i = 0; i < RUNS; i++)
        {
            start = getTimeMs();
            avxFuncMaxSPArray(n, spArray0, spArray1, maxSPArray, idxArray);
            total += getTimeMs() - start;
        }
        printf("Total Execution Time: %.4f ms\n", total);
        checkOutput(n, maxSPArrayC, idxArrayC, maxSPArray, idxArray, "SIMD XMM");
        printResults(n, limit, total, maxSPArray, idxArray, "SIMD XMM");


        //SIMD YMM
        printf("\nStarting SIMD YMM Version...\n");
        resetValues(n, maxSPArray, idxArray, &total);
        for (int i = 0; i < RUNS; i++)
        {
            start = getTimeMs();
            avx2FuncMaxSPArray(n, spArray0, spArray1, maxSPArray, idxArray);
            total += getTimeMs() - start;
        }
        printf("Total Execution Time: %.4f ms\n", total);
        checkOutput(n, maxSPArrayC, idxArrayC, maxSPArray, idxArray, "SIMD YMM");
        printResults(n, limit, total, maxSPArray, idxArray, "SIMD YMM");

    }
    free(spArray0);
    free(spArray1);
    free(maxSPArrayC);
    free(idxArrayC);
    free(maxSPArray);
    free(idxArray);
}

// Function Implementations **************************************************

//Prints average time and first, last 5 elems of max and idx arrays
void printResults(int n, int limit, double total, float* spArray, int* idxArray, char* version)
{
    printf("%s version average time: %.4f ms\n", version, total / RUNS);

    printf("%s max array first %d elements: ", version, limit);
    for (int i = 0; i < limit; i++)
        printf("%.2f ", spArray[i]);
    printf("\n%s idx array first %d elements: ", version, limit);
    for (int i = 0; i < limit; i++)
        printf("%d ", idxArray[i]);
    printf("\n");

    printf("%s max array last %d elements: ", version, limit);
    for (int i = 0; i < limit; i++)
        printf("%.2f ", spArray[n - limit + i]);
    printf("\n%s idx array last %d elements: ", version, limit);
    for (int i = 0; i < limit; i++)
        printf("%d ", idxArray[n - limit + i]);
    printf("\n");

    printf("Done %s version.\n", version);
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

// resets array values with total
void resetValues(int n, float* maxSPArray, int* idxArray, double* total)
{
    for (int i = 0; i < n; i++)
    {
        maxSPArray[i] = -1.0f;
        idxArray[i] = -1;
    } // so skipped elements get caught
    *total = 0.0;
}
