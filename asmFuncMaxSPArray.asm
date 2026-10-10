;   x86-64 scalar kernel 
;
;   void asmFuncMaxSPArray (int n, float* spArray0, float* spArray1, float* maxSPArray, int* idxArray);
;
;   rcx = n
;   rdx = spArray0
;   r8  = spArray1
;   r9  = maxSPArray
;   [rsp+40] = idx   (32-byte shadow space + 8-byte return addr)

section .text
global asmFuncMaxSPArray

asmFuncMaxSPArray:
    test    ecx, ecx                    ; n <= 0 ?
    jle     sc_done

    mov     r10, [rsp+40]         ; idx
    xor     r11, r11                    ; r11 = byte offset 

sc_loop:
    movss   xmm0, dword [rdx+r11]       ; xmm0 = spArray0[i]
    movss   xmm1, dword [r8+r11]        ; xmm1 = spArray1[i]
    comiss  xmm0, xmm1                  
    jb      sc_take_b                  

    mov     dword [r10+r11], 0          
    movss   dword [r9+r11], xmm0        
    jmp     sc_next

sc_take_b:
    mov     dword [r10+r11], 1          
    movss   dword [r9+r11], xmm1        

sc_next:
    add     r11, 4
    dec     ecx
    jnz     sc_loop

sc_done:
    ret
