HW03
===
This is the hw03 sample. Please follow the steps below.

# Build the Sample Program

1. Fork this repo to your own github account.

2. Clone the repo that you just forked.

3. Under the hw03 dir, use:

	* `make` to build.

	* `make clean` to clean the ouput files.

4. Extract `gnu-mcu-eclipse-qemu.zip` into hw03 dir. Under the path of hw03, start emulation with `make qemu`.

	See [Lecture 02 ─ Emulation with QEMU] for more details.

5. The sample is a minimal program for ARM Cortex-M4 devices, which enters `while(1);` after reset. Use gdb to get more details.

	See [ESEmbedded_HW02_Example] for knowing how to do the observation and how to use markdown for taking notes.

# Build Your Own Program

1. Edit main.c.

2. Make and run like the steps above.

3. Please avoid using hardware dependent C Standard library functions like `printf`, `malloc`, etc.

# HW03 Requirements

1. How do C functions pass and return parameters? Please describe the related standard used by the Application Binary Interface (ABI) for the ARM architecture.

2. Modify main.c to observe what you found.

3. You have to state how you designed the observation (code), and how you performed it.

	Just like how you did in HW02.

3. If there are any official data that define the rules, you can also use them as references.

4. Push your repo to your github. (Use .gitignore to exclude the output files like object files or executable files and the qemu bin folder)

[Lecture 02 ─ Emulation with QEMU]: http://www.nc.es.ncku.edu.tw/course/embedded/02/#Emulation-with-QEMU
[ESEmbedded_HW02_Example]: https://github.com/vwxyzjimmy/ESEmbedded_HW02_Example

--------------------

- [ ] **If you volunteer to give the presentation next week, check this.**

--------------------

**★★★ Please take your note here ★★★**

HW03
===

1.實驗題目
---

觀察C語言是如何 pass and return 參數

2.calling conventions介紹
---

我們在使用函式呼叫時牽涉到的參數傳遞，並不只是單純跳到那個Address執行程式碼再跳回來這麼簡單，呼叫副程式的主程式，需要知道怎麼填參數，副程式才能接到參數後進行處理，再將結果傳給主程式，所以這段協定稱之為Calling Convention(呼叫協定)。

3.AAPCS  Procedure Call Standard for the ARM® Architecture
---

THE BASE PROCEDURE CALL STANDARD

1. **Core registers**

