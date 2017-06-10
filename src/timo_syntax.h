#ifndef __timo_syntax_H_
#define __timo_syntax_H_

#include "timo_type.h"
#include "timo_tree.h"
#include "timo_map.h"
#include "timo_libc.h"
#include "timo_symbol.h"
#include "timo_lex.h"
#include "timo_scope.h"
#include "timo_synode.h"

//  前置声明
struct timo_proto;

//  语法分析器
struct timo_syntax
{
    timo_scope*     global;     //  全局作用域
    timo_synode*    root;
    timo_lex*       lex;        //  当前词法分析器

    //  当前环境数据
    timo_scope*     scope;      //  当前作用域
    timo_synode*    synode;     //  当前语法节点
    timo_token*     token;      //  当前符号
    timo_proto*     prototype;  //  当前符号的文法构造原型
};

//  token原型定义
struct timo_proto
{
    timo_int        id;         //  id
    timo_int        lbp;        //  优先级
    timo_synode*   (*nud)(timo_proto* self, timo_token* selfData, timo_syntax* synContext);
    timo_synode*   (*led)(timo_proto* self, timo_token* selfData, timo_syntax* synContext, timo_synode* left);
    timo_synode*   (*stmt)(timo_proto* self, timo_token* selfData, timo_syntax* synContext);
    timo_synode*   (*err)(timo_proto* self, timo_token* selfData, timo_syntax* synContext);
};

//  构造和析构
extern  timo_syntax*    timo_syntax_new();
extern  timo_void       timo_syntax_del(timo_syntax* syntax);

//  核心功能：识别一个表达式以及超前扫描一个符号
extern timo_synode*     timo_syntax_expr(timo_syntax* syntax, timo_int rbp);
extern timo_symbol*     timo_syntax_advance(timo_syntax* syntax, timo_int expectTokenId);

//  获取当前的一个符号
extern timo_token*      timo_syntax_token(timo_syntax* syntax);

//  作用域操作
extern timo_scope*      timo_syntax_pop_scope(timo_syntax* syntax);
extern timo_scope*      timo_syntax_push_scope(timo_syntax* syntax);

//  符号管理
extern timo_void        timo_syntax_def_symbol(timo_syntax* scope, timo_symbol* symbol);
extern timo_symbol*     timo_syntax_get_symbol(timo_syntax* scope, timo_char* key);

#endif//__timo_syntax_H_
