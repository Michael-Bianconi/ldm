/**
 * @author Michael Bianconi
 * @since 04-18-2019
 *
 * Unit testing for Sentences
 */

#include "sentence.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

static void _TEST_CREATE_ATOMIC()
{
	// Creating an atomic sentence
	Sentence atomic = Sentence_createAtomic("a", 0);
	assert(atomic->type == ATOMIC);
	assert(atomic->op == NO_OP);
	assert(atomic->negated == 0);
	assert(strcmp(atomic->right.variable, "\0") == 0);
	assert(strcmp(atomic->left.variable, "a") == 0);
	Sentence_free(atomic);

	// Creating a negated atomic sentence
	Sentence neg = Sentence_createAtomic("b", 1);
	assert(neg->type == ATOMIC);
	assert(neg->op == NO_OP);
	assert(neg->negated == 1);
	assert(strcmp(neg->right.variable, "\0") == 0);
	assert(strcmp(neg->left.variable, "b") == 0);
	Sentence_free(neg);

	printf("_TEST_CREATE_ATOMIC() : SUCCESS\n");
}

static void _TEST_CREATE_COMPOUND()
{
	// Creating some atomic sentences
	Sentence atomic = Sentence_createAtomic("a",0);
	Sentence neg = Sentence_createAtomic("b",1);

	// Creating the compound sentence
	Sentence compound = Sentence_createCompound(OR, atomic, neg, 0);
	assert(compound->type == COMPOUND);
	assert(compound->op == OR);
	assert(compound->left.sentence == atomic);
	assert(compound->right.sentence == neg);
	assert(compound->negated == 0);

	// Creating a negated compound sentence
	Sentence compound2 = Sentence_createCompound(OR, atomic, neg, 1);
	assert(compound2->type == COMPOUND);
	assert(compound2->op == OR);
	assert(compound2->left.sentence == atomic);
	assert(compound2->right.sentence == neg);
	assert(compound2->negated == 1);

	// Free resources
	Sentence_free(atomic);
	Sentence_free(neg);
	Sentence_free(compound);
	Sentence_free(compound2);

	printf("_TEST_CREATE_COMPOUND() : SUCCESS\n");
}

static void _TEST_BIG_COMPOUND()
{
	Sentence atom1 = Sentence_createAtomic("a1",0);
	Sentence atom2 = Sentence_createAtomic("b2",0);
	Sentence neg1 = Sentence_createAtomic("c3",1);
	Sentence comp1 = Sentence_createCompound(AND, atom2, neg1,0);
	Sentence atom3 = Sentence_createAtomic("d4",0);
	Sentence comp2 = Sentence_createCompound(OR, atom3, comp1,0);

	Sentence_free(atom1);
	Sentence_free(atom2);
	Sentence_free(atom3);
	Sentence_free(neg1);
	Sentence_free(comp1);
	Sentence_free(comp2);

	printf("_TEST_BIG_COMPOUND() : SUCCESS\n");
}

static void _TEST_SET()
{
	Sentence atom1 = Sentence_createAtomic("a",0);
	Sentence atom2 = Sentence_createAtomic("b",0);
	Sentence neg1 = Sentence_createAtomic("c",1);
	Sentence comp1 = Sentence_createCompound(AND, atom2, neg1,0);
	Sentence atom3 = Sentence_createAtomic("d",0);
	Sentence comp2 = Sentence_createCompound(OR, atom3, comp1,1);
	SentenceSet set = SentenceSet_create();
	SentenceSet_add(set, atom1);
	SentenceSet_add(set, atom2);
	SentenceSet_add(set, neg1);
	SentenceSet_add(set, comp1);
	SentenceSet_add(set, comp2);
	SentenceSet_add(set, atom3);
	//SentenceSet_print(set);
	SentenceSet_free(set);

	printf("_TEST_SET() : SUCCESS\n");

}

