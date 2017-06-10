#include "timo_lex.h"
#include "timo_libc.h"
#include "timo_error.h"
#include "timo_source.h"
#include "timo_token.h"
#include "timo_list.h"
#include "timo_buffer.h"


//! 字符分类
enum
{
    BIN     =   0x0001, ///<    二进制
    OCT     =   0x0002, ///<    八进制
    DEC     =   0x0004, ///<    十进制
    HEX     =   0x0008, ///<    十六进制
    ALPHA   =   0x0010, ///<    可视字符
    SPACE   =   0x0020, ///<    空白
    NEWLINE =   0x0040, ///<    换行
    //...
    OTHER   =   0x8000, ///<    其他
};

struct  timo_state
{
    timo_list       head;

    timo_int        state;
    timo_buffer*    token;
};

//???????????xxxxxxxxxxxxxxxxxxxx*********
//        |  |   |               |       |
//  line--+  |   |               |       |
//           |   |               |       |
//  cache----+   |               |       |
//  cp-----------+               |       |
//  limit------------------------+       |
//  end----------------------------------+
struct timo_cache
{
    timo_list       head;

    //@{ 一个lex的三要素
    timo_source*    source;
    timo_int        cacheSize;
    timo_int        limitSize;
    //@}

    timo_char*      limit;      //  有效数据的结尾处，该字符放的是一个\n
    timo_char*      line;       //  一行的起始位置，该变量并不用来从行首获取数据，而是用来记录一个符号的当前位置
    timo_int        lineno;
    timo_char*      cp;         //  当前扫描到的字符的位置
    timo_char       cache[1];   //  有效的数据区
};

struct timo_lex
{
    timo_int        prefetchSize;   ///<    最大超前扫描多少个字符
    timo_int        limitSize;      ///<    缓冲区总大小

    timo_list       cache;      ///<    cache栈
    timo_list       state;      ///<    state栈

    timo_token*     token;      ///<    当前的token

    timo_cache*     top_cache;  ///<    输入cache的顶部
    timo_cache*     top_state;  ///<    输入state的顶部
};



