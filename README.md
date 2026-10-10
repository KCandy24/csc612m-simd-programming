# CSC612M - SIMD Programming

**Group 4 | G01**

## Group Members
- AGUARIN, Trish Ann Danielle C.
- ANDAYA, Kurt Christian D.
- CABATO, Marxandrea Diane G.

---

## 1. Program Output and Execution Time

### Program Output

//![Program Output](screenshots/program-output.png)

### Comparative Execution Time

| Kernel | 2^20 | 2^26 | 2^28 |
|:---|---:|---:|---:|
| C Version | - | - | - |
| x86-64 Assembly | - | - | - |
| AVX (XMM) | - | - | - |
| AVX2 (YMM) | - | - | - |

### Performance Speedup Analysis

- **a. x86-64 Assembly:** How many times faster is it than C?
- **b. AVX (XMM):** How many times faster is it than C?
- **c. AVX2 (YMM):** How many times faster is it than C?

**Why are the optimized implementations faster?**

The optimized implementations are faster because they reduce the amount of work that the CPU performs for each data element and leverages the processor's hardware capabilities. 

Assembly-level optimization has fewer instructions and better register usage, allowing the CPU to perform tasks with less overhead. Meanwhile, SIMD instructions enable parallel processing of multiple data elements in a single instruction. AVX (XMM) and AVX2 (YMM) further improve performance by processing multiple elements at once, with YMM registers holding twice as many elements as XMM registers.

However, the actual speedup still depends on factors such as memory bandwidth, CPU architecture, and the type of operations performed.

---

## 2. Correctness Verification

### 2.1 C Version

//![C Correctness Check](screenshots/correctness-c.png)

### 2.2 x86-64 Assembly

//![x86-64 Correctness Check](screenshots/correctness-x86-64.png)

### 2.3 AVX (XMM)

//![AVX XMM Correctness Check](screenshots/correctness-avx.png)

### 2.4 AVX2 (YMM)

//![AVX2 YMM Correctness Check](screenshots/correctness-avx2.png)

---

## 3. SIMD Boundary Checking

### 3.1 AVX (XMM) Boundary Check

//![XMM Boundary Check](screenshots/boundary-xmm.png)

### 3.2 AVX2 (YMM) Boundary Check

//![YMM Boundary Check](screenshots/boundary-ymm.png)

[EXPLANATION HERE on how our implementation handles the remaining elements pag di divisible sa SIMD vector width]

---

## 4. Discussion

### 4.1 Which Implementation Is the Fastest?

[answer here]

### 4.2 Why Is It Faster?

[answer here]

### 4.3 Why Is the Speedup Different from Theoretical Expectations?

[answer here]

### 4.4 Problems Encountered and Solutions

1. 2^30 restriction
When running the program, we encountered issues when the vector size is 2^30. We just had to reduce it to 2^28 since ayun lang yung size na successfully nagrun yung code. It was due to the memory capability of our respective machines 

2. QueryPerformanceCounter instead of clock() 
QPC uses a high-resolution performance counter which lets you measure small time differemce more precisely, which is important when comparing implementations on SIMD-optimized versions. (sapat na ba toh as explanation)

### 4.5 Unique Methodology Used



### 4.6 AHA Moments and Key Learnings



