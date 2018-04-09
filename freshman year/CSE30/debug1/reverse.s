/*
 * Filename: reverse.s
 * Description: Assembly function that reverses the characters of a string
 *              and determines whether the string was a palindrome.
 */ 

	.section ".data"
palStr:
	.asciz	"PALINDROME!\n"

	.global	reverse

	.section ".text"

/*
 * Function name: reverse()
 * Function prototype: int reverse(char* str)
 * Description: Takes in a char*, finds the end of the string, and traverses
 *              the string, swapping characters. At the end, if all swapped
 *              characters matched, returns 1 to indicate it was a palindrome.
 * Parameters: %i0 - string to reverse
 * Side Effects: Prints message if the string is a palindrome
 * Return Value: 0 indicating string not a palindrome
 *               1 indicating string is a palindrome
 * Registers Used: %l0 - strLength
 *                 %l1 - startPtr
 *                 %l2 - endPtr
 */

reverse:
	save	%sp, -(92 + 4) & -8, %sp

	/* Find length of str and ptr to last char */
	mov	%i0, %o0	! ptr to string as first arg to findEnd
	add	%fp, -4, %o1	! ptr to store ptr to last char as second arg
	call	findEnd
	nop

	mov	%o0, %l0	! strLength

	mov	%i0, %l1	! startPtr
	ld	[%fp - 4], %l2	! endPtr

	cmp	%l1, %l2	! while startPtr <= endPtr, swap chars
	bg	endSwapLoop
	nop

swapLoop:

	mov	%l1, %o0	! startPtr as first arg to swapChars
	mov	%l2, %o1	! endPtr as second arg to swapChars
	call	swapChars	! swap 'em!
	nop

	mov	%o0, %l3	! number of chars that were the same
	sub	%l0, %l3, %l0	! subtract number of same chars from strLength

	add	%l1, 1, %l1	! increment startPtr
	sub	%l2, 1, %l2	! decrement endPtr

	cmp	%l1, %l2	! while startPtr <= endPtr, swap chars
	ble	swapLoop
	nop

endSwapLoop:

	mov	%g0, %i0	! tentatively set 0 as return value

	cmp	%l0, %g0
	bne	not_palindrome	! if strLength == 0, then all chars were matched
	nop

	set	palStr, %o0
	call	printf
	nop

	mov	1, %i0		! set 1 as return value

not_palindrome:

	ret
	restore
