;   avxFuncMaxSPArray.asm
;
;   void avxFuncMaxSPArray(int n, float* spArray0, float* spArray1, float* maxSPArray, int* idxArray);
;
;   rcx = n
;   rdx = spArray0
;   r8  = spArray1
;   r9  = maxSPArray
;   [rsp+40] = idx   (32-byte shadow space + 8-byte return addr)

default rel    
                        
section .bss
maxSPArray resd 10
; buffer resd 10
idxArray resd 10

section .data
zero_dd: dd 0, 0, 0, 0                
one_dd:  dd 1, 1, 1, 1         
n dd 10
spArray0 dd 1.49, 5.31, 6.55, 6.94, 5.79, 1.53, 4.89, 7.87, 0.82, 3.12 
spArray1 dd 7.40, 3.80, 3.90, 3.70, 6.62, 2.01, 9.93, 1.48, 2.32, 1.78
 
; Max Array: 7.40, 5.31, 6.55, 6.94, 6.62, 2.01, 9.93, 7.87, 2.32, 3.12 
; p { $xmm0.v4_float, $xmm1.v4_float, $xmm2.v4_int32, $xmm3.v4_float, $xmm4.v4_float, $xmm5.v4_int32}

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
    jle     xm_done

    lea     r10, [idxArray]     
    ;lea     r10, qword [rsp+0x28]      ; r10 = idx
    xor     r11, r11                    ; r11 = byte offset
    mov     eax, ecx                    ; eax = n
    shr     ecx, 2                      ; ecx = n / 4 
    test    ecx, ecx
    jz      xm_tail                    

    vmovups xmm5, [zero_dd]           

xm_loop:
    vmovups xmm0, [rdx+r11]            
    vmovups xmm1, [r8+r11]             
    vcmpps  xmm2, xmm0, xmm1, 1         ; mask = (A < B)
    vblendvps xmm3, xmm0, xmm1, xmm2    
    vblendvps xmm4, xmm5, [one_dd], xmm2 
    vmovups [r9+r11], xmm3              
    vmovdqu [r10+r11], xmm4           

    add     r11, 16
    dec     ecx
    jnz     xm_loop

xm_tail:
    mov     ecx, eax
    and     ecx, 3                      ; remainder = n % 4
    jz      xm_done

xm_tail_loop:
    movss   xmm0, dword [rdx+r11]
    movss   xmm1, dword [r8+r11]
    comiss  xmm0, xmm1                 
    jb      xm_take_b

    mov     dword [r10+r11], 0          
    movss   dword [r9+r11], xmm0       
    jmp     xm_tail_next

xm_take_b:
    mov     dword [r10+r11], 1       
    movss   dword [r9+r11], xmm1     

xm_tail_next:
    add     r11, 4
    dec     ecx
    jnz     xm_tail_loop

xm_done:
    ret
