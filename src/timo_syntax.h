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

//  ǰ������
struct timo_proto;

//  �﷨������
struct timo_syntax
{
    timo_scope*     global;     //  ȫ��������
    timo_synode*    root;
    timo_lex*       lex;        //  ��ǰ�ʷ�������

    //  ��ǰ��������
    timo_scope*     scope;      //  ��ǰ������
    timo_synode*    synode;     //  ��ǰ�﷨�ڵ�
    timo_token*     token;      //  ��ǰ����
    timo_proto*     prototype;  //  ��ǰ���ŵ��ķ�����ԭ��
};

//  tokenԭ�Ͷ���
struct timo_proto
{
    timo_int        id;         //  id
    timo_int        lbp;        //  ���ȼ�
    timo_synode*   (*nud)(timo_proto* self, timo_token* selfData, timo_syntax* synContext);
    timo_synode*   (*led)(timo_proto* self, timo_token* selfData, timo_syntax* synContext, timo_synode* left);
    timo_synode*   (*stmt)(timo_proto* self, timo_token* selfData, timo_syntax* synContext);
    timo_synode*   (*err)(timo_proto* self, timo_token* selfData, timo_syntax* synContext);
};

//  ���������
extern  timo_syntax*    timo_syntax_new();
extern  timo_void       timo_syntax_del(timo_syntax* syntax);

//  ���Ĺ��ܣ�ʶ��һ�����ʽ�Լ���ǰɨ��һ������
extern timo_synode*     timo_syntax_expr(timo_syntax* syntax, timo_int rbp);
extern timo_symbol*     timo_syntax_advance(timo_syntax* syntax, timo_int expectTokenId);

//  ��ȡ��ǰ��һ������
extern timo_token*      timo_syntax_token(timo_syntax* syntax);

//  ���������
extern timo_scope*      timo_syntax_pop_scope(timo_syntax* syntax);
extern timo_scope*      timo_syntax_push_scope(timo_syntax* syntax);

//  ���Ź���
extern timo_void        timo_syntax_def_symbol(timo_syntax* scope, timo_symbol* symbol);
extern timo_symbol*     timo_syntax_get_symbol(timo_syntax* scope, timo_char* key);

#endif//__timo_syntax_H_
