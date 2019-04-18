# ldm
Logic Derivation Machine parses logic sentences and performs derivations

# Rules

Sentences may be either <i>atomic</i> or <i>COMPOUND</i>. Atomic sentences
do not contain any operators, and are represented with characters (e.g. a).

Compound sentences hold (usually) two sentences connected by an operator.
The inner sentences may themselves be atomic or compound. In the case of
a unary operator (e.g. negation), only the <i>left</i> inner sentence is
used.