//  字符属性映射
timo_int    map[] = 
{
    0,                  // 0x00 NUL	Null character( 空字符 )
    0,                  // 0x01 SOH	Start of Header( 标题开始 )
    0,                  // 0x02 STX	Start of Text( 正文开始 )
    0,                  // 0x03 ETX	End of Text( 正文结束 )
    0,                  // 0x04 EOT	End of Transmission( 传输结束 )
    0,                  // 0x05 ENQ	Enquiry( 请求 )
    0,                  // 0x06 ACK	Acknowledgment( 收到通知 / 响应 )
    0,                  // 0x07 BEL	Bell ( 响铃 )
    0,                  // 0x08 BS	Backspace( 退格 )
    SPACE,              // 0x09 HT	Horizontal Tab( 水平制表符 )
    NEWLINE,            // 0x0A LF	Line feed( 换行键 )
    SPACE,              // 0x0B VT	Vertical Tab( 垂直制表符 )
    SPACE,              // 0x0C FF	Form feed( 换页键 )
    SPACE,              // 0x0D CR	Carriage return( 回车键 )
    0,                  // 0x0E SO	Shift Out( 不用切换 )
    0,                  // 0x0F SI	Shift In( 启用切换 )
    0,                  // 0x10 DLE	Data Link Escape( 数据链路转义 )
    0,                  // 0x11 DC1	Device Control 1( 设备控制 1) /XON(Transmit On)
    0,                  // 0x12 DC2	Device Control 2( 设备控制 2)
    0,                  // 0x13 DC3	Device Control 3( 设备控制 3) /XOFF(Transmit Off)
    0,                  // 0x14 DC4	Device Control 4( 设备控制 4)
    0,                  // 0x15 NAK	Negative Acknowledgement( 拒绝接收 / 无响应 )
    0,                  // 0x16 SYN	Synchronous Idle( 同步空闲 )
    0,                  // 0x17 ETB	End of Trans the Block( 传输块结束 )
    0,                  // 0x18 CAN	Cancel( 取消 )
    0,                  // 0x19 EM	End of Medium( 已到介质末端 / 介质存储已满 )
    0,                  // 0x1A SUB	Substitute( 替补 / 替换 )
    0,                  // 0x1B ESC	Escape( 溢出 / 逃离 / 取消 )
    0,                  // 0x1C FS	File Separator( 文件分割符 )
    0,                  // 0x1D GS	Group Separator( 分组符 )
    0,                  // 0x1E RS	Record Separator( 记录分隔符 )
    0,                  // 0x1F US	Unit Separator( 单元分隔符 )
    SPACE,              // 0x20 SP	White space
    OTHER,              // 0x21	!
    OTHER,              // 0x22	"
    OTHER,              // 0x23	#
    OTHER,              // 0x24	$
    OTHER,              // 0x25	%
    OTHER,              // 0x26	&
    OTHER,              // 0x27	'
    OTHER,              // 0x28	(
    OTHER,              // 0x29	)
    OTHER,              // 0x2A	*
    OTHER,              // 0x2B	+
    OTHER,              // 0x2C	,
    OTHER,              // 0x2D	-
    OTHER,              // 0x2E	.
    OTHER,              // 0x2F	/
    HEX|DEC|OCT|BIN,    // 0x30	0
    HEX|DEC|OCT|BIN,    // 0x31	1
    HEX|DEC|OCT,        // 0x32	2
    HEX|DEC|OCT,        // 0x33	3
    HEX|DEC|OCT,        // 0x34	4
    HEX|DEC|OCT,        // 0x35	5
    HEX|DEC|OCT,        // 0x36	6
    HEX|DEC|OCT,        // 0x37	7
    HEX|DEC,            // 0x38	8
    HEX|DEC,            // 0x39	9
    OTHER,              // 0x3A	:
    OTHER,              // 0x3B	;
    OTHER,              // 0x3C	<
    OTHER,              // 0x3D	=
    OTHER,              // 0x3E	>
    OTHER,              // 0x3F	?
    OTHER,              // 0x40	@
    ALPHA|HEX,          // 0x41	A
    ALPHA|HEX,          // 0x42	B
    ALPHA|HEX,          // 0x43	C
    ALPHA|HEX,          // 0x44	D
    ALPHA|HEX,          // 0x45	E
    ALPHA|HEX,          // 0x46	F
    ALPHA,              // 0x47	G
    ALPHA,              // 0x48	H
    ALPHA,              // 0x49	I
    ALPHA,              // 0x4A	J
    ALPHA,              // 0x4B	K
    ALPHA,              // 0x4C	L
    ALPHA,              // 0x4D	M
    ALPHA,              // 0x4E	N
    ALPHA,              // 0x4F	O
    ALPHA,              // 0x50	P
    ALPHA,              // 0x51	Q
    ALPHA,              // 0x52	R
    ALPHA,              // 0x53	S
    ALPHA,              // 0x54	T
    ALPHA,              // 0x55	U
    ALPHA,              // 0x56	V
    ALPHA,              // 0x57	W
    ALPHA,              // 0x58	X
    ALPHA,              // 0x59	Y
    ALPHA,              // 0x5A	Z
    OTHER,              // 0x5B	[
    OTHER,              // 0x5C	\ 
    OTHER,              // 0x5D	]
    OTHER,              // 0x5E	^
    ALPHA,              // 0x5F	_
    ALPHA|HEX,          // 0x61	a
    ALPHA|HEX,          // 0x62	b
    ALPHA|HEX,          // 0x63	c
    ALPHA|HEX,          // 0x64	d
    ALPHA|HEX,          // 0x65	e
    ALPHA|HEX,          // 0x66	f
    ALPHA,              // 0x67	g
    ALPHA,              // 0x68	h
    ALPHA,              // 0x69	i
    ALPHA,              // 0x6A	j
    ALPHA,              // 0x6B	k
    ALPHA,              // 0x6C	l
    ALPHA,              // 0x6D	m
    ALPHA,              // 0x6E	n
    ALPHA,              // 0x6F	o
    ALPHA,              // 0x70	p
    ALPHA,              // 0x71	q
    ALPHA,              // 0x72	r
    ALPHA,              // 0x73	s
    ALPHA,              // 0x74	t
    ALPHA,              // 0x75	u
    ALPHA,              // 0x76	v
    ALPHA,              // 0x77	w
    ALPHA,              // 0x78	x
    ALPHA,              // 0x79	y
    ALPHA,              // 0x7A	z
    OTHER,              // 0x7B	{
    OTHER,              // 0x7C	|
    OTHER,              // 0x7D	}
    OTHER,              // 0x7E	~
    0,                  // 0x7F	DEL Delete( 删除 )
};


