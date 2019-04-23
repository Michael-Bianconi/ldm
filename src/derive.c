/**
 * @author Michael Bianconi
 * @since 04-23-2019
 *
 * Source code for derive.h
 */

#include "derive.h"
#include "sentence.h"
#include <stdlib.h> // malloc(), free()

/// ===========================================================================
/// Function definitions
/// ===========================================================================

Derivation Derivation_create(SentenceSet assumptions, Sentence conclusion)
{
	Derivation d = malloc(sizeof(struct Derivation_s));
	d->assumptions = assumptions;
	d->conclusion = conclusion;
	return d;
}

void Derivation_free(Derivation d)
{
	free(d);
}

SentenceSet Derivation_solve(Derivation d)
{
	return NULL;
}