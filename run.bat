nasm -f win64 asmFuncMaxSPArray.asm -o asmFuncMaxSPArray.obj
nasm -f win64 avxFuncMaxSPArray.asm -o avxFuncMaxSPArray.obj
nasm -f win64 avx2FuncMaxSPArray.asm -o avx2FuncMaxSPArray.obj
gcc main.c asmFuncMaxSPArray.obj avxFuncMaxSPArray.obj avx2FuncMaxSPArray.obj -o main.exe
.\main.exe