timo_cache* timo_cache_new(timo_int cacheSize, timo_int tokenSize, timo_source* source)
{
    if (TIMO_NULL == source)
    {
        return (TIMO_NULL);
    }

    timo_cache* cache = (timo_cache*)timo_malloc(sizeof(timo_cache) + cacheSize + tokenSize + 1);
    timo_list_init(&cache->head);
    cache->source   = source;
    cache->cacheSize= cacheSize;
    cache->limitSize= tokenSize;
    cache->limit    = cache->cache;
    cache->line     = cache->cache;
    cache->cp       = cache->cache;
    cache->lineno   = 1;

    //  设置缓冲区的哨兵
    cache->limit[0] = '\n';

    //  初始化缓冲区的起始字符
    cache->cache[0] = '\0';

    return (cache);
}

timo_void   timo_cache_del(timo_cache* cache)
{
    cache->source->close(cache->source);
    timo_free(cache->source);
    timo_free(cache);
}

timo_void   timo_cache_tidy(timo_cache* cache)
{
    if (TIMO_NULL == cache)
    {
        return;
    }

    //  计算移动多远的距离
    timo_int offset = cache->cp - cache->cache;
    timo_int dataSize = cache->limit - cache->cp;
    if (dataSize > 0)
    {
        timo_memmove(cache->cache, cache->cp, dataSize);
    }
    cache->line  -= offset;
    cache->limit -= offset;
    cache->cp    -= offset;
    cache->limit[0] = '\0';
}

timo_state* timo_state_new()
{
    return (timo_state*)timo_malloc(sizeof(timo_state));
}

timo_void   timo_state_del(timo_state* state)
{
    timo_free(state);
}

inline timo_cache* timo_lex_top_cache(timo_lex* lex)
{
    if (timo_list_empty(&(lex->cache)))
    {
        return (TIMO_NULL);
    }

    timo_list* top = timo_list_top(&(lex->cache));
    return (timo_list_entry(timo_cache, top, head));
}

timo_state* timo_lex_top_state(timo_lex* lex)
{
    if (timo_list_empty(&(lex->state)))
    {
        return (TIMO_NULL);
    }

    timo_list* top = timo_list_top(&(lex->state));
    return (timo_list_entry(timo_state, top, head));
}

timo_token* timo_lex_token(timo_lex* lex)
{
    return (lex->token);
}

timo_lex*   timo_lex_new(timo_int prefetchSize, timo_int limitSize, timo_int tokenInitSize)
{
    timo_lex*   lex = (timo_lex*)timo_malloc(sizeof(timo_lex));
    lex->prefetchSize  = prefetchSize;
    lex->limitSize  = limitSize;
    timo_list_init(&lex->cache);
    timo_list_init(&lex->state);
    lex->token      = timo_token_new(tokenInitSize);
    lex->top_cache  = TIMO_NULL;
    lex->top_state  = TIMO_NULL;

    return (lex);
}

timo_void         timo_lex_del(timo_lex* lex)
{
    if (TIMO_NULL == lex)
    {
        return;
    }

    //  清除所有的cache
    timo_list* pos  =   TIMO_NULL;
    timo_list* next =   TIMO_NULL;
    timo_list_for_each_safe(pos, next, &(lex->cache))
    {
        timo_cache* cache = timo_list_entry(timo_cache, pos, head);
        timo_cache_del(cache);
    }

    //  清除所有的state
    pos  =   TIMO_NULL;
    next =   TIMO_NULL;
    timo_list_for_each_safe(pos, next, &(lex->state))
    {
        timo_state* state = timo_list_entry(timo_state, pos, head);
        timo_state_del(state);
    }

    //  释放错误码
    timo_token_del(lex->token);

    //  释放内存
    timo_free(lex);

    return;
}

timo_void         timo_lex_push(timo_lex* lex, timo_source* source)
{
    timo_cache* cache = timo_cache_new(lex->prefetchSize, lex->limitSize, source);
    if (TIMO_NULL == cache)
    {
        return;
    }

    timo_list_attach(&(lex->cache), &(cache->head));
    lex->top_cache = cache;
}

timo_void    timo_lex_pop(timo_lex* lex)
{
    //  删除最顶层的cache
    timo_free(timo_list_detach(&(lex->top_cache->head)));

    
    if (timo_list_empty(&(lex->cache)))
    {
        lex->top_cache = TIMO_NULL;
        return;
    }

    timo_list* top = timo_list_top(&(lex->cache));
    lex->top_cache = timo_list_entry(timo_cache, top, head);
}


