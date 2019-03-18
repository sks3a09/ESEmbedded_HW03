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

2.實驗步驟
---
1. 設計測試程式 main.c ，從 _start 開始後依序執行

```
Disassembly of section .mytext:

00000000 <sum-0x8>:
   0:	20000100 	andcs	r0, r0, r0, lsl #2
   4:	0000005d 	andeq	r0, r0, sp, asr r0

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

0000005c <reset_handler>:
  5c:	b580      	push	{r7, lr}
  5e:	b082      	sub	sp, #8
  60:	af02      	add	r7, sp, #8
  62:	200a      	movs	r0, #10
  64:	2114      	movs	r1, #20
  66:	f7ff ffcf 	bl	8 <sum>
  6a:	2305      	movs	r3, #5
  6c:	9300      	str	r3, [sp, #0]
  6e:	2001      	movs	r0, #1
  70:	2102      	movs	r1, #2
  72:	2203      	movs	r2, #3
  74:	2304      	movs	r3, #4
  76:	f7ff ffd7 	bl	28 <arithmetic>
  7a:	e7fe      	b.n	7a <reset_handler+0x1e>

Disassembly of section .comment:

00000000 <.comment>:
   0:	3a434347 	bcc	10d0d24 <reset_handler+0x10d0cc8>
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
  28:	1a011803 	bne	4603c <reset_handler+0x45fe0>
  2c:	22061e01 	andcs	r1, r6, #1, 28
  30:	Address 0x0000000000000030 is out of bounds.


```
```c
int sum(int a,int b){
	int c = a+b;
	return c;
}
int arithmetic(int a,int b,int c,int d,int e){
	int f = (a+b)+(c*d)-e;
	return f;
}
void reset_handler(void)
{
	sum(10,20);
	arithmetic(1,2,3,4,5);
	/*int a1,a2,a3,a4,a5;
	a1=10;
	a2=20;
	a3=30;
	a4=40;
	a5=50;
	arithmetic(a1,a2,a3,a4,a5);*/
	while (1)
		;
}
```
