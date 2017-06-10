#include "timo_token.h"
#include "timo_type.h"
#include "timo_libc.h"
#include "timo_error.h"

timo_token*  timo_token_new(timo_int size)
{
    size = (size > 0)?size:1;
    timo_token* token = (timo_token*)timo_malloc(sizeof(timo_token));
    token->proto    = TIMO_NULL;
    token->id       = 0;
    token->len      = 0;
    token->cap      = 0;
    token->data     = (timo_char*)timo_malloc(size);
    return (token);
}

timo_void    timo_token_del(timo_token* token)
{
    if (TIMO_NULL == token)
    {
        return;
    }

    timo_free(token);
}
// 
// timo_void    timo_token_set(timo_token* token, timo_int id, ...)
// {
//     const timo_char* fmt = "";
//     switch (id)
//     {
// #define DEF_ERR(name,id,cn)    case name:  fmt = cn;   break;
// #include "timo_error_table.h"
// #undef  DEF_ERR
//     }
// 
//     timo_char buffer[1024];
//     va_list valist;
//     va_start(valist, id);
//     timo_int size = timo_vsnprintf(buffer, sizeof(buffer), fmt, valist);
//     va_end(valist);
//     if (size <= 0)
//     {
//         size      = 0;
//         buffer[0] = '\0';
//     }
// 
//     //  补齐最后一个\0字符
//     size += 1;
// 
//     //  自动扩容
//     if (size > token->size)
//     {
//         timo_char*  newBuffer = (timo_char*)timo_malloc(size);
//         if (TIMO_NULL == token->buffer)
//         {
//             timo_free(token->buffer);
//         }
// 
//         token->buffer = newBuffer;
//         token->size = size;
//         token->len = 0;
//     }
// 
//     //  保存错误数据
//     timo_memcpy(token->buffer, buffer, size - 1);
//     token->len = size - 1;
// }

timo_void    timo_token_clear(timo_token* error)
{
    if (TIMO_NULL == error)
    {
        return;
    }

    error->id       = TK_NONE;
    error->len      = 0;
    error->data[0]  = '\0';
}

timo_int             timo_token_error(timo_token* token, timo_int eid, ...)
{
    token->id = TK_ERROR;
    va_list valist;
    va_start(valist, eid);
    timo_int size = timo_error_formatv(token->data, token->cap - 1, eid, valist);
    va_end(valist);

    token->data[size] = '\0';
    token->len = size;

    return (TK_ERROR);
}

timo_token*          timo_token_clone(timo_token* token)
{
    return (TIMO_NULL);
}

timo_int         timo_token_text(timo_token* token, timo_int id)
{
    token->id   = id;
    token->len  = 0;
    return (id);
}

timo_char*       timo_token_text_get(timo_token* token)
{
    if (0 == token->len)
    {
        return ("");
    }

    if ('\0' != token->data[token->len])
    {
        token->data[token->len] = '\0';
    }

    return (token->data);
}

timo_void        timo_token_text_put(timo_token* token, timo_char* name, timo_int size)
{
    timo_char* end = name + size;
    timo_int newSize = token->len + size + 1;
    if (newSize > token->cap)
    {
        timo_char* newBuffer = (timo_char*)timo_malloc(newSize);
        if (token->len > 0)
        {
            timo_memcpy(newBuffer, token->data, token->len);
            timo_free(token->data);
        }

        token->data = newBuffer;
        token->cap  = newSize;
    }

    timo_memcpy(token->data + token->len, name, size);
    token->len += size;
}


timo_char*       timo_token_desc(timo_int id)
{
    switch (id)
    {                       //  以下为分隔符
    case TK_ERROR    :  return "(error)";
    case TK_END      :  return "(end)";
    case '{':   return "{";
    case '}':   return "}";
    case '[':   return "[";
    case ']':   return "]";
    case '(':   return "(";
    case ')':   return ")";
    case ';':   return ";";
    case ',':   return ",";
    case '.':   return ".";
    case ':':   return ":";
                            //  以下为数学运算符
    case '+':   return "+";
    case '-':   return "-";
    case '*':   return "*";
    case '/':   return "/";
    case '%':   return "%";
    case '!':   return "!";
    case '~':   return "~";
    case '&':   return "&";
    case '|':   return "|";
    case '^':   return "^";
    case '<':   return "<";
    case '>':   return ">";
    case '=':   return "=";
        //  以下为组合运算符

    case TK_NT_EQ:  return "!=";
    case TK_EQ_EQ:  return "==";
    case TK_LT_EQ:  return "<=";
    case TK_LSHFT:  return "<<";
    case TK_LG_EQ:  return ">=";
    case TK_RSHFT:  return ">>";
    case TK_AD_AD:  return "&&";
    case TK_OR_OR:  return "||";
    case TK_RANGE:  return "..";
    case TK_ELLIP:  return "...";

    case TK_NAME     :  return "(name)";
    case TK_CSTRING  :  return "(cstring)";
    case TK_NUMBER   :  return "(number)";
    case TK_IF       :  return "if";
    case TK_ELSE     :  return "else";
    case TK_FOR      :  return "for";
    case TK_SELECT   :  return "select";
    case TK_PUBLIC   :  return "public";
    case TK_PRIVATE  :  return "private";
    case TK_PROTECTED:  return "protected";
    case TK_FUNC     :  return "function";
    case TK_CLASS    :  return "class";
    case TK_INTERFACE:  return "interface";
    case TK_IMPORT   :  return "import";
    case TK_VAR      :  return "var";
    case TK_BREAK    :  return "break";
    case TK_CONTINUE :  return "continue";
    case TK_RETURN   :  return "return";
    case TK_COMMENT  :  return "(#comment)";
    default:            return "(unknown)";
    };
}