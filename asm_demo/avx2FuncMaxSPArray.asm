;   avx2FuncMaxSPArray.asm
;
;   void avx2FuncMaxSPArray(int n, float* spArray0, float* spArray1, float* maxSPArray, int* idxArray);
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
zero_dd: times 8 dd 0                   
one_dd:  times 8 dd 1       
n dd 10
spArray0 dd 1.49, 5.31, 6.55, 6.94, 5.79, 1.53, 4.89, 7.87, 0.82, 3.12 
spArray1 dd 7.40, 3.80, 3.90, 3.70, 6.62, 2.01, 9.93, 1.48, 2.32, 1.78
 
; Max Array: 7.40, 5.31, 6.55, 6.94, 6.62, 2.01, 9.93, 7.87, 2.32, 3.12 
; p { $ymm0.v8_float, $ymm1.v8_float, $ymm2.v8_int32, $ymm3.v8_float, $ymm4.v8_float, $ymm5.v8_int32}

; General Address content loomup
; x/{N}fw {address, &varName, $reg}

; x/10fw $rdx - arr0
; x/10fw $r8 - arr1
; x/10fw $r9 - maxArr
; x/10dw $r10 - idxArr

section .text
align 16
global main

main:
    mov     ecx, [n]
    lea     rdx, [spArray0]
    lea     r8, [spArray1]
    lea     r9, [maxSPArray]
    
    test    ecx, ecx                    ; n <= 0 ?
    jle     ym_done

    lea     r10, [idxArray]
    ;lea     r10, qword [rsp+0x28]      ; r10 = idx
    xor     r11, r11                    ; r11 = byte offset
    mov     eax, ecx                    ; eax = n
    shr     ecx, 3                      ; ecx = n / 8 
    test    ecx, ecx
    jz      ym_tail                     ; n < 8 -> scalar tail only

    vmovups ymm5, [zero_dd]             ; Why not broadcast to save mem? xD

ym_loop:
    vmovups ymm0, [rdx+r11]             
    vmovups ymm1, [r8+r11]              
    vcmpps  ymm2, ymm0, ymm1, 1         ; mask = (A < B)
    vblendvps ymm3, ymm0, ymm1, ymm2    
    vblendvps ymm4, ymm5, [one_dd], ymm2 
    vmovups [r9+r11], ymm3             
    vmovdqu [r10+r11], ymm4            

    add     r11, 32
    dec     ecx
    jnz     ym_loop

ym_tail:
    mov     ecx, eax
    and     ecx, 7                      ; remainder = n % 8
    jz      ym_done

ym_tail_loop:
    movss   xmm0, dword [rdx+r11]
    movss   xmm1, dword [r8+r11]
    comiss  xmm0, xmm1                  
    jb      ym_take_b

    mov     dword [r10+r11], 0         
    movss   dword [r9+r11], xmm0       
    jmp     ym_tail_next

ym_take_b:
    mov     dword [r10+r11], 1          
    movss   dword [r9+r11], xmm1        

ym_tail_next:
    add     r11, 4
    dec     ecx
    jnz     ym_tail_loop

ym_done:
    ret
