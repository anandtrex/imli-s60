/*************************************************************
 * im.c
 *
 * This file is part of libimli
 *
 * Licensed under the GNU Library General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#define __IMLI_IMPL__
#include "libimli.h"
#include "imli_private.h"
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
/*!
  \file imli.h
*/

/*!
  \typedef \enum imli_char_type imli_char_type_t
  \brief Character types known to the library

  imli_char_type_t is used to indicate the type of the values that is
  returned by the input methods routines.

  \li \c TYPE_INVALID             - Invalid value
  \li \c TYPE_CONSONANT           - The associated value is a consonant
  \li \c TYPE_VOWEL               - The associated value is a vowel
  \li \c TYPE_SPECIAL             - The associated value is a special character
  \li \c TYPE_ASCII               - The associated value is an ASCII character
*/

/*!
  \typedef \enum imli_compose_flag imli_compose_flag_t
  \brief Flags that can be used to influence syllable composition

  This enum is used to influence syllable composition.
  \li \c FLAG_NONE               - Do nothing
  \li \c FLAG_STANDALONE_VOWEL   - Do not combine with the previous syllabe and insert a standalone vowel
  \li \c FLAG_INSERT_HALANT      - Insert an additional halant. Useful in certain cases.

  \sa imli_syl_compose where it is used.
*/

/*!
  \typedef imli_keyboard_t
  \brief The list of keyinput methods understood by the library.
  
  \li \c INSCRIPT - the inscript keyboard layout
  \li \c PHONETIC - the phonetic keyboard layout
*/

/*!
  \brief select the input method to use for the currently
  selected script.
*/

EXPORT_C imli_status_t 
imli_select_input_method(imli_context_t ctx,
			 imli_keyboard_t kb)
{
    imli_input_method_t im;
    if (ctx->active_script == SCRIPT_UNSUPPORTED ||
        ctx->active_script == SCRIPT_ASCII)
    {
        return IMLI_STATUS_OK;
    }
    
    script_info_t sinfo = ctx->scripts[ctx->active_script];
    
    if (!sinfo) return IMLI_STATUS_SCRIPT_UNINITIALIZED;

    if (sinfo->im && sinfo->im->keyboard == kb) 
	return IMLI_STATUS_OK;

    if (kb == INSCRIPT)
    	im = open_inscript_input_method(ctx, ctx->active_script);
    else
	im = open_phonetic_input_method(ctx, ctx->active_script);

    if (!im) return IMLI_STATUS_DATA_NOT_FOUND;

    if (sinfo->im) free(sinfo->im);
    sinfo->im = im;

    return IMLI_STATUS_OK;
}

/*!
  \brief Get the currently selected input method
*/

EXPORT_C imli_keyboard_t imli_get_active_input_method (imli_context_t ctx)
{
    imli_keyboard_t kb = KB_NONE;
    
    if (ctx->active_script == SCRIPT_UNSUPPORTED ||
        ctx->active_script == SCRIPT_ASCII)
        return kb;
    
    script_info_t sinfo = ctx->scripts[ctx->active_script];
    if (sinfo && sinfo->im)
        kb = sinfo->im->keyboard;

    return kb;
}

/*!
  \brief Dump the tables of the currently selected input method to FILE pointer \a fp 
*/
EXPORT_C void dump_active_input_method (imli_context_t ctx, FILE *fp)
{
    imli_input_method_t im;
    if (ctx->active_script == SCRIPT_UNSUPPORTED ||
        ctx->active_script == SCRIPT_ASCII)
    {
        return;
    }
    
    script_info_t sinfo = ctx->scripts[ctx->active_script];
    
    if (sinfo && sinfo->im)
    {
	char types[] = {'C', 'V', 'S', 'A'};
	int i = 0;
	im = sinfo->im;
	
	for (; i < 128; i++)
	{
	    if (im->types[i] == TYPE_INVALID || im->types[i] == TYPE_ASCII)
		continue;
	    
	    fprintf(fp, "%c\t%d\t%c\n", i, im->codes[i], types[im->types[i]]);
	}
    }
}

#define CHECK_AND_SKIP_WS(str, val)		\
    do {					\
	if (!*str)				\
	    return val;				\
	if (!g_unichar_isspace(*str))			\
	    break;				\
        str++;					\
    } while (1)
	
/*
 \brief Helper function to parse a line from the keymap configuration file.
*/
static int parse_keymap_line (imli_input_method_t im, char *line)
{
    char key = 0;
    char code = 0;
    imli_char_type_t type = TYPE_INVALID;
    
    // skip leading white space
    CHECK_AND_SKIP_WS(line, 0);
    // comments; ignore the rest of the line
    if (*line == '/' && *(line + 1) == '/')
	return 0;
    // key - only printable ascii allowed
    if (*line > 32)
    {
	key = *line++;
	CHECK_AND_SKIP_WS(line, -1);
    }
    else
    {
	return -1;
    }
    
    // code - only digits allowed
    while (g_unichar_isdigit(*line))
    {
	code = code * 10 + (*line - '0');
	line++;
    }
    
    CHECK_AND_SKIP_WS(line, -1);
    
    // type
    if (*line == 'C')
	type = TYPE_CONSONANT;
    else if (*line == 'V')
	type = TYPE_VOWEL;
    else if (*line == 'S')
	type = TYPE_SPECIAL;
    else if (*line == 'A')
	type = TYPE_ASCII;
    else if (*line == 'I')
	type = TYPE_INVALID;
    else
	return -1;
    
    // ignore the rest of the line
    im->codes[(int)key] = code;
    im->types[(int)key] = type;
    return 0;
}

