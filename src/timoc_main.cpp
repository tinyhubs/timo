#include "timo_source.h"
#include "timo_libc.h"
#include "timo_lex.h"
#include "timo_type.h"

class pause
{
public:
    ~pause()
    {
        getchar();
    }
}ps;

int main(int argc, char*argv[])
{
    //  ����һ���µĴʷ�������
    timo_lex* lex = timo_lex_new(1024, 32, 512);
    if (TIMO_NULL == lex)
    {
        return (-2);
    }

    //  ���µ��ļ�ѹ��ʷ�������
    timo_lex_push(lex, timo_file_new("E:\\dev\\timo\\simple\\test_zero.timo", "r"));

    //  �Ӵʷ��������л�ȡһ������
    timo_int id = 0;
    while (id >= 0)
    {
        id = timo_lex_next(lex);
        timo_token* token = timo_lex_token(lex);
        timo_printf("%d\t%s\t%s\tline(%d)\n", id, timo_token_desc(id), timo_token_text_get(token), timo_lex_row(lex));
        if (TK_END == id)
        {
            break;
        }
    }

    return (0);
}

