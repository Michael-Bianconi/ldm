/**
 * @author Michael Bianconi
 * @since 04-18-2019
 *
 * Source code for SentenceSets.
 */

#include "sentence.h"
#include <stdlib.h>
#include <stdint.h>

SentenceSet SentenceSet_create()
{
	SentenceSet set = malloc(sizeof(struct SentenceSet_s));
	set->size = 0;
	set->buffer = SENTENCESET_BUFFER;
	set->sentences = calloc(SENTENCESET_BUFFER, sizeof(Sentence));
	return set;
}

void SentenceSet_free(SentenceSet set)
{
	for (size_t n = 0; n < set->size; n++)
	{
		Sentence_free(set->sentences[n]);
	}

	free(set->sentences);
	free(set);
}

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