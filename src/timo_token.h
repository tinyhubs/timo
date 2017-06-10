#ifndef __timo_token_H_
#define __timo_token_H_

#include "timo_type.h"

enum
{
    TK_MIN      =   -1,     //  ���ŵ���Сֵ
#define TOKEN1(name,id,desc,rbp,nud,led,stmt,err)
#define TOKEN2(name,id,desc,rbp,nud,led,stmt,err)   name = id,
#include "timo_token_table.h"
#undef  TOKEN1
#undef  TOKEN2
    TK_MAX      =   300,
};



typedef struct 
{
    timo_void*      proto;      //  ԭ��
    timo_int        id;         //  id
    timo_int        cap;        //  ����
    timo_int        len;        //  ����
    timo_char*      data;       //  ����
}timo_token;



//  ���������٣�����
extern timo_token*      timo_token_new(timo_int size);
extern timo_void        timo_token_del(timo_token* token);
extern timo_void        timo_token_clear(timo_token* token);
extern timo_token*      timo_token_clone(timo_token* token);
extern timo_char*       timo_token_desc(timo_int id);

//  �ı����Ż�ȡ�����
extern timo_char*       timo_token_text_get(timo_token* token);
extern timo_void        timo_token_text_put(timo_token* token, timo_char* name, timo_int size);

//  ��Ϊ�������������֣��ַ���,ע��
extern timo_int         timo_token_text(timo_token* token, timo_int id);
extern timo_int         timo_token_error(timo_token* token, timo_int eid, ...);
#define timo_token_name(token)      (timo_token_text(token, TK_NAME))
#define timo_token_str(token)       (timo_token_text(token, TK_CSTRING))
#define timo_token_comment(token)   (timo_token_text(token, TK_COMMENT))
#define timo_token_number(token)    (timo_token_text(token, TK_NUMBER))

//  ��Ϊ�����
extern timo_void        timo_token_opr(timo_token* token, timo_int id);




#endif//__timo_token_H_

