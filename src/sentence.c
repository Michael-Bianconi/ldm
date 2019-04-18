/**
 * @author Michael Bianconi
 * @since 04-18-2019
 *
 * Source code for sentence.h.
 */

#include "sentence.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/// ===========================================================================
/// SentenceOperator function definitions
/// ===========================================================================

char* SentenceOperator_toString(SentenceOperator op)
{
	switch (op)
	{
		case NO_OP: return "";
		case AND: return "&";
		case OR: return "v";
		case MATERIAL_CONDITIONAL: return ">";
		case MATERIAL_BICONDITIONAL: return "=";
		default: return "?";
	}
}

/// ===========================================================================
/// Sentence function definitions
/// ===========================================================================

Sentence Sentence_createAtomic(const char* var, const uint8_t negated)
{
	Sentence sentence = malloc(sizeof(struct Sentence_s));
	sentence->type = ATOMIC;
	sentence->op = NO_OP;
	sentence->left.variable = malloc(strlen(var) + 1);
	strcpy(sentence->left.variable, var);
	sentence->right.variable = "\0";
	sentence->negated = negated;

	return sentence;
}

Sentence Sentence_createCompound(
	const SentenceOperator op,
	const Sentence left,
	const Sentence right,
	const uint8_t negated)
{
	Sentence sentence = malloc(sizeof(struct Sentence_s));
	sentence->type = COMPOUND;
	sentence->op = op;
	sentence->left.sentence = left;
	sentence->right.sentence = right;
	sentence->negated = negated;
	return sentence;
}

void Sentence_free(Sentence sentence)
{
	if (sentence->type == ATOMIC) free(sentence->left.variable);
	free(sentence);
}

/// ===========================================================================
/// Function definitions - Utility
/// ===========================================================================

uint8_t Sentence_equals(const Sentence a, const Sentence b)
{
	// Check for type and operator
	if (a->type != b->type || a->op != b->op || a->negated != b->negated)
		return 0;

	// Check for atomic sentences
	else if (a->type == ATOMIC)
	{
		if (strcmp(a->left.variable, b->left.variable)!=0)
			return 0;
	}
	else
	{
		// Check left sentence and right sentences
		if (!Sentence_equals(a->left.sentence, b->left.sentence))
			return 0;
		if (!Sentence_equals(a->right.sentence, b->right.sentence))
			return 0;

	}

	return 1;
}


void Sentence_print(const Sentence sentence)
{
	if (sentence->negated)
	{
		printf("~");
		fflush(stdout);
	}

	if (sentence->type == ATOMIC)
	{
		printf("%s",sentence->left.variable);
		fflush(stdout);
		return;
	}

	else
	{
		printf("(");
		Sentence_print(sentence->left.sentence);
		printf(" %s ", SentenceOperator_toString(sentence->op));
		Sentence_print(sentence->right.sentence);
		printf(")");
		fflush(stdout);
	}
}