/**
 * @author Michael Bianconi
 * @since 04-21-2019
 */

#include "sentence.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// ===========================================================================
/// Static functions
/// ===========================================================================

/**
 * Checks if the character is an operator. ~ not included.
 *
 * @param c Char to check.
 * @return Returns the operator, or NO_OP if not op.
 */
static SentenceOperator _getOperator(const char c)
{
	switch (c)
	{
		case '&': return AND;
		case 'v': return OR;
		case '>': return MATERIAL_CONDITIONAL;
		case '=': return MATERIAL_BICONDITIONAL;
		default: return NO_OP;
	}
}

/**
 * Given a sentence, find the index of its operator. If none exist, 0
 * is returned.
 *
 * a >&< b
 * a >&< (b & c)
 * (a & b) >&< (c & d)
 * ((a & b) & c) >&< (d & (e & (f & g)))
 */
static int _getMainOperatorIndex(const char* in)
{
	uint8_t numParens = 0;

	for (size_t n = 0; n < strlen(in); n++)
	{
		if (in[n] == '(') numParens++;
		if (in[n] == ')') numParens--;
		SentenceOperator op = _getOperator(in[n]);
		if (numParens == 0 && op != NO_OP) return n;
	}

	return -1;
}

/**
 * Copies the substrings to the left and right of the given index into
 * the two given buffers. The character at the split is omitted.
 *
 * @param in Sentence to split.
 * @param idx Index to split on.
 * @param left Left buffer.
 * @param right Right buffer.
 */
void _split(const char* in, const size_t idx, char** left, char** right)
{
	*left = calloc(idx+1,1);
	*right = calloc(strlen(in)-idx+1,1);
	strncpy(*left, in, idx);
	strncpy(*right, in+idx+1, strlen(in)-idx-1);
}

/// ===========================================================================
Sentence Sentence_parseString(const char* in, SentenceSet* set)
{
	// Get the index of the main connective
	int opIdx = _getMainOperatorIndex(in);

	// Get whether or not the sentence is negated
	uint8_t negated = in[0] == '~';

	// If opIdx == -1, then the sentence is only a variable
	if (opIdx == -1) return _parseAtomic(in, set, negated);

	// else, split on the index
	char* leftIn;
	char* rightIn;
	_split(in, opIdx, &leftIn, &rightIn);
	printf("Left: '%s'\nRight: '%s'\n",leftIn,rightIn);
	Sentence left = Sentence_parseString(leftIn, set);
	Sentence right = Sentence_parseString(rightIn, set);
	SentenceOperator op = _getOperator(in[opIdx]);
	Sentence compound = Sentence_createCompound(op, left, right, negated);
	return compound;
}
