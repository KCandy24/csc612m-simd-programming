# CSC612M - SIMD Programming

**Group 4 | G01**

## Group Members
- AGUARIN, Trish Ann Danielle C.
- ANDAYA, Kurt Christian D.
- CABATO, Marxandrea Diane G.

---

## 1. Program Output and Execution Time

### Program Output (Release)

#### $2^{20}+0$ Print Output
![2^20+0](/screenshots/2^20+0_release_output.png)

#### $2^{20}+3$ Print Output
![2^20+3](/screenshots/2^20+3_release_output.png)

#### $2^{26}+0$ Print Output
![2^26](/screenshots/2^26_release_output.png)

#### $2^{28}+0$ Print Output
![2^28](/screenshots/2^28_release_output.png)

### Program Output (Debug)

#### $2^{20}+0$ Print Output
![2^20+0](/screenshots/2^20+0_debug_output.png)

#### $2^{20}+3$ Print Output
![2^20+3](/screenshots/2^20+3_debug_output.png)

#### $2^{26}+0$ Print Output
![2^26](/screenshots/2^26_debug_output.png)

#### $2^{28}+0$ Print Output
![2^28](/screenshots/2^28_debug_output.png)

### Comparative Execution Time (in ms)
** This is without offset **

| Kernel | $2^{20}$ | $2^{26}$ | $2^{28}$ |
|:---|---:|---:|---:|
| C Version | 20.7768 | 1083.9886 | 6154.6507 |
| x86-64 Assembly | 12.9786 | 692.0763 | 3561.2578 |
| AVX (XMM) | 1.6531 | 115.0236 | 481.0688 |
| AVX2 (YMM) | 1.2157 | 111.3414 | 358.1627 |

### Performance Speedup Analysis

**a. x86-64 Assembly:** How many times faster is it than C?
- $2^{20}: 1.600 \times$
- $2^{26}: 1.566 \times$
- $2^{28}: 1.728 \times$

**b. AVX (XMM):** How many times faster is it than C?
- $2^{20}: 12.568 \times$
- $2^{26}: 9.424 \times$
- $2^{28}: 12.794 \times$

**c. AVX2 (YMM):** How many times faster is it than C?
- $2^{20}: 17.090 \times$
- $2^{26}: 9.736 \times$
- $2^{28}: 17.184 \times$

**Why are the optimized implementations faster?**

The optimized implementations are faster because they reduce the amount of work that the CPU performs for each data element and leverages the processor's hardware capabilities. 

Assembly-level optimization has fewer instructions and better register usage, allowing the CPU to perform tasks with less overhead. Meanwhile, SIMD instructions enable parallel processing of multiple data elements in a single instruction. AVX (XMM) and AVX2 (YMM) further improve performance by processing multiple elements at once, with YMM registers holding twice as many elements as XMM registers.

However, the actual speedup still depends on factors such as memory bandwidth, CPU architecture, and the type of operations performed.

---

## 2. Correctness Verification

### 2.1 C Version

![C Correctness Check](screenshots/C%20Version.png)

### 2.2 x86-64 Assembly

![x86-64 Correctness Check](screenshots/x86-64.png)

### 2.3 AVX (XMM)

![AVX XMM Correctness Check](screenshots/SIMD_XMM.png)

### 2.4 AVX2 (YMM)

![AVX2 YMM Correctness Check](screenshots/SIMD_YMM.png)

---

## 3. SIMD Boundary Checking (AVX (XMM) and AVX2 (YMM))

![AVX2 YMM Correctness Check](screenshots/Boundary%20Check.png)



```


    mov     r10, [rsp+40]               ; idx
    xor     r11, r11                    ; r11 = byte offset 
    mov     eax, ecx                    ; eax = n
    shr     ecx, 3 ; or 2               ; ecx = n / 8 
    test    ecx, ecx
    jz      tail                     ; n < 8 -> scalar tail only

    ...

    tail:
        mov     ecx, eax
        and     ecx, 7 ;or 3           ; remainder = n % 8 or % 4
        jz      ym_done

    tail_loop:
        movss   xmm0, dword [rdx+r11]
        movss   xmm1, dword [r8+r11]
        comiss  xmm0, xmm1                  
        jb      take_b

        mov     dword [r10+r11], 0      ; store 0 in idxArray   
        movss   dword [r9+r11], xmm0    ; store 1st array's element in maxArray
        jmp     tail_next

    take_b:
        mov     dword [r10+r11], 1      ; store 1 in idxArray    
        movss   dword [r9+r11], xmm1    ; store 2nd array's element in maxArray

    tail_next:
        add     r11, 4
        dec     ecx
        jnz     tail_loop

    done:
        ret
        
```