//  return:
//      0   表示文件已经结束,任何数据都读不出来
//      >0  表示读取成功,返回值为读取的的数据的长度
//      <0  表示读取失败,读取失败
timo_int          timo_lex_fill(modifiable timo_lex* lex)
{
    //  获取topcache
    timo_cache* cache = lex->top_cache;
    if (TIMO_NULL == cache)
    {
        return (-1);
    }

    //  对数据进行整理
    timo_cache_tidy(lex->top_cache);


    timo_source* source = cache->source;
    if (TIMO_NULL == source)
    {
        return (-2);
    }

    //  如果已经到达文件尾部，不需要尝试读取了
    if ((*(source->eof))(source))
    {
        return (0);
    }

    //  向top缓冲区中填满数据
    timo_int readSize = (*(source->read))(source, cache->limit, (cache->cache + cache->cacheSize) - cache->limit);
    if (readSize <= 0)
    {
        return (readSize);
    }

    cache->limit    += readSize;
    cache->limit[0] = '\n';

    return (readSize);
}



inline void skip_space(timo_cache* cache)
{
    if (TIMO_NULL == cache)
    {
        return;
    }

    register timo_char* cp = cache->cp;
    while (1)
    {
        if (map[*cp] & SPACE)
        {
            cp++;
            continue;
        }

        if ('\n' == *cp)
        {
            if (cp == cache->limit)
            {
                break;
            }

            //  记录行号和新行起始地址
            cache->line = cp;
            cache->lineno++;
            cp++;
            continue;
        }

        break;
    }

    cache->cp = cp;
}

timo_int    accept_name(timo_lex* lex)
{
    timo_cache*         cache   = lex->top_cache;
    register timo_char* cp      = cache->cp;

    timo_token_name(lex->token);

RETRY:
    do
    {
        cp++;
    }while (map[*cp] & (ALPHA|DEC));

    //  将所有识别出来的东西保存起来
    timo_token_text_put(lex->token, cache->cp, cp - cache->cp);
    cache->cp = cp;

    //  如果最后遇到的不是换行
    if (('\n' != *cp) || (cp != cache->limit))
    {
        return (TK_NAME);
    }

    //  如果前面遇到的是换行且正好是哨兵，此时需要重新填充
    timo_int fillSize = timo_lex_fill(lex);
    if (0 == fillSize)
    {
        cache->cp = cp;
        return (TK_NAME);
    }

    if (fillSize < 0)
    {
        timo_token_error(lex->token, ERR_READ_FILE_FAILED);
        return (TK_ERROR);
    }

    cache   = lex->top_cache;
    cp      = cache->cp;
    goto RETRY;
}

timo_int    accept_hex_number(modifiable notnull timo_lex* lex)
{
    timo_cache* cache = lex->top_cache;
    timo_char*  cp = cache->cp;

    if ('0' != cp[0])
    {
        return (timo_token_error(lex->token, ERR_HEX_NUM_NEED_0X_PREFIX));
    }

    cp += 2;

    while (map[*cp] & HEX)
    {
        cp++;
    }

    if ((map[*cp] & (OTHER|SPACE|NEWLINE)) && ((cp - cache->cp) > 2))
    {
        timo_token_text_put(lex->token, cache->cp, cp - cache->cp);
        cache->cp = cp;
        return (TK_NUMBER);
    }

    return (timo_token_error(lex->token, ERR_INVALID_HEX_NUMBER));
}

timo_int    accept_oct_number(modifiable notnull timo_lex* lex)
{
    timo_cache* cache = lex->top_cache;
    timo_char*  cp = cache->cp;

    if ('0' != cp[0])
    {
        return (ERR_OCT_NUM_NEED_0T_PREFIX);
    }

    cp += 2;

    while (map[*cp] & OCT)
    {
        cp++;
    }

    if ((map[*cp] & (OTHER|SPACE|NEWLINE)) && ((cp - cache->cp) > 2))
    {
        timo_token_text_put(lex->token, cache->cp, cp - cache->cp);
        cache->cp = cp;
        return (TK_NUMBER);
    }

    return (timo_token_error(lex->token, ERR_INVALID_OCT_NUMBER));
}

