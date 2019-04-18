/**
 * @author Michael Bianconi
 * @since 04-18-2019
 */

#ifndef SENTENCE_H
#define SENTENCE_H

#include <stdlib.h>
#include <stdint.h>

/// ===========================================================================
/// Definitions
/// ===========================================================================
#define SENTENCESET_BUFFER 5

/// ===========================================================================
/// Structure declarations
/// ===========================================================================

struct Sentence_s;

/// ===========================================================================
/// Enum definitions
/// ===========================================================================

/**
 * Sentences have two different types: atomic and compound. Atomic sentences
 * contain only one component, a variable. Compound sentences contain
 * two components, each of which may themselves be either atomic or compound.
 * Note: there is a special type of Sentence that has the negation operator.
 * this unary operator is the only operator that will form a compound sentence
 * with only one inner sentence.
 */
enum SentenceType
{
	ATOMIC,
	COMPOUND
};

enum SentenceOperator
{
	NO_OP,
	AND,
	OR,
	MATERIAL_CONDITIONAL,
	MATERIAL_BICONDITIONAL
};

/// ===========================================================================
/// Union definitions
/// ===========================================================================

/**
 * Holds either a variable or a sentence pointer. Atomic sentences hold
 * variables, compound sentences hold sentence pointers.
 */
union SentenceBuffer
{
	char* variable;
	struct Sentence_s* sentence;
};

/// ===========================================================================
/// Structure definitions
/// ===========================================================================

/**
 * Note: sentences that only make use of one buffer (atomic sentences)
 * will always only use the left buffer.
 */
struct Sentence_s
{
	uint8_t negated;
	enum SentenceType type;
	enum SentenceOperator op;
	union SentenceBuffer left;
	union SentenceBuffer right;
};

/**
 * Since sentences are essentially graphs, freeing them correctly
 * can be difficult. Each time a Sentence is created, add it to
 * a set, then free the whole set at once using SentenceSet_free().
 */
struct SentenceSet_s
{
	size_t size;
	size_t buffer;
	struct Sentence_s** sentences;
};

/// ===========================================================================
/// Typedefs
/// ===========================================================================

typedef enum SentenceType SentenceType;
typedef enum SentenceOperator SentenceOperator;
typedef union SentenceBuffer SentenceBuffer;
typedef struct Sentence_s* Sentence;
typedef struct SentenceSet_s* SentenceSet;

/// ===========================================================================
/// Function declarations - Constructors
/// ===========================================================================

/**
 * Constructs an atomic sentence with no operator and a single variable
 * in the left buffer.
 *
 * @param var Variable to set.
 * @param negated 1 if sentence is negated, 0 otherwise.
 * @return Returns a malloc'd Sentence.
 */
Sentence Sentence_createAtomic(const char* var, const uint8_t negated);

/**
 * Creates a compound sentence.
 *
 * @param op Operator to use. Should not be NEGATION.
 * @param left The sentence to store in the left buffer.
 * @param right The sentence to store in the right buffer.
 * @param negated 1 if the sentence is negated, 0 otherwise.
 * @return Returns a malloc'd sentence.
 */
Sentence Sentence_createCompound(
	const SentenceOperator op,
	const Sentence left,
	const Sentence right,
	const uint8_t negated);

/**
 * Creates a new SentenceSet with an arbitrary length buffer
 * defined by SENTENCESET_BUFFER.
 *
 * @return Returns a malloc'd SentenceSet.
 */
SentenceSet SentenceSet_create();

/// ===========================================================================
/// Function declarations - Destructors
/// ===========================================================================

/**
 * Frees the given sentence but <i>not</i> it's left and right
 * buffers.
 *
 * @param sentence Sentence to free.
 */
void Sentence_free(Sentence sentence);

/**
 * Frees all sentences (and the set itself).
 *
 * @param set Set to free.
 */
void SentenceSet_free(SentenceSet set);

/// ===========================================================================
/// Function declarations - Accessors
/// ===========================================================================

/**
 * Adds the Sentence to the set, if it doesn't already exist in
 * the set.
 *
 * @param set Set to add to.
 * @param sentence Sentence being added.
 */
void SentenceSet_add(SentenceSet set, const Sentence sentence);

/**
 * Checks if the given sentence exists in the set, either directly (having
 * the same address) or indirectly (having the same components).
 *
 * @param set Set to check.
 * @param sentence Sentence to check.
 * @return 1 if exists, 0 otherwise.
 */
uint8_t SentenceSet_contains(const SentenceSet set, const Sentence sentence);

/// ===========================================================================
/// Function declarations - Utility
/// ===========================================================================

/**
 * Recursively checks if the two sentences have the same
 * structure and children.
 *
 * @param a First Sentence.
 * @param b Second Sentence.
 * @return Returns 1 if equals, 0 otherwise.
 */
uint8_t Sentence_equals(const Sentence a, const Sentence b);

/**
 * Recursively prints the sentence such that
 * all inner sentences are also printed.
 *
 * @param sentence Sentence to print.
 */
void Sentence_print(const Sentence sentence);

/**
 * Generate a Sentence from the given character array.
 *
 * @param in Character array to read from.
 * @param set Set buffer, should be NULL or uninitialized.
 * @return Returns the <i>root</i> sentence.
 */
Sentence Sentence_parseString(const char* in, SentenceSet* set);

/**
 * Prints every sentence in the set.
 *
 * @param set Set to print.
 */
void SentenceSet_print(const SentenceSet set);


#endif