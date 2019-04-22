/**
 * @author Michael Bianconi
 * @since 04-21-2019
 */

#include "sentence.h"
#include "trimit.h"
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

/**
 * Given a string and the index of a paren, return the index of the
 * matching paren.
 *
 * @pre string[index] must equal '(' or ')'
 * @pre the paren must have a matching paren
 * @pre the string must be null-terminated
 * @param string String to search
 * @param index Index of the paren
 * @return Returns the index of the matching paren, or the given index
 *         if not found.
 */
static size_t _getMatchingParen(const char* string, size_t index)
{
	if (index >= strlen(string)) return index;
	if (string[index] != ')' && string[index] != '(') return index;

	int8_t dir = string[index] == '(' ? 1 : -1; // direction to search
	int8_t numParens = 0; // if 0, then matching paren found

	// search string
	while (index < strlen(string))
	{
		if (string[index] == '(') numParens++;
		else if (string[index] == ')') numParens--;
		if (numParens == 0 || (index == 0 && dir < 0)) break;
		index += dir;
	}

	// Loop exited without finding the matching paren
	if (numParens != 0) return index;

	return index;
}


/**
 * Checks if the entire string is enclosed in one set of parens.
 */
static uint8_t _isEnclosed(const char* in)
{
	if (strlen(in) == 0) return 0;
	if (in[0] != '(') return 0;
	size_t match = _getMatchingParen(in, 0);
	return match == strlen(in)-1;
}


/**
 * Checks if the <i>entire</i> string is negated.
 */
static uint8_t _isNegated(char* in)
{
	if (strlen(in) == 0) return 0; // empty string
	if (in[0] != '~') return 0; // not negated
	if (in[1] != '(') return 1; // not compound
	return _isEnclosed(in+1); // sentence is enclosed in parens
}


/// ===========================================================================
Sentence Sentence_parseString(char* in, SentenceSet* set)
{
	// Ignore whitespace
	if (in[0] == ' ') return Sentence_parseString(in+1, set);

	//  Trim whitespace from the end
	if (in[strlen(in)-1] == ' ')
	{
		in[strlen(in)-1] = '\0';
		return Sentence_parseString(in, set);
	}

	// If enclosed, remove parens
	if (_isEnclosed(in))
	{
		in[strlen(in)-1] = '\0';
		return Sentence_parseString(in+1, set);
	}

	// If negated, flag and recurse
	if (_isNegated(in))
	{
		Sentence negated = Sentence_parseString(in+1, set);
		negated->negated = 1;
		SentenceSet_add(*set, negated);
		return negated;
	}

	// Get the index of the main connective
	int opIdx = _getMainOperatorIndex(in);

	// If atomic, return atomic sentence
	if (opIdx == -1)
	{
		Sentence atomic = Sentence_createAtomic(in, 0);
		SentenceSet_add(*set, atomic);
		return atomic;
	}

	// If compound, split into left and right and recurse
	char* leftIn;
	char* rightIn;
	_split(in, opIdx, &leftIn, &rightIn);
	Sentence left = Sentence_parseString(leftIn, set);
	Sentence right = Sentence_parseString(rightIn, set);
	SentenceOperator op = _getOperator(in[opIdx]);
	Sentence compound = Sentence_createCompound(op, left, right, 0);
	free(leftIn);
	free(rightIn);
	SentenceSet_add(*set, left);
	SentenceSet_add(*set, right);
	SentenceSet_add(*set, compound);

	return compound;
}
