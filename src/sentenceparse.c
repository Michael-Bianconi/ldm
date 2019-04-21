/**
 * @author Michael Bianconi
 * @since 04-18-2019
 *
 * Source code that handles parsing strings into sentences.
 */

#include "sentence.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

/// ===========================================================================
/// Static functions
/// ===========================================================================

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
 * Given a string and the index of a paren, copy it into a new character array
 * with the opening and closing parenthesis removed. If the index points
 * to a '(', search right and remove a ')', or vice versa if the index
 * points to a ')'.
 *
 * @pre string[index] must equal '(' or ')'
 * @pre the paren must have a matching paren
 * @pre the string must be null-terminated
 * @param string String to remove from
 * @param index Index of the paren
 * @return Returns a malloc'd string of size strlen(string)-2.
 *         Returns NULL if invalid params.
 */
static char* _removeMatchingParen(const char* string, size_t start)
{
	size_t end = _getMatchingParen(string, start);
	if (end == start) return NULL; // matching paren not found

	// put start before end
	if (end < start) {
		end ^= start;
		start ^= end;
		end ^= start;
	}

	// Copy everything excluding start and end
	char* copy = malloc(strlen(string)+1);
	strncpy(copy, string, start);
	strncpy(copy+start, string+start+1, end-start-1);
	strncpy(copy+end-1, string+end+1, strlen(string)-end);

	return copy;
}

/**
 * Returns the substring contained within the matching parenthesis,
 * excluding the parenthesis.
 *
 * @pre string[index] must equal '(' or ')'
 * @pre the paren must have a matching paren
 * @pre the string must be null-terminated
 * @param string String to remove from
 * @param index Index of the paren
 * @return Returns a malloc'd string of size strlen(string)-2.
 *         Returns NULL if invalid params.
 */
static char* _getInner(const char* string, size_t start)
{
	size_t end = _getMatchingParen(string, start);
	if (end == start) return NULL; // matching paren not found

	// put start before end
	if (end < start) {
		end ^= start;
		start ^= end;
		end ^= start;
	}

	// Copy everything excluding start and end
	char* copy = malloc(end - start);
	strncpy(copy, string+start+1, end-start-1);

	return copy;
}

/**
 * Given a string beginning with a variable, return the variable
 * string.
 *
 * @param string String to read.
 * @param index Index to look at.
 * @return Returns the variable.
 */
static char* _getVariable(const char* string, size_t index)
{
	// The variable's length may be the same as the given string
	char* var = calloc(strlen(string) + 1, 1);
	for (size_t n = index; n < strlen(string); n++)
	{
		if (string[n] == ' ' || string[n] == '\0')
		{
			var[n-index] = '\0';
			return var;
		}
		var[n-index] = string[n];
	}
	var[strlen(string)-1] = '\0';
	return var;
}

/**
 * Checks the character. If it is ')' or '\0' then it is the end of
 * the sentence.
 *
 * @param c Char to check.
 * @return 1 if end, 0 otherwise.
 */
static uint8_t _endOfSentence(const char c)
{
	return c == ')' || c == '\0';
}

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
 * Searches through string for next operator and returns it.
 * Fills the given buffer with all characters right of the
 * of the operator.
 *
 * @pre buffer must be of sufficient size.
 * @param in String to search.
 * @param buffer Buffer to hold the remaining string.
 * @return Returns the operator.
 */
static SentenceOperator _cropToOperator(const char* in, char* buffer)
{
	size_t n = 0;
	SentenceOperator op = NO_OP;

	while (in[n])
	{
		op = _getOperator(in[n]);
		if (op != NO_OP) break;
		n++;
	}

	strcpy(buffer, in + n + 1);
	return op;
}

/// ===========================================================================
/// Function definitions
/// ===========================================================================

// TODO
Sentence Sentence_parseString(const char* in, SentenceSet* set)
{
	if (strlen(in) == 0) // end of string
	{
		return NULL;
	}

	if (isalnum(in[0]))
	{
		char* var = _getVariable(in, 0);
		Sentence atomic = Sentence_createAtomic(var, 0);

		// end of sentence means this is atomic
		if (_endOfSentence(in[strlen(var)]))
		{
			free(var);
			return atomic;
		}

		// not end of sentence, compound
		else
		{
			char buffer[strlen(in)+1];
			SentenceOperator op = _cropToOperator(in,buffer);
			Sentence right = Sentence_parseString(buffer,set);
			return Sentence_createCompound(op, atomic, right, 0);
		}
	}

	if (in[0] == '~')
	{
		if (in[1] != '(')
		{
			char* var = _getVariable(in, 0);
			Sentence atomic = Sentence_createAtomic(var, 0);
			//atomic->negated = 1;

			// end of sentence means this is atomic
			if (_endOfSentence(in[strlen(var)]))
			{
				free(var);
				return atomic;
			}

			// not end of sentence, compound
			else
			{
				char buffer[strlen(in)+1];
				SentenceOperator op = _cropToOperator(in,buffer);
				printf("Buffer = %s\n",buffer);
				Sentence right = Sentence_parseString(buffer,set);
				return Sentence_createCompound(op, atomic, right, 0);
			}
		}
		else
		{
			char* inner = _getInner(in, 1);
			Sentence compound = Sentence_parseString(inner, set);
			compound->negated = 1;
			free(inner);
			return compound;
		}
	}

	if (in[0] == '(')
	{
		char* inner = _getInner(in, 0);
		Sentence innerSentence = Sentence_parseString(inner, set);
		free(inner);
		return innerSentence;
	}

	if (in[0] == ' ')
	{
		return Sentence_parseString(in+1, set);
	}



	return NULL;
}

