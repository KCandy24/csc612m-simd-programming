// C version
void funcMaxSPArray(int n, float* spArray0, float* spArray1, float* maxSPArray, int* idxArray)
{
    for (int i = 0; i < n; i++)
    {
        idxArray[i] = (spArray0[i] >= spArray1[i] ? 0 : 1);
        maxSPArray[i] = (!idxArray[i] ? spArray0[i] : spArray1[i]);
    }
}