timo_int    accept_bin_number(modifiable notnull timo_lex* lex)
{
    timo_cache* cache = lex->top_cache;
    timo_char*  cp = cache->cp;

    if ('0' != cp[0])
    {
        return (ERR_BIN_NUM_NEED_0B_PREFIX);
    }

    cp += 2;

    while (map[*cp] & BIN)
    {
        cp++;
    }

    if ((map[*cp] & (OTHER|SPACE|NEWLINE)) && ((cp - cache->cp) > 2))
    {
        timo_token_text_put(lex->token, cache->cp, cp - cache->cp);
        cache->cp = cp;
        return (TK_NUMBER);
    }

    return (timo_token_error(lex->token, ERR_INVALID_BIN_NUMBER));
}

//  begin with '.' or 'e' or 'E'
timo_int    accept_float_postfix(modifiable notnull timo_lex* lex)
{
    timo_cache* cache   = lex->top_cache;
    register timo_char*  cp = cache->cp;

    if ('.' == *cp)
    {
        cp++;

        //  如果点后面为非数字，说明是常量对象的成员函数调用，此时立即回滚
        if (!(map[*cp] & DEC))
        {
            //12.e3   =>  12 . e3
            return (TK_NUMBER);
        }

        while (map[*cp] & DEC)
        {
            cp++;
        }
    }

    //  浮点数的指数后缀部分一定要携带数字
    if (('e' == *cp) || ('E' == *cp))
    {
        cp++;

        //  可选的+-号
        if (('+' == *cp) || ('-' == *cp))
        {
            cp++;
        }

        //  指数后缀必须是数字
        if (!(map[*cp] & DEC))
        {
            //  浮点数的指数部分需要有数字做后缀
            return (timo_token_error(lex->token, ERR_FLOAT_POWER_NEED_DIGIT_POSTFIX));
        }

        while (map[*cp] & DEC)
        {
            cp++;
        }
    }

    //  浮点数最后必须是分隔符号
    if (map[*cp] & (OTHER|SPACE|NEWLINE))
    {
        timo_token_text_put(lex->token, cache->cp, cp - cache->cp);
        cache->cp = cp;
        return (TK_NUMBER);
    }

    return (timo_token_error(lex->token, ERR_INVALID_FLOAT_NUMBER));
}

timo_int    accept_other_number(modifiable notnull timo_lex* lex)
{
    timo_cache* cache = lex->top_cache;
    timo_char*  cp = cache->cp;

    while (map[*cp] & DEC)
    {
        cp++;
    }

    //  如果遇到浮点标识
    if (('.' == *cp) || ('e' == *cp) || ('E' == *cp))
    {
        timo_token_text_put(lex->token, cache->cp, cp - cache->cp);
        cache->cp = cp;
        return (accept_float_postfix(lex));
    }

    if (map[*cp] & OTHER|SPACE)
    {
        timo_token_text_put(lex->token, cache->cp, cp - cache->cp);
        cache->cp = cp;
        return (TK_NUMBER);
    }

    return (timo_token_error(lex->token, ERR_INVALID_NUMBER));
}

timo_int    accept_number(modifiable notnull timo_lex* lex)
{
    timo_cache* cache   = lex->top_cache;
    register timo_char*  cp = cache->cp;

    timo_token_number(lex->token);

    switch (cp[1])
    {
    case 'x':   
    case 'X':   return  (accept_hex_number(lex));
    case 't':   
    case 'T':   return  (accept_oct_number(lex));
    case 'b':   
    case 'B':   return  (accept_bin_number(lex));
    default:    return  (accept_other_number(lex));
    }
}

timo_int    accept_raw_string(timo_lex* lex)
{
    timo_cache* cache = lex->top_cache;
    register timo_char* cp = (++cache->cp);

    timo_token_str(lex->token);

    cp++;

    while ('\'' != *cp)
    {
        if ('\n' == *cp)
        {
            if (cp != cache->limit)
            {
                return (timo_token_error(lex->token, ERR_STRING_NOT_OVER_LINE));
            }

            //  将所有的数据缓存起来,并同步cp
            timo_token_text_put(lex->token, cache->cp, cp - cache->cp);
            cache->cp = cp;

            //  重新读取文件
            timo_int fillSize = timo_lex_fill(lex);
            if (fillSize < 0)
            {
                return (TK_ERROR);  //  读取文件失败
            }

            if (0 == fillSize)
            {
                return (TK_ERROR);  //  文件读取完毕，字符串未正常结束
            }

            //  重新获取cache和cp
            cache = lex->top_cache;
            cp    = cache->cp;
        }
    }

    //  保存读出来的数据,并同步cp
    timo_token_text_put(lex->token, cache->cp, cp - cache->cp);
    cache->cp = cp + 1;

    return (TK_CSTRING);
}

