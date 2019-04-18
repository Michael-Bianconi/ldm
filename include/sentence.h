/**
 * @author Michael Bianconi
 * @since 04-18-2019
 */

#ifndef SENTENCE_H
#define SENTENCE_H

#include <stdint.h>

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
	NEGATION,
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
 * Note: sentences that only make use of one buffer (atomic sentences and
 * negated sentences) will always only use the left buffer.
 */
struct Sentence_s
{
	enum SentenceType type;
	enum SentenceOperator op;
	union SentenceBuffer left;
	union SentenceBuffer right;
};

/// ===========================================================================
/// Typedefs
/// ===========================================================================

typedef enum SentenceType SentenceType;
typedef enum SentenceOperator SentenceOperator;
typedef union SentenceBuffer SentenceBuffer;
typedef struct Sentence_s* Sentence;

/// ===========================================================================
/// Function declarations - Constructors
/// ===========================================================================

/**
 * Constructs an atomic sentence with no operator and a single variable
 * in the left buffer.
 *
 * @param var Variable to set.
 * @return Returns a malloc'd Sentence.
 */
Sentence Sentence_createAtomic(const char* var);

/**
 * Creates a compound sentence.
 *
 * @param op Operator to use. Should not be NEGATION.
 * @param left The sentence to store in the left buffer.
 * @param right The sentence to store in the right buffer.
 * @return Returns a malloc'd sentence.
 */
Sentence Sentence_createCompound(
	const SentenceOperator op,
	const Sentence left,
	const Sentence right);

/**
 * Constructs a negated sentence, similar to an atomic sentence but
 * with a sentence instead of a variable. Note: this will
 * create a <i>new</i> sentence without altering the given one.
 *
 * @param toNegate The sentence to negate.
 * @return Returns a new negated sentence with the given sentence
 *         in the left buffer.
 */
Sentence Sentence_createNegated(const Sentence toNegate);

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

/// ===========================================================================
/// Function declarations - Utility
/// ===========================================================================

/**
 * Recursively prints the sentence such that
 * all inner sentences are also printed.
 *
 * @param sentence Sentence to print.
 */
void Sentence_print(const Sentence sentence);

#endif