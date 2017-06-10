#ifndef __timo_token_H_
#define __timo_token_H_

#include "timo_type.h"

enum
{
    TK_MIN      =   -1,     //  符号的最小值
#define TOKEN1(name,id,desc,rbp,nud,led,stmt,err)
#define TOKEN2(name,id,desc,rbp,nud,led,stmt,err)   name = id,
#include "timo_token_table.h"
#undef  TOKEN1
#undef  TOKEN2
    TK_MAX      =   300,
};



typedef struct 
{
    timo_void*      proto;      //  原型
    timo_int        id;         //  id
    timo_int        cap;        //  容量
    timo_int        len;        //  长度
    timo_char*      data;       //  数据
}timo_token;



//  创建，销毁，重置
extern timo_token*      timo_token_new(timo_int size);
extern timo_void        timo_token_del(timo_token* token);
extern timo_void        timo_token_clear(timo_token* token);
extern timo_token*      timo_token_clone(timo_token* token);
extern timo_char*       timo_token_desc(timo_int id);

//  文本符号获取和添加
extern timo_char*       timo_token_text_get(timo_token* token);
extern timo_void        timo_token_text_put(timo_token* token, timo_char* name, timo_int size);

//  作为错误描述，名字，字符串,注释
extern timo_int         timo_token_text(timo_token* token, timo_int id);
extern timo_int         timo_token_error(timo_token* token, timo_int eid, ...);
#define timo_token_name(token)      (timo_token_text(token, TK_NAME))
#define timo_token_str(token)       (timo_token_text(token, TK_CSTRING))
#define timo_token_comment(token)   (timo_token_text(token, TK_COMMENT))
#define timo_token_number(token)    (timo_token_text(token, TK_NUMBER))

//  作为运算符
extern timo_void        timo_token_opr(timo_token* token, timo_int id);




#endif//__timo_token_H_

