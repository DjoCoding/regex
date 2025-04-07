#include <src/data/data.h>

const char *TokenKindToString[TOKEN_KIND_COUNT] = {
    "TokenKindEnd",
    "TokenKindUnit",
    "TokenKindOpenParen",
    "TokenKindCloseParen",
    "TokenKindOperatorOR",
    "TokenKindQuantifierOneOrMore",
    "TokenKindQuantifierZeroOrMore",
    "TokenKindQuantifierZeroOrOne",
    "TokenKindQuantifierOne",
};


const CharToTokenKind CharToQuantifierTokenKindMap[] = {
    { '+', TOKEN_KIND_QUANTIFIER_ONE_OR_MORE },
    { '*', TOKEN_KIND_QUANTIFIER_ZERO_OR_MORE },
    { '?', TOKEN_KIND_QUANTIFIER_ZERO_OR_ONE },
    { '.', TOKEN_KIND_QUANTIFIER_ONE },
};

const CharToTokenKind CharToSymbolTokenKindMap[] = {
    { '(', TOKEN_KIND_OPEN_PAREN },
    { ')', TOKEN_KIND_CLOSE_PAREN },
};