/*
 \brief Helper function to load a keymap configuration file. Used by
 the input method implementations.
*/
int load_keymap(imli_input_method_t im, const char *basedir, const char *file)
{
    int i;
    char fullpath[128];
    snprintf(fullpath, sizeof(fullpath), "%s\\langfiles\\%s", basedir, file); //TODO needs to be backslash
    FILE *fp = fopen(fullpath, "r");
    if (!fp)
    	return -1;
    
    for(i = 0; i <= 32; i++)
    {
	im->codes[i] = i;
	im->types[i] = TYPE_ASCII;
    }
    
    for(i = 33; i < 128; i++)	
    {
	im->codes[i] = 0;
	im->types[i] = TYPE_INVALID;
    }
    
    char buf[128];
    while (!feof(fp))
    {
	fgets(buf, sizeof(buf), fp);
	char include_file[128];
	if (sscanf(buf, "include %s\n", include_file) == 1)
	{
	    load_keymap(im, basedir, include_file);
	    continue;
	}
	if (parse_keymap_line(im, buf) != 0)
	    fprintf(stderr, "Debug: malformed line in file: %s\nLine: %s\n", file, buf);
    }
    fclose(fp);
    return 0;
}

/*!
  \brief process a keystroke using the currently selected input method
  and script.
*/

EXPORT_C syl_t imli_process_key (imli_context_t ctx,
			char key,
			imli_script_t script_prev_syl,
			syl_t prev_syl,
			imli_compose_flag_t flags,
			int *delete_prev_syl)
{
    script_info_t sinfo = ctx->scripts[ctx->active_script];
    if (!sinfo)
    {
	ctx->error_status = IMLI_STATUS_SCRIPT_UNINITIALIZED;
	return SYL_INVALID;
    }
    
    if (!sinfo->im)
    {
	ctx->error_status = IMLI_STATUS_INPUT_METHOD_INVALID;
	return SYL_INVALID;
    }

    return sinfo->im->compose(ctx, sinfo->im, key, script_prev_syl, prev_syl, flags, delete_prev_syl);
}


static const byte_t HALANT = 15;
static const byte_t NUKHTA = 54;
static const byte_t KA     = 1;
static const byte_t SHA    = 40;
static const byte_t KSHA   = 45;
static const byte_t JA     = 8;
static const byte_t NJA    = 10;
static const byte_t JNYA   = 46;
static const byte_t NA     = 20;
static const byte_t NEX    = 21; /* NEX = Na Extended */
static const byte_t RA     = 30;
static const byte_t REX    = 31; /* REX = Ra EXtended */
static const byte_t RXL    = 32; /* RXL = Ra EXtended Extended*/
static const byte_t RXXL   = 33; /* RXXL = Ra EXtended Extended Extended */


/*!
 \brief generic compose function for input methods
 
 This is the generic compose function that includes all the rules of
 syllable composition in IMLI. The \a input, an IMLI character code 
 (0-63) is passed along with its type \a type . Based on the type of
 character the function either builds a completely new syllable or returns 
 a syllable that is composed from \a prev_syl and \a input in which case
 \a delete_prev_syl is set to 1 meaning that the \a prev_syl is now 
 invalid and should be removed from the buffer. If \a prev_syl is invalid
 (ie, 0xFF) then the function simply returns a new syllable with 
 \a delete_prev_syl set to 0.

 \param ctx - the current IMLI context
 \param input - the IMLI character. Typically in the range 0-63
 \param type  - the type of the IMLI character that was passed in \a input
 \param script_prev_syl - the script of the previous syllable
 \param prev_syl - the previous syllable
 \param flag  - flags if any can be passed by 'or'ing them to influence the composition
 \param [out] delete_prev_syl - upon return will indicate whether the previous syllable is to be deleted or not.
 \returns a new valid syllable on success, SYL_INVALID on error.
*/

