/**
 * @author Michael Bianconi
 * @since 04-18-2019
 *
 * Source code for SentenceSets.
 */

#include "sentence.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/// ===========================================================================
/// Function definitions - Constructors
/// ===========================================================================

SentenceSet SentenceSet_create()
{
	SentenceSet set = malloc(sizeof(struct SentenceSet_s));
	set->size = 0;
	set->buffer = SENTENCESET_BUFFER;
	set->sentences = calloc(SENTENCESET_BUFFER, sizeof(Sentence));
	return set;
}

/// ===========================================================================
/// Function definitions - Destructors
/// ===========================================================================

void SentenceSet_free(SentenceSet set)
{
	for (size_t n = 0; n < set->size; n++)
	{
		Sentence_free(set->sentences[n]);
	}

	free(set->sentences);
	free(set);
}

/// ===========================================================================
/// Function definitions - Accessors
/// ===========================================================================

void SentenceSet_add(SentenceSet set, const Sentence sentence)
{
	// Check it hasn't been added already
	for (size_t n = 0; n < set->size; n++)
	{
		// Compare pointers
		if (set->sentences[n] == sentence)
		{
			return;
		}
	}

	// Check buffer is big enough
	if (set->size == set->buffer)
	{
		set->buffer *= 2;
		set->sentences = realloc(set->sentences, set->buffer*sizeof(Sentence));
	}

	// Add sentence to the set
	set->sentences[set->size++] = sentence;
}

uint8_t SentenceSet_contains(const SentenceSet set, const Sentence sentence)
{
	for (size_t n = 0; n < set->size; n++)
	{
		Sentence other = set->sentences[n];
		if (sentence == other || Sentence_equals(other, n)) return 1;
	}

	return 0;
}

/// ===========================================================================
/// Function definitions - Utility
/// ===========================================================================

void SentenceSet_print(const SentenceSet set)
{
	for (size_t n = 0; n < set->size; n++)
	{
		Sentence_print(set->sentences[n]);
		printf("\n");
	}
}