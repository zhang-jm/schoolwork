/*
 * Filename: swapChars.s
 * Description: Assembly function that swaps two chars and determines how many
 *              unique characters had the same value
 */ 
	.section	".text"
	.global	swapChars

/*
 * Function name: swapChars()
 * Function prototype: int swapChars(char* c1, char* c2)
 * Description: Takes two pointers to chars and swaps the values.  Then compares
 *              values and ptrs to determine if the characters were the same and,
 *              if so, if the ptrs were pointing to the same place.
 * Parameters: %i0 - first char to swap
 *             %i1 - second char to swap
 * Side Effects: Swaps two characters in memory
 * Return Value: 0 if chars were different
 *               1 if chars were same, but ptrs were to the same address
 *               2 if chars were same and ptrs were to different addresses
 */

swapChars:
	save	%sp, -96, %sp

	ldub	[%i0], %l0	! load first char
	ldub	[%i1], %l1	! load second char

	stb	%l0, [%i1]	! store first char in second spot
	stb	%l1, [%i0]	! store second char in first spot

	mov	%g0, %l3	! tentatively store 0 for return value

	cmp	%l0, %l1	! if first char and second char aren't same,
	bne	notSame		! return 0
	nop

	cmp	%i0, %i1	! chars are same; if ptrs are the same, return 1
	be	samePtr
	nop

	mov	2, %l3		! chars are same, ptrs different -- return 2

	ba	notSame
	nop

samePtr:

	mov	1, %l3		! chars are same, ptrs are same -- return 1

notSame:

	mov	%l3, %i0

	ret
	restore
