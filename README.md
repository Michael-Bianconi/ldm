# ldm
Logic Derivation Machine parses logic sentences and performs derivations

# Sentences

Sentences may be either atomic or compound. Atomic sentences consist solely
of a variable, and possible a negation. Compound sentences consist of two
inner sentences, connected by an operator.

# Operators

| Name | Description | Symbol |
|:----:|:-----------:|:------:|
|Negation|NOT P|\~|
|Conjunction|P AND Q|&|
|Disjunction|P OR Q|v|
|Material Conditional|If P then Q|>|
|Material Biconditional|If and only if P then Q|=|
 
# Sentence Sets

Since sentences are essentially graphs with interconnected nodes,
it may be useful to add them to a SentenceSet to help with
memory freeing.

# Sentence Parsing

![Parsing example](https://github.com/Michael-Bianconi/ldm/blob/master/ldmParsing.png)