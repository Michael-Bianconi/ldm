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
		case NEGATION: return "~";
		case MATERIAL_CONDITIONAL: return ">";
		case MATERIAL_BICONDITIONAL: return "=";
		default: return "?";
	}
}

/// ===========================================================================
/// Sentence function definitions
/// ===========================================================================

Sentence Sentence_createAtomic(const char var)
{
	Sentence sentence = malloc(sizeof(struct Sentence_s));
	sentence->type = ATOMIC;
	sentence->op = NO_OP;
	sentence->left.variable = var;
	sentence->right.variable = '\0';

	return sentence;
}

Sentence Sentence_createCompound(
	const SentenceOperator op,
	const Sentence left,
	const Sentence right)
{
	Sentence sentence = malloc(sizeof(struct Sentence_s));
	sentence->type = COMPOUND;
	sentence->op = op;
	sentence->left.sentence = left;
	sentence->right.sentence = right;
	return sentence;
}

Sentence Sentence_createNegated(const Sentence toNegate)
{
	Sentence sentence = malloc(sizeof(struct Sentence_s));
	sentence->type = COMPOUND;
	sentence->op = NEGATION;
	sentence->left.sentence = toNegate;
	sentence->right.sentence = NULL;

	return sentence;
}

void Sentence_free(Sentence sentence)
{
	free(sentence);
}

/**
void Sentence_freeRecursive(Sentence sentence)
{
	printf("Freeing: ");
	Sentence_print(sentence);
	printf("\n");
	if (sentence->type == COMPOUND)
	{
		Sentence_freeRecursive(sentence->left.sentence);

		if (sentence->op != NEGATION)
		{
			Sentence_freeRecursive(sentence->right.sentence);
		}
	}

	free(sentence);
}
*/

void Sentence_print(const Sentence sentence)
{
	if (sentence->op == NEGATION)
	{
		printf("~");
		Sentence_print(sentence->left.sentence);
		fflush(stdout);
		return;
	}

	if (sentence->type == ATOMIC)
	{
		printf("%c",sentence->left.variable);
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