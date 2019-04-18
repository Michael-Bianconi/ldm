/**
 * @author Michael Bianconi
 * @since 04-18-2019
 *
 * Unit testing for Sentences
 */

#include "sentence.h"
#include <stdio.h>
#include <assert.h>

static void _TEST_CREATE_ATOMIC()
{
	Sentence atomic = Sentence_createAtomic('a');
	assert(atomic->type == ATOMIC);
	assert(atomic->op == NO_OP);
	assert(atomic->right.variable == '\0');
	assert(atomic->left.variable == 'a');
	Sentence_free(atomic);

	printf("_TEST_CREATE_ATOMIC() : SUCCESS\n");
}

static void _TEST_CREATE_NEGATED()
{
	Sentence atomic = Sentence_createAtomic('a');
	Sentence negated = Sentence_createNegated(atomic);
	assert(negated->type == COMPOUND);
	assert(negated->op == NEGATION);
	assert(negated->right.sentence == NULL);
	assert(negated->left.sentence == atomic);
	Sentence_free(atomic);
	Sentence_free(negated);

	printf("_TEST_CREATE_NEGATED() : SUCCESS\n");
}

static void _TEST_CREATE_COMPOUND()
{
	Sentence atomic = Sentence_createAtomic('a');
	Sentence negated = Sentence_createNegated(atomic);
	Sentence compound = Sentence_createCompound(OR, atomic, negated);
	assert(compound->type == COMPOUND);
	assert(compound->op == OR);
	assert(compound->left.sentence == atomic);
	assert(compound->right.sentence == negated);
	Sentence_free(atomic);
	Sentence_free(negated);
	Sentence_free(compound);

	printf("_TEST_CREATE_COMPOUND() : SUCCESS\n");
}

static void _TEST_BIG_COMPOUND()
{
	Sentence atom1 = Sentence_createAtomic('a');
	Sentence atom2 = Sentence_createAtomic('b');
	Sentence neg1 = Sentence_createNegated(atom1);
	printf("reached 1\n");
	Sentence comp1 = Sentence_createCompound(AND, atom2, neg1);
	printf("reached 3\n");
	Sentence atom3 = Sentence_createAtomic('c');
	printf("reached 2\n");
	Sentence comp2 = Sentence_createCompound(OR, atom3, comp1);
	Sentence_print(comp2);
	printf("\n");
	Sentence_free(atom1);
	Sentence_free(atom2);
	Sentence_free(atom3);
	Sentence_free(neg1);
	Sentence_free(comp1);
	Sentence_free(comp2);
}


int main(int argc, char** argv)
{
	(void) argc;
	(void) argv;
	_TEST_CREATE_ATOMIC();
	_TEST_CREATE_NEGATED();
	_TEST_CREATE_COMPOUND();
	_TEST_BIG_COMPOUND();

}