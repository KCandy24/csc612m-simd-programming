;   x86-64 scalar kernel 
;
;   void asmFuncMaxSPArray (int n, float* spArray0, float* spArray1, float* maxSPArray, int* idxArray);
;
;   rcx = n
;   rdx = spArray0
;   r8  = spArray1
;   r9  = maxSPArray
;   [rsp+40] = idx   (32-byte shadow space + 8-byte return addr)

section .bss
maxSPArray resd 10
; buffer resd 10
idxArray resd 10

section .data
n dd 10
spArray0 dd 1.49, 5.31, 6.55, 6.94, 5.79, 1.53, 4.89, 7.87, 0.82, 3.12 
spArray1 dd 7.40, 3.80, 3.90, 3.70, 6.62, 2.01, 9.93, 1.48, 2.32, 1.78
 
; Max Array: 7.40, 5.31, 6.55, 6.94, 6.62, 2.01, 9.93, 7.87, 2.32, 3.12 

; General Address content loomup
; x/{N}fw {address, &varName, $reg}

; x/10fw $rdx - arr0
; x/10fw $r8 - arr1
; x/10fw $r9 - maxArr
; x/10dw $r10 - idxArr

section .text
global main

main:
    mov     ecx, [n]
    lea     rdx, [spArray0]
    lea     r8, [spArray1]
    lea     r9, [maxSPArray]
    
    test    ecx, ecx                    ; n <= 0 ?
    jle     sc_done

    lea     r10, [idxArray]     
    ;mov     r10, qword [rsp+0x28]      ; idx
    xor     r11d, r11d                  ; r11 = byte offset 

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