![](https://i.imgur.com/hF3gmGa.png)

The first four registers r0-r3 (a1-a4) are used to pass argument values into a subroutine and to return a result
value from a function. They may also be used to hold intermediate values within a routine (but, in general, only
between subroutine calls). 

2. **Subroutine Calls**

Both the ARM and Thumb instruction sets contain a primitive subroutine call instruction, **BL, which performs a
branch-with-link operation.** The effect of executing BL is to transfer the sequentially next value of the program
counter **—the return address—into the link register (LR)** and **the destination address into the program counter
(PC)**. Bit 0 of the link register will be set to 1 if the BL instruction was executed from Thumb state, and to 0 if
executed from ARM state. The result is to transfer control to the destination address, passing the return address in
LR as an additional parameter to the called subroutine.

3. **Result Return**

**A Half-precision Floating Point Type is returned in the least significant 16 bits of r0.**

A Fundamental Data Type that is smaller than 4 bytes is zero- or sign-extended to a word and returned in r0.

4. **Parameter Passing**

The base standard provides for passing arguments in core registers (r0-r3) and on the stack. For subroutines that
take a small number of parameters, only registers are used, greatly reducing the overhead of a call.

4.實驗步驟
---
1. 設計測試程式 main.c ，從 reset_handler 開始後依序執行，觀察 function with 2 parameters 和 function with 5 parameters 和 function using pointer 的不同

main.c

```c
int sum(int a,int b){
	int c = a+b;
	return c;
}
int arithmetic(int a,int b,int c,int d,int e){
	int f = (a+b)+(c*d)-e;
	return f;
}
int pointer(int *a,int *b,int *c){
	int d = *a + *b + *c;
	return d;
}
void reset_handler(void)
{
	int a = 100;
	int b = 200;
	int c = 300;
	sum(10,20);
	arithmetic(1,2,3,4,5);
	pointer(&a,&b,&c);
	while (1)
		;
}
```
2.將 main.s 編譯並以 qemu 模擬， $ make clean, $ make, $ make qemu 開啟另一 Terminal 連線 $ arm-none-eabi-gdb ，再輸入 target remote 
localhost:1234 連接，輸入兩次的 ctrl + x 再輸入 2, 開啟 Register 以及指令，並且輸入 si 單步執行觀察。

objdump的結果
```
Disassembly of section .mytext:

00000000 <sum-0x8>:
   0:	20000100 	andcs	r0, r0, r0, lsl #2
   4:	00000089 	andeq	r0, r0, r9, lsl #1

00000008 <sum>:
   8:	b480      	push	{r7}
   a:	b085      	sub	sp, #20
   c:	af00      	add	r7, sp, #0
   e:	6078      	str	r0, [r7, #4]
  10:	6039      	str	r1, [r7, #0]
  12:	687a      	ldr	r2, [r7, #4]
  14:	683b      	ldr	r3, [r7, #0]
  16:	4413      	add	r3, r2
  18:	60fb      	str	r3, [r7, #12]
  1a:	68fb      	ldr	r3, [r7, #12]
  1c:	4618      	mov	r0, r3
  1e:	3714      	adds	r7, #20
  20:	46bd      	mov	sp, r7
  22:	f85d 7b04 	ldr.w	r7, [sp], #4
  26:	4770      	bx	lr

00000028 <arithmetic>:
  28:	b480      	push	{r7}
  2a:	b087      	sub	sp, #28
  2c:	af00      	add	r7, sp, #0
  2e:	60f8      	str	r0, [r7, #12]
  30:	60b9      	str	r1, [r7, #8]
  32:	607a      	str	r2, [r7, #4]
  34:	603b      	str	r3, [r7, #0]
  36:	68fa      	ldr	r2, [r7, #12]
  38:	68bb      	ldr	r3, [r7, #8]
  3a:	441a      	add	r2, r3
  3c:	687b      	ldr	r3, [r7, #4]
  3e:	6839      	ldr	r1, [r7, #0]
  40:	fb01 f303 	mul.w	r3, r1, r3
  44:	441a      	add	r2, r3
  46:	6a3b      	ldr	r3, [r7, #32]
  48:	1ad3      	subs	r3, r2, r3
  4a:	617b      	str	r3, [r7, #20]
  4c:	697b      	ldr	r3, [r7, #20]
  4e:	4618      	mov	r0, r3
  50:	371c      	adds	r7, #28
  52:	46bd      	mov	sp, r7
  54:	f85d 7b04 	ldr.w	r7, [sp], #4
  58:	4770      	bx	lr
  5a:	bf00      	nop

0000005c <pointer>:
  5c:	b480      	push	{r7}
  5e:	b087      	sub	sp, #28
  60:	af00      	add	r7, sp, #0
  62:	60f8      	str	r0, [r7, #12]
  64:	60b9      	str	r1, [r7, #8]
  66:	607a      	str	r2, [r7, #4]
  68:	68fb      	ldr	r3, [r7, #12]
  6a:	681a      	ldr	r2, [r3, #0]
  6c:	68bb      	ldr	r3, [r7, #8]
  6e:	681b      	ldr	r3, [r3, #0]
  70:	441a      	add	r2, r3
  72:	687b      	ldr	r3, [r7, #4]
  74:	681b      	ldr	r3, [r3, #0]
  76:	4413      	add	r3, r2
  78:	617b      	str	r3, [r7, #20]
  7a:	697b      	ldr	r3, [r7, #20]
  7c:	4618      	mov	r0, r3
  7e:	371c      	adds	r7, #28
  80:	46bd      	mov	sp, r7
  82:	f85d 7b04 	ldr.w	r7, [sp], #4
  86:	4770      	bx	lr

00000088 <reset_handler>:
  88:	b580      	push	{r7, lr}
  8a:	b086      	sub	sp, #24
  8c:	af02      	add	r7, sp, #8
  8e:	2364      	movs	r3, #100	; 0x64
  90:	60fb      	str	r3, [r7, #12]
  92:	23c8      	movs	r3, #200	; 0xc8
  94:	60bb      	str	r3, [r7, #8]
  96:	f44f 7396 	mov.w	r3, #300	; 0x12c
  9a:	607b      	str	r3, [r7, #4]
  9c:	200a      	movs	r0, #10
  9e:	2114      	movs	r1, #20
  a0:	f7ff ffb2 	bl	8 <sum>
  a4:	2305      	movs	r3, #5
  a6:	9300      	str	r3, [sp, #0]
  a8:	2001      	movs	r0, #1
  aa:	2102      	movs	r1, #2
  ac:	2203      	movs	r2, #3
  ae:	2304      	movs	r3, #4
  b0:	f7ff ffba 	bl	28 <arithmetic>
  b4:	f107 010c 	add.w	r1, r7, #12
  b8:	f107 0208 	add.w	r2, r7, #8
  bc:	1d3b      	adds	r3, r7, #4
  be:	4608      	mov	r0, r1
  c0:	4611      	mov	r1, r2
  c2:	461a      	mov	r2, r3
  c4:	f7ff ffca 	bl	5c <pointer>
  c8:	e7fe      	b.n	c8 <reset_handler+0x40>
  ca:	bf00      	nop

Disassembly of section .comment:

00000000 <.comment>:
   0:	3a434347 	bcc	10d0d24 <reset_handler+0x10d0c9c>
   4:	35312820 	ldrcc	r2, [r1, #-2080]!	; 0xfffff7e0
   8:	392e343a 	stmdbcc	lr!, {r1, r3, r4, r5, sl, ip, sp}
   c:	732b332e 			; <UNDEFINED> instruction: 0x732b332e
  10:	33326e76 	teqcc	r2, #1888	; 0x760
  14:	37373131 			; <UNDEFINED> instruction: 0x37373131
  18:	2029312d 	eorcs	r3, r9, sp, lsr #2
  1c:	2e392e34 	mrccs	14, 1, r2, cr9, cr4, {1}
  20:	30322033 	eorscc	r2, r2, r3, lsr r0
  24:	35303531 	ldrcc	r3, [r0, #-1329]!	; 0xfffffacf
  28:	28203932 	stmdacs	r0!, {r1, r4, r5, r8, fp, ip, sp}
  2c:	72657270 	rsbvc	r7, r5, #112, 4
  30:	61656c65 	cmnvs	r5, r5, ror #24
  34:	00296573 	eoreq	r6, r9, r3, ror r5

Disassembly of section .ARM.attributes:

00000000 <.ARM.attributes>:
   0:	00003041 	andeq	r3, r0, r1, asr #32
   4:	61656100 	cmnvs	r5, r0, lsl #2
   8:	01006962 	tsteq	r0, r2, ror #18
   c:	00000026 	andeq	r0, r0, r6, lsr #32
  10:	726f4305 	rsbvc	r4, pc, #335544320	; 0x14000000
  14:	2d786574 	cfldr64cs	mvdx6, [r8, #-464]!	; 0xfffffe30
  18:	0600344d 	streq	r3, [r0], -sp, asr #8
  1c:	094d070d 	stmdbeq	sp, {r0, r2, r3, r8, r9, sl}^
  20:	14041202 	strne	r1, [r4], #-514	; 0xfffffdfe
  24:	17011501 	strne	r1, [r1, -r1, lsl #10]
  28:	1a011803 	bne	4603c <reset_handler+0x45fb4>
  2c:	22061e01 	andcs	r1, r6, #1, 28
  30:	Address 0x0000000000000030 is out of bounds.
```

3.程式執行步驟解析

 程式首先先執行 reset_handler，並先把 ```r7```和 ```lr``` push 進入stack中，根據變數使用，在stack空出一定的區塊。
 
 程式的一開始是先宣告變數，所以先將宣告的變數先放入stack中。
 
 呼叫sum function，帶有2個參數，先將2個參數存入暫存器```r0``` ```r1```中，跳到 sum function 的起始位置。
 
 ```sum```
 
 * 首先會先push ```r7```。
 * 將function 要使用到的stack空間空出來 ，然後將stack pointer 的值給```r7```。
 * 將傳遞的參數放入stack中，等待使用時取出。
 * 將存入的值取出放入```r2``` ```r3```中，並做加法，存入```r3```中。
 * 將```r3```的值放到stack中。
 * 最後取出在stack中的值放入```r3```，然後將要return的值放到```r0```中。
 * 將使用的stack區域釋放，並將```r7```的值給```sp```。
 * branch 回到 reset_handler。
 
 呼叫arithmetic function，帶有5個參數，將其一放入stack中，另外4個參數存入暫存器```r0``` ```r1``` ```r2``` ```r3```中，跳到arithmetic function 的起始位置。(因為要傳遞的參數大於四個，所以暫存器會不夠用，所以會先將其中的一個放入stack中)
 
```arithmetic```

首先會先push ```r7```。

將function 要使用到的stack空間空出來 ，將stack pointer 的值給```r7```。

將儲存在暫存器 ```r0``` ```r1``` ```r2``` ```r3``` 傳遞的參數放入到stack中，等待使用時取出。

將存入在stack中的值取出，並對其做函式指定的四則運算。

再取出被放在stack中傳遞的參數(第五個參數)，繼續做四則運算。

把最後的結果給```r3```並放到stack中。

從stack中取出值後，最後將要return的值放到```r0```中。

將使用的stack區域釋放，並將```r7```的值給```sp```。

branch 回到 reset_handler。

呼叫pointer function，因為使用指標，所以先在暫存器```r1``` ```r2``` ```r3``` 放入三個address(一開始宣告變數時的那三個address)。

將address搬移到 ```r0``` ```r1``` ```r2```。

```pointer```

首先會先push ```r7```。

將function 要使用到的stack空間空出來 ，將stack pointer 的值給```r7```。

將儲存在暫存器 ```r0``` ```r1``` ```r2``` 傳遞的參數放入到stack中，等待使用時取出。

先將存放在stack中的位址取出放到暫存器，然後再將在那個位址的值取出放入暫存器中。

做運算並將最後結果放入```r3```，再放入stack。

最後將結果取出放入```r0```中return。

將使用的stack區域釋放，並將```r7```的值給```sp```。

branch 回到 reset_handler。


4.觀察結果
1) 當主程式要呼叫function時，會先將```r7```和 ```lr``` push 進入stack中
2) 根據AAPCS，傳遞參數會先使用```r0``` ```r1``` ```r2``` ```r3```，如果不夠用會使用到stack。
3) 執行function時，會先push ```r7```，空出stack空間，並將stack pointer 的值給```r7```。
4) 傳遞參數的參數會先被 store 到 stack 中。
5) 要使用傳遞的參數時，先將參數從 stack load 出來，放入暫存器再使用。
6) 運算時最後的結果會被放到```r3```中，再 store 到 stack 中。
7) 要回傳參數時，會先把在 stack 中的結果 load 到```r3```，在把```r3```的值給```r0```進行回傳。
8) 使用pointer傳遞參數時，會把要傳遞的位址放入暫存器中。
9) 使用pointer接收參數時，會把接收到的位址先放到暫存器中，再將該位址的值存到暫存器中。