static void _TEST_SENTENCE_EQUALS()
{
	// Create standard atomic sentences
	Sentence a1 = Sentence_createAtomic("a", 0);
	Sentence a2 = Sentence_createAtomic("a", 0);
	Sentence b1 = Sentence_createAtomic("b", 1);
	Sentence b2 = Sentence_createAtomic("b", 1);

	assert(Sentence_equals(a1, a2));
	assert(Sentence_equals(a2, a1));
	assert(Sentence_equals(b1, b2));
	assert(Sentence_equals(b2, b1));
	assert(!Sentence_equals(a1, b1));
	assert(!Sentence_equals(b2, a2));

	// Create some compound sentences
	Sentence c1 = Sentence_createCompound(AND, a1, b1, 0);
	Sentence c2 = Sentence_createCompound(AND, a1, b1, 0);
	Sentence d1 = Sentence_createCompound(OR, a2, b1, 1);
	Sentence d2 = Sentence_createCompound(MATERIAL_CONDITIONAL, b1, b1, 0);

	assert(Sentence_equals(c1, c2));
	assert(Sentence_equals(c2, c1));
	assert(Sentence_equals(c1, c1));
	assert(Sentence_equals(c2, c2));
	assert(!Sentence_equals(d1, b1));
	assert(!Sentence_equals(d2, c1));

	// Free resources
	Sentence_free(a1);
	Sentence_free(a2);
	Sentence_free(b1);
	Sentence_free(b2);
	Sentence_free(c1);
	Sentence_free(c2);
	Sentence_free(d1);
	Sentence_free(d2);

	printf("_TEST_SENTENCE_EQUALS() : SUCCESS\n");

}

static void _TEST_SENTENCESET_CONTAINS()
{
	// Create standard atomic sentences
	Sentence a1 = Sentence_createAtomic("a", 0);
	Sentence a2 = Sentence_createAtomic("a", 0);
	Sentence b1 = Sentence_createAtomic("b", 1);
	Sentence b2 = Sentence_createAtomic("b", 1);

	// Create some compound sentences
	Sentence c1 = Sentence_createCompound(AND, a1, b1, 0);
	Sentence c2 = Sentence_createCompound(AND, a1, b1, 0);
	Sentence d1 = Sentence_createCompound(OR, a2, b1, 1);
	Sentence d2 = Sentence_createCompound(MATERIAL_CONDITIONAL, b1, b1, 0);

	SentenceSet set = SentenceSet_create();
	SentenceSet_add(set,a1);
	SentenceSet_add(set,a2);
	SentenceSet_add(set,b1);
	SentenceSet_add(set,b2);
	SentenceSet_add(set,c1);
	SentenceSet_add(set,c2);
	SentenceSet_add(set,d1);

	assert(SentenceSet_contains(set, a1));
	assert(SentenceSet_contains(set, a2));
	assert(SentenceSet_contains(set, b2));
	assert(SentenceSet_contains(set, c1));
	assert(SentenceSet_contains(set, d1));
	assert(!SentenceSet_contains(set, d2));

	SentenceSet_free(set);
	Sentence_free(d2);

	printf("_TEST_SENTENCESET_CONTAINS() : SUCCESS\n");
}

static void _TEST_SENTENCE_PARSE(char* in)
{
	SentenceSet set = SentenceSet_create();
	Sentence_print(Sentence_parseString(in,&set));
	printf("\n");
	SentenceSet_print(set);
	SentenceSet_free(set);

	printf("_TEST_SENTENCE_PARSE() : SUCCESS\n");
}

int main(int argc, char** argv)
{
	(void) argc;
	(void) argv;
	_TEST_CREATE_ATOMIC();
	_TEST_CREATE_COMPOUND();
	_TEST_BIG_COMPOUND();
	_TEST_SET();
	_TEST_SENTENCE_EQUALS();
	_TEST_SENTENCESET_CONTAINS();
	_TEST_SENTENCE_PARSE(argv[1]);
}