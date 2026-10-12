# CSC612M - SIMD Programming

**Group 4 | G01**

## Group Members
- AGUARIN, Trish Ann Danielle C.
- ANDAYA, Kurt Christian D.
- CABATO, Marxandrea Diane G.

---

## 1. Program Output and Execution Time

### Program Output

#### $2^{20}+0$ Print Output
![2^20+0](/screenshots/2^20+0_output.png)

#### $2^{20}+3$ Print Output
![2^20+3](/screenshots/2^20+3_output.png)

#### $2^{26}+0$ Print Output
![2^26](/screenshots/2^26_output.png)

#### $2^{28}+0$ Print Output
![2^28](/screenshots/2^28_output.png)


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


[EXPLANATION HERE on how our implementation handles the remaining elements pag di divisible sa SIMD vector width]

---

## 4. Discussion

### 4.1 Which Implementation Is the Fastest?

The fastest implementation is with the AVX2 setup, as it was 17.09x, 9.74x, and 17.18x faster than the standard C (scalar) baseline in $2^{20}$, $2^{26}$, and $2^{28}$.

The possible reasons why it is the fastest among the kernels is because of its capability of holding and manipulating larger amounts of data two times better than XMM due to its capacity of 256 bits. It also leverages on the SIMD architecture to perform the operation in a quicker pace than C which performs the calculations “one by one” in a sequential phase, and not parallel.

### 4.2 Why Is the Speedup Different from Theoretical Expectations?

Based on the results of the runs we have, the results did not show the theoretical effect that YMM should be twice as faster to XMM given that YMM was only ~1.42 times faster than XMM for $2^{20}$ and $2^{28}$, but was almost the same time for the $2^{26}$ one at nine times faster. We also tried doing multiple runs to check this effect and it held true for most of the runs. 

One of the possible reasons is memory bandwidth limitations. In cases such as $2^{26}$ and $2^{28}$, vector sizes are too large that the system could not supply data fast enough to keep the 256-bit vector execution going. With this, performance becomes memory-bound than compute-bound.

^^ SAPAT NA BA TOH IMMA CRY IF HINDI

Moreover, we also noticed that during the multiple runs, there were instances that XMM was a little faster than YMM from merely milliseconds to a few seconds apart (see figures below). This “discrepancy?” can be due to runtime environmental factors such as OS context switching, thread preemption, or differing hardware specifications. (Would cache be issue here too?)


#### $2^{20}+3$ instance where XMM is faster 
![XMM_Faster_2^20+3](screenshots/2^20+3_xmm_faster.png)

#### $2^{26}$ instance where XMM is faster 
![XMM_Faster_2^26](screenshots/2^26_xmm_faster.png)

### 4.3 Problems Encountered and Solutions

1. $2^{30}$ restriction
>When running the program, we encountered issues when the vector size is 2^30. We just had to reduce it to 2^28 since ayun lang yung size na successfully nagrun yung code. It was due to the memory capability of our respective machines 

2. QueryPerformanceCounter instead of clock() 
>QPC uses a high-resolution performance counter which lets you measure small time differemce more precisely, which is important when comparing implementations on SIMD-optimized versions. (sapat na ba toh as explanation)


### 4.4 Unique Methodology Used
// kurt shall do this

### 4.5 AHA Moments and Key Learnings
// float zero_dd incident

Data types matter when getting assigned to the registers, specifically for float or integer. We had this incident of declaring zero_dd and one_dd into float types initially and in the output, it appears that the XMM and YMM register outputs are not the same. This is because the float types get read differently in hex value than as an integer, which triggers the “not same” flag for it. WIth this, we made sure that both are in integer format so it can show that the SIMD results are the same as standard C baseline. 


// kurt's baliktad idea (starting from 28)


// ymm doesn't guarantee to be the fastest

Given the multiple run results, it shows that wider SIMD instructions do not automatically mean better performance. It still depends on the CPU microarchitecture, instruction type, workload, and surrounding code.



