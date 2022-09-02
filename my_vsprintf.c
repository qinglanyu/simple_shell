/**
 * this file is copyed from /linux/lib/vsprintf.c
 * for learning to format string
 *
 */

#include <stdarg.h>

const char hex_asc[] = "0123456789abcdef";
const char hex_asc_upper[] = "0123456789ABCDEF";

#define SIGN    1   /* unsigned/signed, must be 1 */
#define LEFT    2   /* left justified */
#define PLUS    4   /* show plus */
#define SPACE   8   /* space if plus */
#define ZEROPAD 16  /* pad with zero, must 16 == '0' - ' ' */
#define SMALL   32  /* use lowercase in hex (must be 32 == 0x20) */
#define SPECIAL 64  /* prefix hex with "0x", otcal with "0" */

enum format_type {
    FORMAT_TYPE_NONE, /* Just a string part */
    FORMAT_TYPE_WIDTH,
    FORMAT_TYPE_CHAR,
    FORMAT_TYPE_STR,
    FORMAT_TYPE_PTR,
    FORMAT_TYPE_PERCENT_CHAR,
    FORMAT_TYPE_INVALID,
    FORMAT_TYPE_LONG_LONG,
    FORMAT_TYPE_ULONG,
    FORMAT_TYPE_LONG,
    FORMAT_TYPE_UBYTE,
    FORMAT_TYPE_BYTE,
    FORMAT_TYPE_USHORT,
    FORMAT_TYPE_SHORT,
    FORMAT_TYPE_UINT,
    FORMAT_TYPE_INT,
    FORMAT_TYPE_SIZE_T,
    FORMAT_TYPE_PTRDIFF
};

struct printf_spec {
    unsigned int type:8;         /* format_type enum */
    signed int   field_width:24; /* width of output field */
    unsigned int flags:8;        /* flags to number() */
    unsigned int base:8;         /* number base, 8, 10, or 16 only */
    signed int   precision:16;   /* # of digits/chars */
} __packed;

#define FIELD_WIDTH_MAX ((1 << 23) - 1)
#define PRECISION_MAX ((1 << 15) - 1)

static char *put_dec_trunc8(char *buf, unsigned r)
{
    unsigned q;

    /* 1 <= r <= 10^8 */
    if (r < 100) {
        goto out_r;
    }

    /* 100 <= r < 10^8 */
    q = (r * (u64)0x28f5c29) >> 32;
    *((u16 *)buf) = decpair[r - 100 * q];
    buf += 2;

    // to be continue...
}

static char *put_dec(char *buf, unsigned long long n)
{
    uint32_t d3, d2, d1, q, h;

    if (n < 100 * 1000 * 1000) {
        return put_dec_trunc8(buf, n);
    }
}

static char *number(char *buf, char *end, unsigned long long num, struct printf_spec spec)
{
    /* put_dec requires 2-byte alignment of the buffer */
    char tmp[3 * sizeof(num)] __aligned(2);
    char sign;
    char locase;
    int need_pfx = ((spec.flags & SPECIAL) && spec.base != 10); // prefix with 0x or 0
    int i;
    bool is_zero = num == 0LL;
    int field_width = spec.field_width;
    int precision = spec.precision;

    /* locase = 0 or 0x20. ORing digits or letters with 'locase'
     * produces same digits or (maybe lowercased) letters */
    locase = (spec.flags & SMALL);
    if (spec.flags & LEFT) {
        spec.flags &= ~ZEROPAD;
    }
    sign = 0;
    if (spec.flags & SIGN) {
        if ((signed long long)num < 0) {
            sign = '-;
            num = -(signed long long)num;
            field_width--;
        } else if (spec.flags & PLUS) {
            sign = '+';
            field_width--;
        } else if (spec.flags & SPACE) {
            sign = ' ';
            field_width--;
        }
    }
    if (need_pfx) {
        if (spec.base == 16)
            field_width -= 2;
        else if (!is_zero)
            field_width--;
    }

    /* generate full string in tmp[], in reverse order */
    i = 0;
    if (num < spec.base) {
        tmp[i++] = hex_asc_upper[num] | locase;
    } else if (spec.base != 10) { /* 8 or 16 */
        int mask = spec.base -1;
        int shift = 3;

        if (spec.base == 16) {
            shift = 4;
        }
        do {
            tmp[i++] = (hex_asc_upper[((unsigned char)num) & mask] | locase);
            num >>= shift;
        } while (num);
    } else { /* base 10 */
        i = put_dec(tmp, num) - tmp;
    }

    /* printing 100 using %2d gives "100", not "00" */
    if (i > precision) {
        precision = i;
    }

    /* leading space padding */
    field_width -= precision;
    if (!(spec.flags & (ZEROPAD | LEFT))) {
        while (--field_width >= 0) {
            if (buf < end) {
                *buf = ' ';
            }
            ++buf;
        }
    }

    /* sign */
    if (sign) {
        if (buf < end) {
            *buf = sign;
        }
        ++buf;
    }

    /* "0x" / "0" prefix */
    if (need_pfx) {
        if (spec.base == 16 || !is_zero) {
            if (buf < end) {
                *buf = '0';
            }
            ++buf;
        }
        if (spec.base == 16) {
            if (buf < end) {
                *buf = ('X' | locase);
            }
            ++buf;
        }
    }

    /* zero or space padding */
    if (!(spec.flags & LEFT)) {
        char c = ' ' + (spec.flags & ZEROPAD);
        while (--field_width >= 0) {
            if (buf < end) {
                *buf = c;
            }
            ++buf;
        }
    }

    /* hmm even more zero padding? */
    while (i <= --precision) {
        if (buf < end) {
            *buf = '0';
        }
        ++buf;
    }

    /* actual digits of result */
    while (--i >= 0) {
        if (buf < end) {
            *buf = tmp[i];
        }
        ++buf;
    }

    /* trailing space padding */
    while (--field_width >= 0) {
        if (buf < end) {
            *buf = ' ';
        }
        ++buf;
    }

    return buf;
}

int vsnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
    unsigned long long num;
    char *str, *end;
    struct printf_spec spec = {0};

    /* Reject out-of-range values early. Large positive sizes are used for unknown buffer sizes. */
    /* So the size should not be too large, here just do not check it for simple */
    
    str = buf;
    end = buf + size;

    /* Make sure end is always >= buf */
    if (end < buf) {
        end = ((void *) - 1);
        size = end - buf;
    }

    while (*fmt) {
        const char *old_fmt = fmt;
        int read = format_decode(fmt, &spce);

        fmt += read;
        
        switch (spec.type) {
            case FORMAT_TYPE_NONE: 
        }
    }
}

/* vxworks usrLib.c
 * d - display memory
 */

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
#define UINT8 unsigned char
#define USHORT unsigned short
#define ULONG unsigned long
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
    last_adrs = (void *)((int) last_adrs & ~(width - 1));

    /* print leading spaces on first line */
    memset(ascii, '.', 16);
    printf("%08x:  ", (int) last_adrs & ~0xF);

    for (item = 0; item < ((int) last_adrs & 0xF) / width; item++) {
        printf("%*s ", 2*width, " ");
        memset(&ascii[item * width], ' ', 2*width);
    }

    /* print out all the words */
    while (nunits-- > 0) {
        if (item == MAX_BYTES_PER_LINE / width) {
            /* end of line:
             * print out ascii format values and address of next line */
            printf("  *%16s*\n%\n%08x:  ", ascii, (int) last_adrs);
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
                acsii[item * width + ix] = *pByte;
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

    printf(" *%16s*\n", ascii);  /* print oiut ascii format values */
}



























