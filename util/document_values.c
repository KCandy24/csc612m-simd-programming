/*    
gcc document_values.c -o document_values.exe
./document_values.exe

RUN AT YOUR OWN RISK GENERATES 2 x 2GB TXT FILES
*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
    // 1. Declare a file pointer
    FILE *array1;
    FILE *array2;

    array1 = fopen("array1.txt", "w");
    array2 = fopen("array2.txt", "w");

    if (array1 == NULL || array2 == NULL) {
        printf("Error: Could not open or create the file.\n");
        return 1; 
    }

    unsigned int seed = 20;
    srand(seed);

    int twoTo28 = (1 << 28);

    srand(seed);
    for (int i = 0; i < twoTo28; i++)
    {
        fprintf(array1, "%.4f\n", (float)rand() / (float)(RAND_MAX) * 10);
        fprintf(array2, "%.4f\n", (float)rand() / (float)(RAND_MAX) * 10);
    }

    fclose(array1);
    fclose(array2);
    return 0;
}

