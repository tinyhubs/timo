#include "timo_syntax.h"
#include "timo_symbol.h"
#include "timo_limit.h"
#include "timo_scope.h"

static timo_proto  protos[(TK_MAX - TK_MIN) + 1] = {{0}};

timo_proto* timo_proto_get(timo_int tokenId)
{
    return (protos + (tokenId - TK_MIN));
}

timo_void       timo_prototype_init()
{
#define TOKEN1(tname,tid,tdesc,tlbp,tnud,tled,tstmt,terr)   \
    protos[(tid) - TK_MIN].id   = (tid);                 \
    protos[(tid) - TK_MIN].lbp  = (tlbp);                \
    protos[(tid) - TK_MIN].nud  = (tnud);                \
    protos[(tid) - TK_MIN].led  = (tled);                \
    protos[(tid) - TK_MIN].stmt = (tstmt);               \
    protos[(tid) - TK_MIN].err  = (terr);
#define TOKEN2  TOKEN1
#include "timo_token_table.h"
#undef  TOKEN1
#undef  TOKEN2
}

timo_syntax*    timo_syntax_new()
{
    timo_syntax* syntax = (timo_syntax*)timo_malloc(sizeof(timo_syntax));
    syntax->global      = timo_scope_new();
    syntax->root        = timo_synode_new(TIMO_NULL);
    syntax->lex         = timo_lex_new(TIMO_CACHE_SIZE, TIMO_PREFETCH_SIZE, TIMO_TOKEN_INIT_SIZE);
    syntax->scope       = syntax->global;
    syntax->synode      = syntax->root;
    syntax->token       = TIMO_NULL;
    syntax->token->proto= timo_proto_get(TK_NONE);
    return (syntax);
}

timo_void       timo_syntax_del(timo_syntax* syntax)
{
    timo_scope_del(syntax->global);

    timo_lex_del(syntax->lex);
}

timo_synode*    timo_syntax_expr(timo_syntax* syntax, timo_int rbp)
{
    timo_token*     t   = syntax->token;
    timo_proto* pt  = syntax->prototype;

    timo_syntax_advance(syntax, TK_NONE);
    timo_synode* left = (*(pt->nud))(pt, t, syntax);
    while (rbp < pt->lbp)
    {
        t   = syntax->token;
        pt  = syntax->prototype;

        timo_syntax_advance(syntax, TK_NONE);
        left = (*(pt->led))(pt, t, syntax, left);
    }

    return left;
}

timo_symbol*    timo_syntax_advance(timo_syntax* syntax, timo_int expectTokenId)
{
    if (TIMO_NULL == syntax)
    {
        return (TIMO_NULL);
    }

    timo_int tokenId = timo_lex_next(syntax->lex);

    //  如果发生错误
    if (tokenId < 0)
    {
        return (TIMO_NULL);
    }

    if (expectTokenId == tokenId)
    {

    }

    syntax->token = timo_lex_token(syntax->lex);
    syntax->prototype = &(protos[tokenId - TK_MIN]);

    timo_symbol* symbol = timo_symbol_new();
    //symbol->type
    return (TIMO_NULL);
}
/*
timo_synode* advance(timo_int id)
{
    var a, o, t, v;
    if (id && token.id !== id)
    {
        token.error("Expected '" + id + "'.");
    }
    if (token_nr >= tokens.length)
    {
        token = symbo_table["(end)"];
        return;
    }
    t = tokens[token_nr];
    token_nr += 1;
    v = t.value;
    a = t.type;
    if (a === "name")
    {
        o = scope.find(v);
    } 
    else
    if (a === "operator")
    {
        o = symbol_table[v];
        if (!o)
        {
            t.error("Unknown operator.");
        }
    }
    else 
    if (a === "string" || a === "number")
    {
        a = "literal";
        o = symbol_table["(literal)"];
    }
    else 
    {
        t.error("Unexpected token.");
    }

    token = object(o);
    token.value = v;
    token.arity = a;
    return token;
};
*/
timo_scope*    timo_syntax_pop_scope(timo_syntax* syntax)
{
    return (syntax->scope = timo_scope_pop(syntax->scope));
}

timo_scope*    timo_syntax_push_scope(timo_syntax* syntax)
{
    return (syntax->scope = timo_scope_push(syntax->scope, timo_scope_new()));
}