//  返回错误码
timo_eid accept_hex_char(timo_lex* lex, timo_int width)
{
    timo_cache* cache = lex->top_cache;
    timo_token* token = lex->token;
    timo_char*  cp    = cache->cp;

    //  跳过\u或者\x前缀
    cp += 2;

    timo_char c = 0;
    for (int i = 0; i < width; i++)
    {
        switch (cp[i])
        {
        case 'a':   case 'b':   case 'c':
        case 'd':   case 'e':   case 'f':   
            c = ((c << 4) + (cp[i] - 'a') + 10);
            continue;
        case 'A':   case 'B':   case 'C':
        case 'D':   case 'E':   case 'F':   
            c = ((c << 4) + (cp[i] - 'A') + 10);
            continue;
        default:
            return (ERR_INVALID_LEX_CHAR);  //  非十六进制转义字符
        }

        //  识别出一个字节后写入字符串cache
        if (1 == (i % 2))
        {
            timo_token_text_put(token, &c, 1);
            c = 0;
        }
    }

    (*cp) += width;
    return (0);
}

//  返回错误码
timo_eid    accept_string_escape_char(timo_lex* lex)
{
    timo_cache* cache = lex->top_cache;
    timo_token* token = lex->token;

    //  区分不同的转移类型处理
    switch (cache->cp[1])
    {
    case '\n':  return (ERR_STRING_NOT_OVER_LINE);  //  字符串不能跨行定义
    case 'x':   
    case 'X':   return accept_hex_char(lex, 2);
    case 'u':   
    case 'U':   return accept_hex_char(lex, 4);
    case '\"':  return timo_token_text_put(token, "\"", 1), cache->cp += 2, 0;
    case '\\':  return timo_token_text_put(token, "\\", 1), cache->cp += 2, 0;
    case 'n':   return timo_token_text_put(token, "\n", 1), cache->cp += 2, 0;
    case 'r':   return timo_token_text_put(token, "\r", 1), cache->cp += 2, 0;
    case 't':   return timo_token_text_put(token, "\t", 1), cache->cp += 2, 0;
    case 'v':   return timo_token_text_put(token, "\v", 1), cache->cp += 2, 0;
    case 'a':   return timo_token_text_put(token, "\a", 1), cache->cp += 2, 0;
    case 'b':   return timo_token_text_put(token, "\b", 1), cache->cp += 2, 0;
    case 'f':   return timo_token_text_put(token, "\f", 1), cache->cp += 2, 0;
    default:    return (ERR_INVALID_ESCAPE_CHAR);  //  不支持的转移字符
    }
}

//  返回tokenid
timo_int    accept_esc_string(timo_lex* lex)
{
    timo_cache* cache = lex->top_cache;
    timo_token* token = lex->token;
    register timo_char* cp = (++cache->cp);

    timo_token_str(lex->token);
    
    while ('\"' != *cp)
    {
        //  先尽可能保证至少能够识别出一个字符
        if ((cache->limit - cp) < cache->limitSize)
        {
            //  保存字符串并同步cp
            timo_token_text_put(lex->token, cache->cp, cp - cache->cp);
            cache->cp = cp;

            //  重新读取文件
            timo_int fillSize = timo_lex_fill(lex);
            if (fillSize < 0)
            {
                return (TK_ERROR);  //  读取文件失败
            }

            if (0 == fillSize)
            {
                return (TK_ERROR);  //  文件读取完毕
            }

            //  程序走到这里一定可以保证后续识别出一个字符所需要的缓冲区是足够的
            //  如果遇到文件输入结束，导致无法满足一个字符所需要的缓冲区大小，
            //  那么这是输入代码的词法问题

            //  重新获取cache和cp
            cache = lex->top_cache;
            cp    = cache->cp;
            continue;
        }


        switch (*cp)
        {
        case '\n':  
            {
                timo_token_error(lex->token, ERR_STRING_NOT_OVER_LINE);
                return (TK_ERROR);  //  字符串不能跨行定义
            }break;
        case '\\':
            {
                //  同步cp
                cache->cp = cp;

                //  该函数会修改cache->cp，所以这个函数返回成功后需要同步cp指针的值
                timo_int ret = accept_string_escape_char(lex);
                if (0 != ret)
                {
                    timo_token_error(lex->token, ret);
                    return (TK_ERROR);
                }

                //  同步cp指针的位置
                cp = cache->cp;
            }break;
        default:    cp++;   break;
        }
    }

    //  保存识别出来的数据
    timo_token_text_put(lex->token, cache->cp, cp - cache->cp);
    cache->cp = (cp + 1);

    return (TK_CSTRING);
}