Our implementation of boundary handling involves taking the remainder and storing it. A special case is done if the original number is less than the number of single precision in the register, the porgram automatically jumps to the tail subsection. The tail subsection iterates over the remainder to operate the rest of the array to obtain the max element and the index.

---

## 4. Discussion

### 4.1 Which Implementation Is the Fastest?

The fastest implementation is with the AVX2 setup, as it was 17.09x, 9.74x, and 17.18x faster than the standard C (scalar) baseline in $2^{20}$, $2^{26}$, and $2^{28}$.

The possible reasons why it is the fastest among the kernels is because of its capability of holding and manipulating larger amounts of data two times better than XMM due to its capacity of 256 bits. It also leverages on the SIMD architecture to perform the operation in a quicker pace than C which performs the calculations “one by one” in a sequential phase, and not parallel.

### 4.2 Why Is the Speedup Different from Theoretical Expectations?

Based on the results of the runs we have, the results did not show the theoretical effect that YMM should be twice as faster to XMM given that YMM was only ~1.42 times faster than XMM for $2^{20}$ and $2^{28}$, but was almost the same time for the $2^{26}$ one at nine times faster. We also tried doing multiple runs to check this effect and it held true for most of the runs. 

One of the possible reasons is memory bandwidth limitations. In cases such as $2^{26}$ and $2^{28}$, vector sizes are too large that the system could not supply data fast enough to keep the 256-bit vector execution going. With this, performance becomes memory-bound than compute-bound.

Moreover, we also noticed that during the multiple runs, there were instances that XMM was a little faster than YMM from merely milliseconds to a few seconds apart (see figures below). This discrepancy can be due to runtime environmental factors such as OS context switching, thread preemption, or differing hardware specifications.


#### $2^{20}+3$ instance where XMM is faster 
![XMM_Faster_2^20+3](screenshots/2^20+3_xmm_faster.png)

#### $2^{26}$ instance where XMM is faster 
![XMM_Faster_2^26](screenshots/2^26_xmm_faster.png)

### 4.3 Problems Encountered and Solutions

1. $2^{30}$ restriction
>When running the program, we encountered issues when the vector size is 2^30. We just had to reduce it to 2^28 since ayun lang yung size na successfully nagrun yung code. It was due to the memory capability of our respective machines 

2. QueryPerformanceCounter instead of clock() 
>QPC uses a high-resolution performance counter which lets you measure small time differemce more precisely, which is important when comparing implementations on SIMD-optimized versions where differences may be miniscule.


### 4.4 Unique Methodology Used

1. **Seeded Random Numbers**: To ensure deterministic values for array 0 and array 1, we implimented a random number generator seeded at `seed = 20` which generates the same numbers across runs. These values can also be stored in text files through the `util/document_values.c` program.

2.  **Shift for Div, And for Remainder**:  `shr` is used to divide the number while the original is stored and an `and` is used to the the dividend minus 1 to get the remainder.  

3. **Generating and using a Mask**: `vcmpps` is used to compare the vector of single precision values to generate a bit mask which is then used to choose between two arrays through `vblendvps`.

### 4.5 AHA Moments and Key Learnings

A minor mistake that flew under the radar was the initialization of `zero_dd` to a float which caused the index array to show values that are incorrect due to the wrong initialization of the array 

Data types matter when getting assigned to the registers, specifically for float or integer. We had this incident of declaring zero_dd and one_dd into float types initially and in the output, it appears that the XMM and YMM register outputs are not the same. This is because the float types get read differently in hex value than as an integer, which triggers the “not same” flag for it. WIth this, we made sure that both are in integer format so it can show that the SIMD results are the same as standard C baseline. 

Initially, the program re-initializes all arrays upon switching to a different array size, the issue becomes the extensive re-use of memory which is why midway implementation, we decided to forego allocation and initialization for each array size and allocate to the largest array size before initializing the values for array 0 and array 1.  

In theory, YMM SIMD implementation should be the fastest, however in practicality, it shows little improvement and sometimes even underperform XMM SIMD, this made us learn that YMM although may be handling multiple data, will not automatically outperform XMM.

Given the multiple run results, it shows that wider SIMD instructions do not automatically mean better performance. It still depends on the CPU microarchitecture, instruction type, workload, and surrounding code.


