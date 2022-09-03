

/* vxworks usrLib.c
 * d - display memory
 */

// #include <stdio.h>
#include "printf.h"
#include <string.h>

#define _C_UPPER        0x1
#define _C_LOWER        0x2
#define _C_NUMBER       0x4
#define _C_WHITE_SPACE  0x8
#define _C_PUNCT        0x10
#define _C_CONTROL      0x20
#define _C_HEX_NUMBER   0x40
#define _C_B            0x80

static const unsigned char __ctype[] = {
    /* 0 */ 	0,				/* dummy */
    /* 80 */	_C_B,				/* nul */
    /* 80 */	_C_B,				/* soh */
    /* 80 */	_C_B,				/* stx */
    /* 80 */	_C_B,				/* etx */
    /* 80 */	_C_B,				/* eot */
    /* 80 */	_C_B,				/* enq */
    /* 80 */	_C_B,				/* ack */
    /* 80 */	_C_B,				/* bel */
    /* 80 */	_C_B,				/* bs */
    /* 20 */	_C_CONTROL,			/* ht */
    /* 20 */	_C_CONTROL,			/* nl */
    /* 20 */	_C_CONTROL,			/* vt */
    /* 20 */	_C_CONTROL,			/* np */
    /* 20 */	_C_CONTROL,			/* cr */
    /* 80 */	_C_B,				/* so */
    /* 80 */	_C_B,				/* si */
    /* 80 */	_C_B,				/* dle */
    /* 80 */	_C_B,				/* dc1 */
    /* 80 */	_C_B,				/* dc2 */
    /* 80 */	_C_B,				/* dc3 */
    /* 80 */	_C_B,				/* dc4 */
    /* 80 */	_C_B,				/* nak */
    /* 80 */	_C_B,				/* syn */
    /* 80 */	_C_B,				/* etb */
    /* 80 */	_C_B,				/* can */
    /* 80 */	_C_B,				/* em */
    /* 80 */	_C_B,				/* sub */
    /* 80 */	_C_B,				/* esc */
    /* 80 */	_C_B,				/* fs */
    /* 80 */	_C_B,				/* gs */
    /* 80 */	_C_B,				/* rs */
    /* 80 */	_C_B,				/* us */
    /* 08 */	_C_WHITE_SPACE,			/* space */
    /* 10 */	_C_PUNCT,			/* ! */
    /* 10 */	_C_PUNCT,			/* " */
    /* 10 */	_C_PUNCT,			/* # */
    /* 10 */	_C_PUNCT,			/* $ */
    /* 10 */	_C_PUNCT,			/* % */
    /* 10 */	_C_PUNCT,			/* & */
    /* 10 */	_C_PUNCT,			/* ' */
    /* 10 */	_C_PUNCT,			/* ( */
    /* 10 */	_C_PUNCT,			/* ) */
    /* 10 */	_C_PUNCT,			/* * */
    /* 10 */	_C_PUNCT,			/* + */
    /* 10 */	_C_PUNCT,			/* , */
    /* 10 */	_C_PUNCT,			/* - */
    /* 10 */	_C_PUNCT,			/* . */
    /* 10 */	_C_PUNCT,			/* / */
    /* 44 */	_C_NUMBER | _C_HEX_NUMBER,	/* 0 */
    /* 44 */	_C_NUMBER | _C_HEX_NUMBER,	/* 1 */
    /* 44 */	_C_NUMBER | _C_HEX_NUMBER,	/* 2 */
    /* 44 */	_C_NUMBER | _C_HEX_NUMBER,	/* 3 */
    /* 44 */	_C_NUMBER | _C_HEX_NUMBER,	/* 4 */
    /* 44 */	_C_NUMBER | _C_HEX_NUMBER,	/* 5 */
    /* 44 */	_C_NUMBER | _C_HEX_NUMBER,	/* 6 */
    /* 44 */	_C_NUMBER | _C_HEX_NUMBER,	/* 7 */
    /* 44 */	_C_NUMBER | _C_HEX_NUMBER,	/* 8 */
    /* 44 */	_C_NUMBER | _C_HEX_NUMBER,	/* 9 */
    /* 10 */	_C_PUNCT,			/* : */
    /* 10 */	_C_PUNCT,			/* ; */
    /* 10 */	_C_PUNCT,			/* < */
    /* 10 */	_C_PUNCT,			/* = */
    /* 10 */	_C_PUNCT,			/* > */
    /* 10 */	_C_PUNCT,			/* ? */
    /* 10 */	_C_PUNCT,			/* @ */
    /* 41 */	_C_UPPER | _C_HEX_NUMBER,	/* A */
    /* 41 */	_C_UPPER | _C_HEX_NUMBER,	/* B */
    /* 41 */	_C_UPPER | _C_HEX_NUMBER,	/* C */
    /* 41 */	_C_UPPER | _C_HEX_NUMBER,	/* D */
    /* 41 */	_C_UPPER | _C_HEX_NUMBER,	/* E */
    /* 41 */	_C_UPPER | _C_HEX_NUMBER,	/* F */
    /* 01 */	_C_UPPER,			/* G */
    /* 01 */	_C_UPPER,			/* H */
    /* 01 */	_C_UPPER,			/* I */
    /* 01 */	_C_UPPER,			/* J */
    /* 01 */	_C_UPPER,			/* K */
    /* 01 */	_C_UPPER,			/* L */
    /* 01 */	_C_UPPER,			/* M */
    /* 01 */	_C_UPPER,			/* N */
    /* 01 */	_C_UPPER,			/* O */
    /* 01 */	_C_UPPER,			/* P */
    /* 01 */	_C_UPPER,			/* Q */
    /* 01 */	_C_UPPER,			/* R */
    /* 01 */	_C_UPPER,			/* S */
    /* 01 */	_C_UPPER,			/* T */
    /* 01 */	_C_UPPER,			/* U */
    /* 01 */	_C_UPPER,			/* V */
    /* 01 */	_C_UPPER,			/* W */
    /* 01 */	_C_UPPER,			/* X */
    /* 01 */	_C_UPPER,			/* Y */
    /* 01 */	_C_UPPER,			/* Z */
    /* 10 */	_C_PUNCT,			/* [ */
    /* 10 */	_C_PUNCT,			/* \ */
    /* 10 */	_C_PUNCT,			/* ] */
    /* 10 */	_C_PUNCT,			/* ^ */
    /* 10 */	_C_PUNCT,			/* _ */
    /* 10 */	_C_PUNCT,			/* ` */
    /* 42 */	_C_LOWER | _C_HEX_NUMBER,	/* a */
    /* 42 */	_C_LOWER | _C_HEX_NUMBER,	/* b */
    /* 42 */	_C_LOWER | _C_HEX_NUMBER,	/* c */
    /* 42 */	_C_LOWER | _C_HEX_NUMBER,	/* d */
    /* 42 */	_C_LOWER | _C_HEX_NUMBER,	/* e */
    /* 42 */	_C_LOWER | _C_HEX_NUMBER,	/* f */
    /* 02 */	_C_LOWER,			/* g */
    /* 02 */	_C_LOWER,			/* h */
    /* 02 */	_C_LOWER,			/* i */
    /* 02 */	_C_LOWER,			/* j */
    /* 02 */	_C_LOWER,			/* k */
    /* 02 */	_C_LOWER,			/* l */
    /* 02 */	_C_LOWER,			/* m */
    /* 02 */	_C_LOWER,			/* n */
    /* 02 */	_C_LOWER,			/* o */
    /* 02 */	_C_LOWER,			/* p */
    /* 02 */	_C_LOWER,			/* q */
    /* 02 */	_C_LOWER,			/* r */
    /* 02 */	_C_LOWER,			/* s */
    /* 02 */	_C_LOWER,			/* t */
    /* 02 */	_C_LOWER,			/* u */
    /* 02 */	_C_LOWER,			/* v */
    /* 02 */	_C_LOWER,			/* w */
    /* 02 */	_C_LOWER,			/* x */
    /* 02 */	_C_LOWER,			/* y */
    /* 02 */	_C_LOWER,			/* z */
    /* 10 */	_C_PUNCT,			/* { */
    /* 10 */	_C_PUNCT,			/* | */
    /* 10 */	_C_PUNCT,			/* } */
    /* 10 */	_C_PUNCT,			/* ~ */
    /* 80 */	_C_B				/* del */
};

