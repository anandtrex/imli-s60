/* Generated by re2c 0.13.5 on Wed Oct 28 17:26:23 2009 */
/* scanner that will be included in cnj.c */
#include <stdio.h>
#include "libimli.h"
#include "imli_private.h"

int scan(char *s, int l, imli_context_t ctx)
{
    char *p = s;
    char *q = 0;
    char *t;
    int terminate = 0;
    char cons[4] = {0, 0, 0, 0};
    int level = 0;
    int count = 0;
    int i = 0;
    cnj_entry_t *entry = 0;
    
#define COMBINE(c)                              \
    do                                          \
    {                                           \
        cons[level] = c;                        \
        if (level && level < 4)                 \
        {                                       \
                                                \
            unsigned int code = 0;              \
            for (i = 1; i <= level; i++)        \
                code |= cons[i] << 8 * (i - 1); \
            count++;                            \
            entry->conjuncts[count] = code;     \
        }                                       \
        else                                    \
        {                                       \
            entry = &(ctx->conjunct_table[c]);  \
        }                                       \
    } while (0)

    
#define YYCTYPE         char
#define YYCURSOR        p
#define YYLIMIT         (s+l+1)
#define YYMARKER        q
#define YYFILL(n)
	
    while(!terminate)
    {
        t = p;
        
		{
			YYCTYPE yych;

			if ((YYLIMIT - YYCURSOR) < 6) YYFILL(6);
			yych = *YYCURSOR;
			switch (yych) {
			case '\t':
			case '\n':
			case '\r':
			case ' ':
			case ',':	goto yy2;
			case '#':	goto yy12;
			case '(':	goto yy8;
			case ')':	goto yy10;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':	goto yy4;
			case ':':	goto yy6;
			case 'b':	goto yy15;
			case 'c':	goto yy16;
			case 'd':	goto yy17;
			case 'f':	goto yy18;
			case 'g':	goto yy19;
			case 'h':	goto yy20;
			case 'j':	goto yy21;
			case 'k':	goto yy22;
			case 'l':	goto yy23;
			case 'm':	goto yy24;
			case 'n':	goto yy25;
			case 'p':	goto yy26;
			case 'q':	goto yy27;
			case 'r':	goto yy28;
			case 's':	goto yy29;
			case 't':	goto yy30;
			case 'v':	goto yy31;
			case 'x':	goto yy32;
			case 'y':	goto yy33;
			case 'z':	goto yy34;
			default:	goto yy14;
			}
yy2:
			++YYCURSOR;
			yych = *YYCURSOR;
			goto yy188;
yy3:
			{ continue; }
yy4:
			yych = *(YYMARKER = ++YYCURSOR);
			if (yych == '.') goto yy183;
			if (yych <= '/') goto yy5;
			if (yych <= '9') goto yy185;
yy5:
			{ if (entry) entry->count = count; terminate = 1; continue; }
yy6:
			++YYCURSOR;
			{ if (level == 0) level++; continue; }
yy8:
			++YYCURSOR;
			{ if (level > 0 && level < 3) level++; continue; }
yy10:
			++YYCURSOR;
			{ if (level > 1) level--; continue; }
yy12:
			++YYCURSOR;
			{ terminate = 1; continue; }
yy14:
			yych = *++YYCURSOR;
			goto yy5;
yy15:
			yych = *(YYMARKER = ++YYCURSOR);
			if (yych == 'a') goto yy179;
			if (yych == 'h') goto yy178;
			goto yy5;
yy16:
			yych = *(YYMARKER = ++YYCURSOR);
			if (yych <= 'b') {
				if (yych == 'a') goto yy168;
				goto yy5;
			} else {
				if (yych <= 'c') goto yy167;
				if (yych == 'h') goto yy170;
				goto yy5;
			}
yy17:
			yych = *(YYMARKER = ++YYCURSOR);
			if (yych <= 'c') {
				if (yych == 'a') goto yy152;
				goto yy5;
			} else {
				if (yych <= 'd') goto yy150;
				if (yych == 'h') goto yy151;
				goto yy5;
			}
yy18:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy148;
			goto yy5;
yy19:
			yych = *(YYMARKER = ++YYCURSOR);
			if (yych <= 'f') {
				if (yych == 'a') goto yy142;
				goto yy5;
			} else {
				if (yych <= 'g') goto yy140;
				if (yych <= 'h') goto yy141;
				goto yy5;
			}
yy20:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy138;
			goto yy5;
yy21:
			yych = *(YYMARKER = ++YYCURSOR);
			if (yych <= 'g') {
				if (yych == 'a') goto yy131;
				goto yy5;
			} else {
				if (yych <= 'h') goto yy130;
				if (yych == 'n') goto yy129;
				goto yy5;
			}
yy22:
			yych = *(YYMARKER = ++YYCURSOR);
			if (yych <= 'g') {
				if (yych == 'a') goto yy122;
				goto yy5;
			} else {
				if (yych <= 'h') goto yy121;
				if (yych == 's') goto yy120;
				goto yy5;
			}
yy23:
			yych = *(YYMARKER = ++YYCURSOR);
			if (yych == 'a') goto yy116;
			if (yych == 'l') goto yy115;
			goto yy5;
yy24:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy113;
			goto yy5;
yy25:
			yych = *(YYMARKER = ++YYCURSOR);
			switch (yych) {
			case 'a':	goto yy92;
			case 'e':	goto yy94;
			case 'g':	goto yy97;
			case 'j':	goto yy96;
			case 'n':	goto yy95;
			case 'u':	goto yy91;
			default:	goto yy5;
			}
yy26:
			yych = *(YYMARKER = ++YYCURSOR);
			if (yych == 'a') goto yy87;
			if (yych == 'h') goto yy86;
			goto yy5;
yy27:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy84;
			goto yy5;
yy28:
			yych = *(YYMARKER = ++YYCURSOR);
			if (yych <= 'e') {
				if (yych == 'a') goto yy73;
				if (yych <= 'd') goto yy5;
				goto yy72;
			} else {
				if (yych <= 'r') {
					if (yych <= 'q') goto yy5;
					goto yy70;
				} else {
					if (yych == 'x') goto yy71;
					goto yy5;
				}
			}
yy29:
			yych = *(YYMARKER = ++YYCURSOR);
			if (yych == 'a') goto yy65;
			if (yych == 'h') goto yy67;
			goto yy5;
yy30:
			yych = *(YYMARKER = ++YYCURSOR);
			if (yych <= 'g') {
				if (yych == 'a') goto yy56;
				goto yy5;
			} else {
				if (yych <= 'h') goto yy55;
				if (yych == 't') goto yy54;
				goto yy5;
			}
yy31:
			yych = *(YYMARKER = ++YYCURSOR);
			if (yych == 'a') goto yy48;
			if (yych == 'e') goto yy47;
			goto yy5;
yy32:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy45;
			goto yy5;
yy33:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy41;
			goto yy5;
yy34:
			yych = *(YYMARKER = ++YYCURSOR);
			if (yych == 'a') goto yy35;
			if (yych == 'h') goto yy37;
			goto yy5;
yy35:
			++YYCURSOR;
			{ COMBINE(50); continue; }
yy37:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy39;
yy38:
			YYCURSOR = YYMARKER;
			goto yy5;
yy39:
			++YYCURSOR;
			{ COMBINE(37); continue; }
yy41:
			++YYCURSOR;
			if ((yych = *YYCURSOR) == 'b') goto yy43;
			{ COMBINE(28); continue; }
yy43:
			++YYCURSOR;
			{ COMBINE(29); continue; }
yy45:
			++YYCURSOR;
			{ COMBINE(48); continue; }
yy47:
			yych = *++YYCURSOR;
			if (yych == 'd') goto yy50;
			goto yy38;
yy48:
			++YYCURSOR;
			{ COMBINE(38); continue; }
yy50:
			yych = *++YYCURSOR;
			if (yych != 'i') goto yy38;
			yych = *++YYCURSOR;
			if (yych != 'c') goto yy38;
			++YYCURSOR;
			{ COMBINE(53); continue; }
yy54:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy60;
			if (yych == 'h') goto yy62;
			goto yy38;
yy55:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy58;
			goto yy38;
yy56:
			++YYCURSOR;
			{ COMBINE(11); continue; }
yy58:
			++YYCURSOR;
			{ COMBINE(16); continue; }
yy60:
			++YYCURSOR;
			{ COMBINE(12); continue; }
yy62:
			yych = *++YYCURSOR;
			if (yych != 'a') goto yy38;
			++YYCURSOR;
			{ COMBINE(17); continue; }
yy65:
			++YYCURSOR;
			{ COMBINE(41); continue; }
yy67:
			yych = *++YYCURSOR;
			if (yych != 'a') goto yy38;
			++YYCURSOR;
			{ COMBINE(40); continue; }
yy70:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy82;
			goto yy38;
yy71:
			yych = *++YYCURSOR;
			if (yych == 'l') goto yy77;
			if (yych == 'x') goto yy79;
			goto yy38;
yy72:
			yych = *++YYCURSOR;
			if (yych == 'x') goto yy75;
			goto yy38;
yy73:
			++YYCURSOR;
			{ COMBINE(30); continue; }
yy75:
			++YYCURSOR;
			{ COMBINE(31); continue; }
yy77:
			++YYCURSOR;
			{ COMBINE(32); continue; }
yy79:
			yych = *++YYCURSOR;
			if (yych != 'l') goto yy38;
			++YYCURSOR;
			{ COMBINE(33); continue; }
yy82:
			++YYCURSOR;
			{ COMBINE(34); continue; }
yy84:
			++YYCURSOR;
			{ COMBINE(47); continue; }
yy86:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy89;
			goto yy38;
yy87:
			++YYCURSOR;
			{ COMBINE(23); continue; }
yy89:
			++YYCURSOR;
			{ COMBINE(24); continue; }
yy91:
			yych = *++YYCURSOR;
			if (yych == 'k') goto yy108;
			goto yy38;
yy92:
			++YYCURSOR;
			if ((yych = *YYCURSOR) == 's') goto yy106;
			{ COMBINE(20); continue; }
yy94:
			yych = *++YYCURSOR;
			if (yych == 'x') goto yy104;
			goto yy38;
yy95:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy102;
			goto yy38;
yy96:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy100;
			goto yy38;
yy97:
			yych = *++YYCURSOR;
			if (yych != 'a') goto yy38;
			++YYCURSOR;
			{ COMBINE(5); continue; }
yy100:
			++YYCURSOR;
			{ COMBINE(10); continue; }
yy102:
			++YYCURSOR;
			{ COMBINE(15); continue; }
yy104:
			++YYCURSOR;
			{ COMBINE(21); continue; }
yy106:
			++YYCURSOR;
			{ COMBINE(22); continue; }
yy108:
			yych = *++YYCURSOR;
			if (yych != 'h') goto yy38;
			yych = *++YYCURSOR;
			if (yych != 't') goto yy38;
			yych = *++YYCURSOR;
			if (yych != 'a') goto yy38;
			++YYCURSOR;
			{ COMBINE(54); continue; }
yy113:
			++YYCURSOR;
			{ COMBINE(27); continue; }
yy115:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy118;
			goto yy38;
yy116:
			++YYCURSOR;
			{ COMBINE(35); continue; }
yy118:
			++YYCURSOR;
			{ COMBINE(36); continue; }
yy120:
			yych = *++YYCURSOR;
			if (yych == 'h') goto yy126;
			goto yy38;
yy121:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy124;
			goto yy38;
yy122:
			++YYCURSOR;
			{ COMBINE(1); continue; }
yy124:
			++YYCURSOR;
			{ COMBINE(2); continue; }
yy126:
			yych = *++YYCURSOR;
			if (yych != 'a') goto yy38;
			++YYCURSOR;
			{ COMBINE(45); continue; }
yy129:
			yych = *++YYCURSOR;
			if (yych == 'y') goto yy135;
			goto yy38;
yy130:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy133;
			goto yy38;
yy131:
			++YYCURSOR;
			{ COMBINE(8); continue; }
yy133:
			++YYCURSOR;
			{ COMBINE(9); continue; }
yy135:
			yych = *++YYCURSOR;
			if (yych != 'a') goto yy38;
			++YYCURSOR;
			{ COMBINE(46); continue; }
yy138:
			++YYCURSOR;
			{ COMBINE(42); continue; }
yy140:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy146;
			goto yy38;
yy141:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy144;
			goto yy38;
yy142:
			++YYCURSOR;
			{ COMBINE(3); continue; }
yy144:
			++YYCURSOR;
			{ COMBINE(4); continue; }
yy146:
			++YYCURSOR;
			{ COMBINE(49); continue; }
yy148:
			++YYCURSOR;
			{ COMBINE(51); continue; }
yy150:
			yych = *++YYCURSOR;
			if (yych <= 'g') {
				if (yych == 'a') goto yy156;
				goto yy38;
			} else {
				if (yych <= 'h') goto yy158;
				if (yych == 'r') goto yy159;
				goto yy38;
			}
yy151:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy154;
			goto yy38;
yy152:
			++YYCURSOR;
			{ COMBINE(18); continue; }
yy154:
			++YYCURSOR;
			{ COMBINE(19); continue; }
yy156:
			++YYCURSOR;
			{ COMBINE(13); continue; }
yy158:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy165;
			goto yy38;
yy159:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy161;
			if (yych != 'h') goto yy38;
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy163;
			goto yy38;
yy161:
			++YYCURSOR;
			{ COMBINE(43); continue; }
yy163:
			++YYCURSOR;
			{ COMBINE(44); continue; }
yy165:
			++YYCURSOR;
			{ COMBINE(14); continue; }
yy167:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy176;
			goto yy38;
yy168:
			++YYCURSOR;
			{ COMBINE(39); continue; }
yy170:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy171;
			if (yych == 'h') goto yy173;
			goto yy38;
yy171:
			++YYCURSOR;
			{ COMBINE(6); continue; }
yy173:
			yych = *++YYCURSOR;
			if (yych != 'a') goto yy38;
			++YYCURSOR;
			{ COMBINE(7); continue; }
yy176:
			++YYCURSOR;
			{ COMBINE(52); continue; }
yy178:
			yych = *++YYCURSOR;
			if (yych == 'a') goto yy181;
			goto yy38;
yy179:
			++YYCURSOR;
			{ COMBINE(25); continue; }
yy181:
			++YYCURSOR;
			{ COMBINE(26); continue; }
yy183:
			++YYCURSOR;
			{ continue; }
yy185:
			++YYCURSOR;
			if (YYLIMIT <= YYCURSOR) YYFILL(1);
			yych = *YYCURSOR;
			if (yych == '.') goto yy183;
			if (yych <= '/') goto yy38;
			if (yych <= '9') goto yy185;
			goto yy38;
yy187:
			++YYCURSOR;
			if (YYLIMIT <= YYCURSOR) YYFILL(1);
			yych = *YYCURSOR;
yy188:
			if (yych <= '\r') {
				if (yych <= 0x08) goto yy3;
				if (yych <= '\n') goto yy187;
				if (yych <= '\f') goto yy3;
				goto yy187;
			} else {
				if (yych <= ' ') {
					if (yych <= 0x1F) goto yy3;
					goto yy187;
				} else {
					if (yych == ',') goto yy187;
					goto yy3;
				}
			}
		}

    }
    return 0;
}