timo_int    accept_error(timo_lex* lex, timo_int id)
{
    timo_token_error(lex->token, id);
    return (TK_ERROR);
}

timo_int    accept_comment(timo_lex* lex)
{
    timo_cache* cache = lex->top_cache;
    register timo_char* cp = cache->cp;

    timo_token_comment(lex->token);

RETRY:
    while ('\n' != *cp)
    {
        cp++;
    }

    if (cp != cache->limit)
    {
        timo_token_text_put(lex->token, cache->cp, cp - cache->cp);
        cache->cp = cp;
        return (TK_COMMENT);
    }

    //  先吃掉已经识别的数据
    cache->cp = cp;

    //  重新填充
    timo_int ret = timo_lex_fill(lex);

    //  如果填充失败
    if (ret < 0)
    {
        return (TK_ERROR);
    }

    //  如果无数据可读
    if (0 == ret)
    {
        return (TK_COMMENT);
    }

    //  重新读取后，关键变量必须重新获取
    cache = lex->top_cache;
    cp = cache->cp;
    goto   RETRY;

    return (TK_COMMENT);
}

timo_int timo_lex_next(timo_lex* lex)
{
    //  获取当前的cache
    timo_cache* cache = TIMO_NULL;

RETRY:
    //  获取当前的cache
    cache = lex->top_cache;

    //  如果没有缓冲区提供输入数据，所有输入结束了
    if (TIMO_NULL == cache)
    {
        return (TK_END);
    }

    //  先尝试跳过空白
    skip_space(cache);

    //  检查预留空间是否足够
    if ((cache->limit - cache->cp) < cache->limitSize)
    {
        int fillSize = timo_lex_fill(lex);
        cache = lex->top_cache;

        if (fillSize < 0)
        {
            return (TK_ERROR);
        }

        //  当前文件读取完毕
        if (0 == fillSize)
        {
            if (cache->cp == cache->limit)  //  当前文件读取完毕，且没有剩余数据时，自动弹出下一个缓冲区
            {
                timo_lex_pop(lex);
                goto RETRY;
            }
        }
    }

    //  程序走到这里必须保证缓冲区至少有cache->limitSize大小的数据；
    //  或者确实无数据可读导致数据不够cache->limitSize大小也可以；

    timo_token_clear(lex->token);

    //  由于cache在lex重新填充后可能会变更，所以cache需要重新获取
    register timo_char* cp = cache->cp;
//#define ACCEPT(x,)
#define ACCEPT1(x,retx,retz)        ((cp[1] == (x))?(cache->cp +=2, retx):(cache->cp +=1, retz))
#define ACCEPT2(x,retx,y,rety,retz) ((cp[1] == (x))?(cache->cp +=2, retx):((cp[1] == (y))?(cache->cp +=2, rety):(cache->cp +=1, retz)))
    switch (*cp)
    {
    case ':':   case ',':   case ';':   
    case '{':   case '}':   case '[':   
    case ']':   case '(':   case ')':   
    case '^':   case '~':   case '.':   
    case '+':   case '-':   case '*':   
    case '/':   case '%':   
        return  ((cache->cp++), *cp);
    case '!':   return  (ACCEPT1('=', TK_NT_EQ, '!'));
    case '&':   return  (ACCEPT1('&', TK_AD_AD, '&'));
    case '=':   return  (ACCEPT1('=', TK_EQ_EQ, '='));
    case '|':   return  (ACCEPT1('|', TK_OR_OR, '|'));
    case '<':   return  (ACCEPT2('=', TK_LT_EQ, '<', TK_LSHFT, '<'));
    case '>':   return  (ACCEPT2('=', TK_LG_EQ, '>', TK_RSHFT, '>'));
    case '\'':  return  (accept_raw_string(lex));
    case '\"':  return  (accept_esc_string(lex));
    case '#':   return  (accept_comment(lex));
    case '0':   case '1':   case '2':
    case '3':   case '4':   case '5':
    case '6':   case '7':   case '8':
    case '9':   
        return  (accept_number(lex));
    case '_':   case 'A':   case 'B':
    case 'C':   case 'D':   case 'E':
    case 'F':   case 'G':   case 'H':
    case 'I':   case 'J':   case 'K':
    case 'L':   case 'M':   case 'N':
    case 'O':   case 'P':   case 'Q':
    case 'R':   case 'S':   case 'T':
    case 'U':   case 'V':   case 'W':
    case 'X':   case 'Y':   case 'Z':
        return (accept_name(lex));
    case 'a':   case 'g':   case 'h':
    case 'j':   case 'k':   case 'l':
    case 'm':   case 'n':   case 'o':
    case 'p':   case 'q':   case 's':
    case 't':   case 'u':   case 'w':
    case 'x':   case 'y':   case 'z':
    case 'd':   return (accept_name(lex));
    case 'b':   //  break
        if (   (cp[1] == 'r')
            && (cp[2] == 'e')
            && (cp[3] == 'a')
            && (cp[4] == 'k')
            && (cp[5] &  (OTHER|SPACE)))
        {
            return  (cache->cp += 5, TK_BREAK);
        }
        return  (accept_name(lex));
    case 'c':   //  continue
        if (   (cp[1] == 'o')
            && (cp[2] == 'n')
            && (cp[3] == 't')
            && (cp[4] == 'i')
            && (cp[5] == 'n')
            && (cp[6] == 'u')
            && (cp[7] == 'e')
            && (cp[8] &  (OTHER|SPACE)))
        {
            return (cache->cp += 8, TK_CONTINUE);
        }
        return  (accept_name(lex));
    case 'e':   //  else
        if (   (cp[1] == 'l')
            && (cp[2] == 's')
            && (cp[3] == 'e')
            && (cp[4] &  (OTHER|SPACE)))
        {
            return  (cache->cp += 4, TK_ELSE);
        }
        return  (accept_name(lex));
    case 'f':   //  func    for
        if (   (cp[1] == 'u')
            && (cp[2] == 'n')
            && (cp[3] == 'c')
            && (cp[4] &  (OTHER|SPACE)))
        {
            return  (cache->cp += 4, TK_FUNC);
        }
        if (   (cp[1] == 'o')
            && (cp[2] == 'r')
            && (cp[3] &  (OTHER|SPACE)))
        {
            return  (cache->cp += 3, TK_FOR);
        }
        return (accept_name(lex));
    case 'i':   //  if  import  interface
        if (   (cp[1] == 'f')
            && (cp[2] &  (OTHER|SPACE)))
        {
            return  (cache->cp += 2, TK_IF);
        }
        if (   (cp[1] == 'm')
            && (cp[2] == 'p')
            && (cp[3] == 'o')
            && (cp[4] == 'r')
            && (cp[5] == 't')
            && (cp[6] &  (OTHER|SPACE)))
        {
            return  (cache->cp += 6, TK_IMPORT);
        }
        if (   (cp[1] == 'n')
            && (cp[2] == 't')
            && (cp[3] == 'e')
            && (cp[4] == 'r')
            && (cp[5] == 'f')
            && (cp[6] == 'a')
            && (cp[7] == 'c')
            && (cp[8] == 'e')
            && (cp[9] &  (OTHER|SPACE)))
        {
            return  (cache->cp += 9, TK_INTERFACE);
        }
        return  (accept_name(lex));
    case 'r':   //  return
        if (   (cp[1] == 'e')
            && (cp[2] == 't')
            && (cp[3] == 'u')
            && (cp[4] == 'r')
            && (cp[5] == 'n')
            && (cp[6] &  (OTHER|SPACE)))
        {
            return  (cache->cp += 6, TK_RETURN);
        }
        return  (accept_name(lex));
    case 'v':   //  var
        if (   (cp[1] == 'a')
            && (cp[2] == 'r')
            && (cp[3] &  (OTHER|SPACE)))
        {
            return  (cache->cp += 3, TK_VAR);
        }
        return  (accept_name(lex));
    case '?':   case '@':   case '$':   
    case '`':   case '\\':  
    default:    return  (accept_error(lex, ERR_INVALID_LEX_CHAR));
    }


    return (-1);
}

timo_int     timo_lex_row(const notnull timo_lex* lex)
{
    if (TIMO_NULL == lex->top_cache)
    {
        return (0);
    }

    return (lex->top_cache->lineno);
}

timo_int     timo_lex_col(const notnull timo_lex* lex)
{
    if (TIMO_NULL == lex->top_cache)
    {
        return (0);
    }

    return ((lex->top_cache->cp - lex->top_cache->line) + 1);
}
