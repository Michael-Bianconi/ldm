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
#include <assert.h>

/// ===========================================================================
/// Static functions
/// ===========================================================================

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
 * @return Returns a non malloc'd string of size strlen(string)-2.
 *         Returns NULL if invalid params.
 */
static char* _removeMatchingParen(const char* string, size_t index)
{
	if (index >= strlen(string)) return NULL;
	if (string[index] != ')' && string[index] != '(') return NULL;

	int8_t dir = string[index] == '(' ? 1 : -1; // direction to search
	int8_t numParens = 0; // if 0, then matching paren found
	size_t start = index;
	char* copy = malloc(strlen(string)+1); // string holding result


	// search string
	while (index < strlen(string))
	{
		if (string[index] == '(') numParens++;
		else if (string[index] == ')') numParens--;
		if (numParens == 0 || (index == 0 && dir < 0)) break;
		index += dir;
	}

	// Loop exited without finding the matching paren
	if (numParen != 0) return NULL;

	size_t end = index;

	// put start before end
	if (end < start && end != start) {
		end ^= start;
		start ^= end;
		end ^= start;
	}

	// Copy everything excluding start and end
	strncpy(copy, string, start);
	strncpy(copy+start, string+start+1, end-start-1);
	strncpy(copy+end-1, string+end+1, strlen(string)-end);

	return copy;
}

/// ===========================================================================
/// Function definitions
/// ===========================================================================

// TODO
Sentence Sentence_parseString(const char* in, SentenceSet* set)
{
	char* string = "012(3((()))45)678";
	printf("String: %s\n", string);
	char* paren = _removeMatchingParen(string, 3);
	char* p2 = _removeMatchingParen(string, 13);

	assert(p2);

	printf("%s\n", paren);
	printf("%s\n", p2);

	free(paren);
	return NULL;
}