EXPORT_C syl_t imli_syl_compose (imli_context_t ctx,
			byte_t input, imli_char_type_t type,
			imli_script_t script_prev_syl,
			syl_t prev_syl,
			imli_compose_flag_t flag,
			int *delete_prev_syl)
{
    byte_t c, cons1, cons2, cons3, cons4, v;
    syl_t syl, temp;
    *delete_prev_syl = 0;
    syl = SYL_INVALID;
    
    switch ( type )
    {
    case TYPE_INVALID:
	break;
    case TYPE_SPECIAL:
	//printf("Type = SPECIAL, input = %d\n", input);
	syl = (SPECIAL_START | input);
	break;
    case TYPE_VOWEL:
	//printf("Type = VOWEL, input = %d\n", input);
	syl = (input & VOWEL_MASK);

	/* Check if the previous character was a consonant with "ah" as vowel */
	if (prev_syl == SYL_INVALID) 
	    return syl;

	/* Don't combine across scripts */
	if (imli_get_active_script(ctx) != script_prev_syl)
	    return syl;

	/* Don't combine with special and ascii syllables */
	if (IS_SYL_ASCII(prev_syl) || ( IS_SYL_SPECIAL(prev_syl)))
	    return syl;

	c = CONSONANT(prev_syl);
	v = VOWEL(prev_syl);

	/* useful for implementing vowel rule */
	if (flag == FLAG_STANDALONE_VOWEL)
	{
	    /* delete orphaned halants */
	    if (v == HALANT && c == 0)
		*delete_prev_syl = 1;
	    return syl;
	}

	/* non-joiner */
	if (v == HALANT && input == HALANT && c != 0)
	    return syl;

	if (c && ( v == 0))
	{
	    syl = prev_syl | input;
	    *delete_prev_syl = 1;
	}
	return syl;

    case TYPE_CONSONANT:
	//printf("Type = CONSONANT, input = %d\n", input);
	syl = SYLLABLE(input, 0, 0); /* By default */

	/* Don't combine across scripts */
	if (imli_get_active_script(ctx) != script_prev_syl)
	    return syl;

	/* Nothing much to do if there was no prev syl_t or if the prev syl_t
	 * was a special, vowel or english
	 */
	if ((prev_syl == SYL_INVALID) ||
	    (IS_SYL_ASCII(prev_syl))  ||   /* English    */
	    (IS_SYL_SPECIAL(prev_syl)))    /* Special    */
	{
	    return syl;
	}
	
	v = VOWEL(prev_syl);
	c = CONSONANT(prev_syl);

	/* FIXME: handle nukhta using only 2 keys 
	 *  Old code follows:
	 */
/* 	if (key == ']' && v == 0) */
/* 	    v = HALANT; */

        if (flag == FLAG_INSERT_HALANT && v == 0)
            v = HALANT;

	if (input == NUKHTA && v == 0)
	    v = HALANT;

	/* Combine only if there is a halant */
	if (v != HALANT) return syl;

	/* remove standalone halant */
	if (c == 0 )
	{
	    *delete_prev_syl = 1;
	    return syl;
	}
	
	if (imli_is_syl_full(ctx, prev_syl, &cons1, &cons2, &cons3, &cons4))
	    return syl;

	if (cons2 == 0)
	{
	    /* create a new conjunct */
	    *delete_prev_syl = 1;

	    // hardcoded syllables for ksha and jnya (promoted consonants)
	    if (c == KA && input == SHA ) // ksha
		return SYLLABLE(KSHA, 0, 0);
	    else if (c == JA && input == NJA ) // jnya
		return SYLLABLE(JNYA, 0, 0);
            else if (c == NA)
	    {
		 /* if na fails, check in nex */
		temp = imli_get_conjunct_code (ctx, c, input, 0, 0);
		if (temp != 0xFFFF) 
		    return temp; 
		else 
		    c = NEX;

		temp = imli_get_conjunct_code (ctx, c, input, 0, 0);
		if (temp != 0xFFFF) 
		    return temp; 
		else 
		    *delete_prev_syl = 0;
                return syl;
	    }
	    else if (c == RA)
	    {
		 /* if ra fails, check in rex */
		temp = imli_get_conjunct_code (ctx, c, input, 0, 0);
		if (temp != 0xFFFF) 
                {
		    return temp; 
                }
                c = REX;
                temp = imli_get_conjunct_code (ctx, c, input, 0, 0);
                if (temp != 0xFFFF)
                    return temp;
                
                c = RXL;
                temp = imli_get_conjunct_code(ctx, c, input, 0, 0);
                if (temp != 0xFFFF)
                    return temp;
                    
                c = RXXL;
	    }
	    temp = imli_get_conjunct_code(ctx, c, input, 0, 0);
	    if (temp != 0xFFFF ) 
		return temp;
	    else
		*delete_prev_syl = 0;
	    return syl;
	}

	if (cons3 == 0)
	{
	    temp = imli_get_conjunct_code(ctx, c, cons2, input, 0);
	    if (temp != 0xFFFF )
	    {
		*delete_prev_syl = 1;
		return temp;
	    }
            else 
            {
                *delete_prev_syl = 0;
                return syl;
            }
	}

	if (cons4 == 0)
	{
	    temp = imli_get_conjunct_code(ctx, c, cons2, cons3, input);
	    if (temp != 0xFFFF )
	    {
		*delete_prev_syl = 1;
		return temp;
	    }
        }
        
	*delete_prev_syl = 0;
	return syl;
    default:
        break;
    }
    return syl;
}
