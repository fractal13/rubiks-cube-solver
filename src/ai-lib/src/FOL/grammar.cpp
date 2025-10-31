// /*
// *
// * <Sentence> -> <Sentence0>
// *
// * <Sentence0> -> <Sentence1> <Sentence0Tail>
// * <Sentence0Tail> -> TOKEN_IFF <Sentence1> <Sentence0Tail>
// * <Sentence0Tail> -> {empty}
// *
// * <Sentence1> -> <Sentence2> <Sentence1Tail>
// * <Sentence1Tail> -> TOKEN_IMPLIES <Sentence2> <Sentence1Tail>
// * <Sentence1Tail> -> {empty}
// *
// * <Sentence2> -> <Sentence3> <Sentence2Tail>
// * <Sentence2Tail> -> TOKEN_OR <Sentence3> <Sentence2Tail>
// * <Sentence2Tail> -> {empty}
// *           
// * <Sentence3> -> <Sentence4> <Sentence3Tail>
// * <Sentence3Tail> -> TOKEN_AND <Sentence4> <Sentence3Tail>
// * <Sentence3Tail> -> {empty}
// *           
// * <Sentence4> -> TOKEN_NOT <Sentence4>
// * <Sentence4> -> <Sentence5>
// *           
// * <Sentence5> -> <AtomicSentence>
// * <Sentence5> -> TOKEN_LEFT_PAREN <Sentence0> TOKEN_RIGHT_PAREN
// * <Sentence5> -> TOKEN_FORALL <VariableList> <Sentence0>
// * <Sentence5> -> TOKEN_EXISTS <VariableList> <Sentence0>
// *
// * <AtomicSentence> -> <Identifier> TOKEN_LEFT_PAREN <TermList> TOKEN_RIGHT_PAREN
// * <AtomicSentence> -> <Term> TOKEN_EQUAL <Term>
// *
// * <VariableList> -> <Identifier> <VariableListTail>
// * <VariableListTail> -> TOKEN_COMMA <Identifier> <VariableListTail>
// * <VariableListTail> -> {empty}
// *
// * <TermList> -> <Term> <TermListTail>
// * <TermListTail> -> TOKEN_COMMA <Term> <TermListTail>
// * <TermListTail> -> <Term>
// *
// * <Term> -> <Identifier> TOKEN_LEFT_PAREN <TermList> TOKEN_RIGHT_PAREN
// * <Term> -> <Identifier>
// *
// * <Identifier> -> TOKEN_SYMBOL          // ([A-Za-z][A-Za-z0-9_]*)
// */
