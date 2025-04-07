#include <src/data/data.h>

// Tokens
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

const CharToTokenKind CharToQuantifierTokenKindMap[QUANTIFIER_COUNT] = {
    { '+', TOKEN_KIND_QUANTIFIER_ONE_OR_MORE },
    { '*', TOKEN_KIND_QUANTIFIER_ZERO_OR_MORE },
    { '?', TOKEN_KIND_QUANTIFIER_ZERO_OR_ONE },
    { '.', TOKEN_KIND_QUANTIFIER_ONE },
};

const CharToTokenKind CharToSymbolTokenKindMap[SYMBOL_COUNT] = {
    { '(', TOKEN_KIND_OPEN_PAREN },
    { ')', TOKEN_KIND_CLOSE_PAREN },
    { '|', TOKEN_KIND_OPERATOR_OR },
};

// Nodes
const char *NodeToString[NODE_KIND_COUNT] = {
    "NodeKindAlternation",
    "NodeKindConcatenation",
    "NodeKindRepitition",
};

// Tokens-Nodes
const TokenKindToQuantifier TokenKindToQuantifierMap[QUANTIFIER_COUNT] = {
    { TOKEN_KIND_QUANTIFIER_ONE_OR_MORE, QUANTIFIER_ONE_OR_MORE},
    { TOKEN_KIND_QUANTIFIER_ZERO_OR_MORE, QUANTIFIER_ZERO_OR_MORE},
    { TOKEN_KIND_QUANTIFIER_ZERO_OR_ONE, QUANTIFIER_ZERO_OR_ONE},
    { TOKEN_KIND_QUANTIFIER_ONE, QUANTIFIER_ONE},
};