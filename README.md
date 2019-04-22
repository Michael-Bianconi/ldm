# ldm
Logic Derivation Machine parses logic sentences and performs derivations

# Rules

Sentences may be either <i>atomic</i> or <i>COMPOUND</i>. Atomic sentences
do not contain any operators, and are represented with characters (e.g. a).

Compound sentences hold (usually) two sentences connected by an operator.
The inner sentences may themselves be atomic or compound. In the case of
a unary operator (e.g. negation), only the <i>left</i> inner sentence is
used.

# SentenceSets

SentenceSets are simply lists of sentences, structured such that
sentences can be freed from memory.

It is recommended to have two (or more) memory sets: one that holds
<i>all</i> sentences, and one that holds working sentences (i.e.
sentences that have been assumed, or proven true). The reason for this
is that if `(A & B) v (C & D)` is assumed, then `(A & B) v (C & B)`,
`(A & B)` and `(C & D)` will all exist in the set. However, neither
`(A & B)` nor `(C & D)` can be assumed to be true. In this case, it
would be wise to copy `(A & B) v (C & B)` into an "assumption" set
that contains only that sentence.

#Sentence Parsing

![Parsing example](https://github.com/Michael-Bianconi/ldm/blob/master/ldmParsing.png)