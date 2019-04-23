/**
 * @author Michael Bianconi
 * @since 04-22-2019
 *
 * Derivation is the process by which a set of sentences
 * lead to another sentence.
 */

#include "sentence.h"

/// ===========================================================================
/// Structure definitions
/// ===========================================================================

struct Derivation_s
{
	SentenceSet assumptions;
	Sentence conclusion;
};

/// ===========================================================================
/// Typedefs
/// ===========================================================================

typedef struct Derivation_s* Derivation;

/// ===========================================================================
/// Function declarations
/// ===========================================================================

/**
 * Returns a malloc'd Derivation.
 *
 * @param assumptions Sentences assumed to be true.
 * @param conclusion The sentence to derive to.
 * @return Returns a malloc'd Derivation.
 */
Derivation Derivation_create(SentenceSet assumptions, Sentence conclusion);

/**
 * Frees the memory allocated for the given Derivation.
 *
 * @param d Derivation to free.
 */
void Derivate_free(Derivation d);

/**
 * Returns the set of steps required to solve the derivation.
 *
 * @param d Derivation to solve.
 * @return Solution steps, or NULL if not found.
 */
SentenceSet Derivation_solve(Derivation d);