#define __isprint(c) (__ctype[(unsigned char)(c)] & (_C_PUNCT | _C_UPPER | _C_LOWER | _C_WHITE_SPACE | _C_NUMBER))
#define isprint(c) __isprint((unsigned char)(c))

#define FAST register
#define MAX_BYTES_PER_LINE 16
typedef unsigned char UINT8;
typedef unsigned short USHORT;
typedef unsigned int UINT;
typedef unsigned long ULONG;
#define EOS '\0'
#define isascii(c) ((unsigned char)(c) <= 0177)
#define toascii(c) ((unsigned char)(c) & 0177)

void d(FAST void *adrs, int nunits, int width)
{
    static int dNitems = 0x80;
    static int dWidth = 2;
    static void *last_adrs = 0;

    FAST int item;
    char ascii[MAX_BYTES_PER_LINE + 1];
    int ix;
    UINT8 *pByte;
    UINT8 *tmpByte;
    USHORT *tmpShort;
    ULONG *tmpLong;

    ascii[MAX_BYTES_PER_LINE] = EOS;

    if (nunits == 0)
        nunits = dNitems;
    else
        dNitems = nunits;

    if (width == 0)
        width = dWidth;
    else {
        if (width != 1 && width !=2 && width != 4 && width != 8)
            width = 1;
        dWidth = width;
    }

    if (adrs == 0)
        adrs = last_adrs;
    else
        last_adrs = adrs;

    /* round address down to appropriate boundary */
    last_adrs = (void *)((long long) last_adrs & ~(width - 1));

    /* print leading spaces on first line */
    memset(ascii, '.', 16);
    printf("%08x:  ", (unsigned int) last_adrs & ~0xF);

    for (item = 0; item < ((int) last_adrs & 0xF) / width; item++) {
        printf("%*s ", 2*width, " ");
        memset(&ascii[item * width], ' ', 2*width);
    }

    /* print out all the words */
    while (nunits-- > 0) {
        if (item == MAX_BYTES_PER_LINE / width) {
            /* end of line:
             * print out ascii format values and address of next line */
            printf("  *%16s*\n%08x:  ", ascii, (int) last_adrs);
            memset(ascii, '.', MAX_BYTES_PER_LINE);
            item = 0;
        }

        switch (width) {
            case 1:
                tmpByte = (UINT8 *) last_adrs;
                printf("%02x", *tmpByte);
                break;
            case 2:
                tmpShort = (USHORT *) last_adrs;
                printf("%04x", *tmpShort);
                break;
            case 4:
                tmpLong = (ULONG *) last_adrs;
                printf("%08lx", *tmpLong);
                break;
            case 8:
                tmpLong = (ULONG *) last_adrs;
#if _BYTE_ORDER == _LITTLE_ENDIAN
                printf("%08lx%08lx", *(tmpLong+1), *tmpLong);
#else
                printf("%08lx%08lx", *tmpLong, *(tmpLong+1));
#endif
                break;
            default:
                tmpByte = (UINT8 *) last_adrs;
                printf("%02x", *tmpByte);
                break;
        }
        printf(" ");    /* space between words */

        /* set ascii buffer */
        pByte = (UINT8 *) last_adrs;
        for (ix = 0; ix < width; ix++) {
            if (*pByte == ' ' || (isascii(*pByte) && isprint(*pByte))) {
                ascii[item * width + ix] = *pByte;
            }
            pByte++;
        }

        last_adrs = (void *)((int) last_adrs + width);
        item++;
    }

    /* print remainder of last line */
    for (; item < MAX_BYTES_PER_LINE / width; item++) {
        printf("%*s ", 2*width, " ");
    }

    printf("  *%16s*\n", ascii);  /* print out ascii format values */
}

int main()
{
    int a = 123;
    int *p = &a;
    char *china = "I love china";
    
    printf("a = %d = %08x, &a = %p = %p\n", a, a, &a, p);

    printf("call d(&a, 0, 0):\n");
    
    printf("%s pi=%f %d=0%o=0x%x at='%c' @\"%s\"\n", __FUNCTION__, 3.1415926, 1024, 1024, 1024, '@', __DATE__);

    d((void*)p, 32, 1);

    d((void*)china, 32, 1);

    return 0;
}

/*#include <stdio.h>*/
void _putchar(char character)
{
	putchar(character);
}














