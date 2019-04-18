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

void Sentence_freeRecursive(Sentence sentence)
{
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

void Sentence_print(const Sentence sentence)
{
	if (sentence->op == NEGATION)
	{
		printf("~");
		Sentence_print(sentence->left.sentence);
		fflush();
		return;
	}

	if (sentence->type == ATOMIC)
	{
		printf(sentence->left.variable);
		fflush();
		return;
	}

	else
	{
		printf("(");
		Sentence_print(sentence->left.sentence);
		printf(" %c ", SentenceOperator_toString(sentence->op));
		Sentence_print(sentece->right.sentence);
		printf(")");
		fflush();
	}
}