/*
 * Filename: findEnd.s
 * Description: Assembly function that traverses a string and returns the
 *              length of the string and stores a pointer to the last char.
 */ 

	.section	".text"
	.global	findEnd

/*
 * Function name: findEnd()
 * Function prototype: int findEnd(char* str, char** endPtr)
 * Description: Takes in a string and a char**.  Traverses string until it
 *              finds the null character.  Stores pointer to the last char
 *              of the string in the endPtr parameter. Returns strLength.
 * Parameters: %i0 - string to find length of
 *             %i1 - char** endPtr, to store ptr to last char, for use in
 *                   calling function
 * Side Effects: Stores ptr to last char of str in char** endPtr
 * Return Value: length of string
 */

findEnd:

	save    %sp, -96, %sp

	mov	%g0, %l1	! strLength = 0

	mov	%i0, %l4	! endChar = strPtr

	ldub	[%i0], %l0	! *strPtr

	cmp	%l0, %g0	! if *strPtr == '\0', this is empty string
	be	emptyStr	! skip loop to find last char
	nop

checkChar:

	add	%i0, 1, %i0	! increment strPtr to look at next char
	add	%l1, 1, %l1	! increment strLength

	ldub	[%i0], %l0	! *strPtr

	cmp	%l0, %g0	! if *strPtr == '\0', end loop
	bne	checkChar
	nop

	add	%i0, -1, %l4	! adjust strPtr to point back to last char

emptyStr:

	st	%l4, [%i1]	! store ptr to last char
	mov	%l1, %i0	! return strLength

	ret
	restore
