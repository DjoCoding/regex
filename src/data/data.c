#include <src/data/data.h>

// Tokens
const char *TokenKindToString[TOKEN_KIND_COUNT] = {
    "TokenKindEnd",
    "TokenKindUnit",
    "TokenKindAnyUnit",
    "TokenKindOpenParen",
    "TokenKindCloseParen",
    "TokenKindOperatorOR",
    "TokenKindOpenCurly",
    "TokenKindCloseCurly",
    "TokenKindComma",
    "TokenKindCaret",
    "TokenKindDollar",
    "TokenKindQuantifierOneOrMore",
    "TokenKindQuantifierZeroOrMore",
    "TokenKindQuantifierZeroOrOne",
};

const CharToTokenKind CharToQuantifierTokenKindMap[QUANTIFIER_COUNT] = {
    { '+', TOKEN_KIND_QUANTIFIER_ONE_OR_MORE },
    { '*', TOKEN_KIND_QUANTIFIER_ZERO_OR_MORE },
    { '?', TOKEN_KIND_QUANTIFIER_ZERO_OR_ONE },
};

const CharToTokenKind CharToSymbolTokenKindMap[SYMBOL_COUNT] = {
    { '(', TOKEN_KIND_OPEN_PAREN },
    { ')', TOKEN_KIND_CLOSE_PAREN },
    { '|', TOKEN_KIND_OPERATOR_OR },
    { '.', TOKEN_KIND_ANY_UNIT },
    { '{', TOKEN_KIND_OPEN_CURLY },
    { '}', TOKEN_KIND_CLOSE_CURLY },
    { ',', TOKEN_KIND_COMMA },
    { '^', TOKEN_KIND_CARET },
    { '$', TOKEN_KIND_DOLLAR }
};

// Nodes
const char *NodeToString[NODE_KIND_COUNT] = {
    "NodeKindAlternation",
    "NodeKindConcatenation",
    "NodeKindRepitition",
};

// Tokens-Nodes
const TokenKindToQuantifierKind TokenKindToQuantifierKindMap[QUANTIFIER_COUNT] = {
    { TOKEN_KIND_QUANTIFIER_ONE_OR_MORE, QUANTIFIER_ONE_OR_MORE},
    { TOKEN_KIND_QUANTIFIER_ZERO_OR_MORE, QUANTIFIER_ZERO_OR_MORE},
    { TOKEN_KIND_QUANTIFIER_ZERO_OR_ONE, QUANTIFIER_ZERO_OR_ONE},
};

// Chars
const CharToStringFormat PrintableCharToStringFormatMap[PRINTABLE_CHAR_COUNT] = {
    { '0', "0" },
    { '1', "1" },
    { '2', "2" },
    { '3', "3" },
    { '4', "4" },
    { '5', "5" },
    { '6', "6" },
    { '7', "7" },
    { '8', "8" },
    { '9', "9" },
    { ':', ":" },
    { ';', ";" },
    { '<', "<" },
    { '=', "=" },
    { '>', ">" },
    { '?', "?" },
    { '@', "@" },
    { 'A', "A" },
    { 'B', "B" },
    { 'C', "C" },
    { 'D', "D" },
    { 'E', "E" },
    { 'F', "F" },
    { 'G', "G" },
    { 'H', "H" },
    { 'I', "I" },
    { 'J', "J" },
    { 'K', "K" },
    { 'L', "L" },
    { 'M', "M" },
    { 'N', "N" },
    { 'O', "O" },
    { 'P', "P" },
    { 'Q', "Q" },
    { 'R', "R" },
    { 'S', "S" },
    { 'T', "T" },
    { 'U', "U" },
    { 'V', "V" },
    { 'W', "W" },
    { 'X', "X" },
    { 'Y', "Y" },
    { 'Z', "Z" },
    { 'a', "a" },
    { 'b', "b" },
    { 'c', "c" },
    { 'd', "d" },
    { 'e', "e" },
    { 'f', "f" },
    { 'g', "g" },
    { 'h', "h" },
    { 'i', "i" },
    { 'j', "j" },
    { 'k', "k" },
    { 'l', "l" },
    { 'm', "m" },
    { 'n', "n" },
    { 'o', "o" },
    { 'p', "p" },
    { 'q', "q" },
    { 'r', "r" },
    { 's', "s" },
    { 't', "t" },
    { 'u', "u" },
    { 'v', "v" },
    { 'w', "w" },
    { 'x', "x" },
    { 'y', "y" },
    { 'z', "z" },
};