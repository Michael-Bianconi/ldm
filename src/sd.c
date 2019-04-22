/**
 * @author Michael-Bianconi
 * @since 04-22-2019
 *
 * Source code for SD rules.
 */

#include "sentence.h"
#include "sd.h"
#include <stdlib.h>

/// ===========================================================================
/// Function definitions
/// ===========================================================================

uint8_t SD_canEliminateConditional(const Sentence sent, const SentenceSet set)
{
	// Sentence must be a conditional
	if (sent->op != MATERIAL_CONDITIONAL
		&& sent->op != MATERIAL_BICONDITIONAL)
			return 0;

	// Check antecedent
	if (SentenceSet_contains(set, sent->left.sentence))
		return 1;

	// If conditional, check consequent as well
	if (sent->op == MATERIAL_BICONDITIONAL
		&& SentenceSet_contains(set, sent->left.sentence))
			return 1;

	// Conditional cannot be eliminated
	return 0;
}

void SD_eliminateConjunction(const Sentence source, SentenceSet destination)
{
	// Sentence must be conjunctive
	if (source->op != AND) return;

	// Add both sides to the set
	SentenceSet_add(destination, source->left.sentence);
	SentenceSet_add(destination, source->right.sentence);
}