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

section .data
zero_dd: dd 0, 0, 0, 0                
one_dd:  dd 1, 1, 1, 1                  

section .text
global avxFuncMaxSPArray

avxFuncMaxSPArray:
    test    ecx, ecx                    ; n <= 0 ?
    jle     xm_done

    mov     r10, [rsp+40]         ; idx
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