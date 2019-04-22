/**
 * @author Michael Bianconi
 * @since 04-18-2019
 *
 *
 * Conjunction introduction (&I)
 * Conjunction elimination (&E)
 * Disjunction introduction (vI)
 * Conditional elimination (>E)
 * Biconditional elimination (=E)
 */

#ifndef SD_H
#define SD_H

#include "sentence.h"

/// ===========================================================================
/// Function declarations
/// ===========================================================================


/**
 * Checks if (bi)conditional elimination can be performed. Elimination
 * can be performed if the antecedent (left sentence) exists within
 * the given set.
 *
 * @pre sent Sent is either material conditional or material biconditional.
 * @param sent Sentence to check.
 * @param set Set to check.
 *
 */
uint8_t SD_canEliminateConditional(const Sentence sent, const SentenceSet set);


/**
 * Adds both the left and right sides to the given set.
 *
 * @pre source is a conjunctive sentence.
 * @param source Source sentence.
 * @param destination Set to add to.
 */
void SD_eliminateConjunction(const Sentence source, SentenceSet destination);