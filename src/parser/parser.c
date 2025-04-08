#include <src/parser/parser.h>

#include <stdbool.h>

#include <src/data/data.h>
#include <src/error/error.h>
#include <src/shared/shared.h>

Parser parser_new(TokenList tokens) {
    return (Parser) {
        .current = 0,
        .tokens = tokens
    };
}

Token parser_peek(Parser *this) {
    return this->tokens.items[this->current];
}

void parser_consume(Parser *this) {
    this->current++;
}

bool parser_end(Parser *this) {
    return parser_peek(this).kind == TOKEN_KIND_END;
}

Node *parser_parse_node(Parser *this);

Node *parser_parse_group(Parser *this) {
    if(parser_peek(this).kind != TOKEN_KIND_OPEN_PAREN) {
        panic("expected `(` but %s found.", parser_end(this) ? "end" : TokenKindToString[parser_peek(this).kind]);
    }

    parser_consume(this);

    Node *node = parser_parse_node(this);

    if(parser_peek(this).kind != TOKEN_KIND_CLOSE_PAREN) {
        panic("expected `)` but %s found.", parser_end(this) ? "end" : TokenKindToString[parser_peek(this).kind]);
    }

    parser_consume(this);

    return node;
}

Primary parser_parse_pri(Parser *this) {
    if(parser_peek(this).kind == TOKEN_KIND_OPEN_PAREN) {
        Node *group = parser_parse_group(this);
        return primary_new_group(group);
    }

    if(parser_peek(this).kind == TOKEN_KIND_UNIT) {
        char c = parser_peek(this).value.content[0];
        parser_consume(this);
        return primary_new_lit(c);
    }

    if(parser_peek(this).kind == TOKEN_KIND_ANY_UNIT) {
        parser_consume(this);
        return primary_new_any_lit();
    }

    panic("expected a literal or a group but %s found.", parser_end(this) ? "end" : TokenKindToString[parser_peek(this).kind]);
}

size_t parser_parse_number(Parser *this) {
    if(parser_peek(this).kind != TOKEN_KIND_UNIT) {
        panic("expected a number but %s found.", parser_end(this) ? "end" : TokenKindToString[parser_peek(this).kind]);
    }

    Token token = parser_peek(this);
    if(!isnum(token.value.content[0])) {
        panic("expected a number but %c found.", token.value.content[0]);
    }

    size_t result = 0;
    
    while(!parser_end(this)) {
        Token token = parser_peek(this);
        if(token.kind != TOKEN_KIND_UNIT) { break; }
        char c = token.value.content[0];
        if(!isnum(c)) { break; }
        result *= 10;
        result += (size_t)(c - '0');
        parser_consume(this);
    }

    return result;
}

Quantifier parser_parse_quantifier_between(Parser *this) {
    if(parser_peek(this).kind != TOKEN_KIND_OPEN_CURLY) {
        panic("expected { token but %s found.", parser_end(this) ? "end" : TokenKindToString[parser_peek(this).kind]);
    }

    parser_consume(this);

    Quantifier q = {0};
    q.kind = QUANTIFIER_BETWEEN;


    Token token = parser_peek(this);

    if(token.kind == TOKEN_KIND_UNIT) {
        q.as_between.left.value = parser_parse_number(this);
        q.as_between.left.exists = true;
    }

    if(parser_peek(this).kind != TOKEN_KIND_COMMA) {
        panic("expected , token but %s found.", parser_end(this) ? "end" : TokenKindToString[parser_peek(this).kind]);
    }

    parser_consume(this);
    
    if(parser_peek(this).kind == TOKEN_KIND_UNIT) {
        q.as_between.right.value = parser_parse_number(this);
        q.as_between.right.exists = true;
    }

    if(parser_peek(this).kind != TOKEN_KIND_CLOSE_CURLY) {
        panic("expected } token but %s found.", parser_end(this) ? "end" : TokenKindToString[parser_peek(this).kind]);
    }

    parser_consume(this);

    return q;
}

Quantifier parser_parse_quantifier(Parser *this) {
    Token token = parser_peek(this);

    if(token.kind == TOKEN_KIND_OPEN_CURLY) {
        return parser_parse_quantifier_between(this);
    }

    for(size_t i = 0; i < LEN(TokenKindToQuantifierKindMap); ++i) {
        if(token.kind != TokenKindToQuantifierKindMap[i].kind) continue;
        parser_consume(this);
        return (Quantifier) {
            .kind = TokenKindToQuantifierKindMap[i].q
        };
    }
    
    return (Quantifier) {
        .kind = QUANTIFIER_NONE
    };
}

Node *parser_parse_rep(Parser *this) {
    Primary pri = parser_parse_pri(this);
    
    if(parser_end(this)) {
        return node_new_rep(pri, (Quantifier) { .kind = QUANTIFIER_NONE });
    }

    Quantifier q = parser_parse_quantifier(this);
    return node_new_rep(pri, q);
}

Node *parser_parse_concat(Parser *this) {
    Node *lhs = parser_parse_rep(this);
    while (true) {
        if(parser_end(this)) break;
        if(parser_peek(this).kind == TOKEN_KIND_OPERATOR_OR) break;
        if(parser_peek(this).kind == TOKEN_KIND_CLOSE_PAREN) break;

        Node *rhs = parser_parse_rep(this);
        lhs = node_new_concat(lhs, rhs);
    }
    return lhs;
}

Node *parser_parse_alter(Parser *this) {
    Node *lhs = parser_parse_concat(this);
    while(true) {
        if(parser_end(this)) break;
        if(parser_peek(this).kind == TOKEN_KIND_CLOSE_PAREN) break;

        if(parser_peek(this).kind != TOKEN_KIND_OPERATOR_OR) { 
            if(parser_end(this)) { break; }
            panic("expected `|` but %s was found.", TokenKindToString[parser_peek(this).kind]);
         }
         parser_consume(this);

         Node *rhs = parser_parse_concat(this);
         
         lhs = node_new_alter(lhs, rhs);
    }
    return lhs;
}

Node *parser_parse_node(Parser *this) {
    return parser_parse_alter(this);
}

AST parser_parse(Parser *this) {
    Node *root = parser_parse_node(this);
    return ast_new(root); 
}