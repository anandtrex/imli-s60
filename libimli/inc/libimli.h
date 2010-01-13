/*************************************************************
 * imli.h
 *
 * This file is part of libimli
 *
 * Licensed under the GNU Library General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#ifndef __LIBIMLI_H__
#define __LIBIMLI_H__

//#include <e32base.h>	// CBase
//#include <e32std.h>	 // TBuf

/*
#ifndef __IMLI_IMPL__
#define __IMLI_IMPL__
*/

#ifdef __cplusplus
extern "C" {
#endif
/*
#ifdef WIN32
    #ifdef __IMLI_IMPL__
        #define IMPORT_C extern __declspec(dllexport)
    #else
        #define IMPORT_C extern __declspec(dllimport)
    #endif
#else
    #define IMPORT_C
#endif*/
//#ifdef __IMLI_IMPL__
//	#define IMPORT_C IMPORT_C
//#else
//	#define IMPORT_C //IMPORT_C
//#endif
//#define IMPORT_C IMPORT_C

#include <stddef.h>
#include <stdio.h>

typedef unsigned char  byte_t;
typedef unsigned short syl_t;

#define MAX_CONSONANTS  64
#define MAX_CONJUNCTS   64
#define MAX_VOWELS      16

#define CONSONANT_MASK 0xFC00
#define CONJUNCT_MASK  0x03F0
#define VOWEL_MASK     0x000F

#define ASCII_MSB      0xFB
#define SPECIAL_MSB    0xFC
#define ASCII_START    (0xFB << 8)
#define SPECIAL_START  (0xFC << 8)

#define SYL_INVALID    0xFFFF
#define SYL_TERMINATOR 0xFB00
#define SYL_NEWLINE    0xFB0A
#define SYL_SPACE      0xFB20
#define SYL_TAB        0xFB09

#define SYLLABLE(consonant, conjunct, vowel)				\
    (((consonant) << 10) | (((conjunct) << 4) & CONJUNCT_MASK) |	\
     ((vowel) & VOWEL_MASK))

#define CONSONANT(syl) (((syl) & CONSONANT_MASK) >> 10)
#define CONJUNCT(syl)  (((syl) & CONJUNCT_MASK) >> 4)
#define VOWEL(syl)     (((syl) & VOWEL_MASK))

#define IS_SYL_SPECIAL(syl) (((syl) & 0xFF00) == SPECIAL_START)
#define IS_SYL_ASCII(syl)   (((syl) & 0xFF00) == ASCII_START)

#define SWITCH_CODE 0xFA00
#define SWITCH_MASK 0xFF00
#define IS_SYL_SWITCH(syl)  (((syl) & SWITCH_MASK) == SWITCH_CODE)

#define LANGCODE_MASK 0x00FF

#define ATTR_SYL_STOP          1
#define ATTR_SYL_WHITESPACE    2
#define ATTR_SYL_WORD_STOP     4

typedef struct imli_context* imli_context_t;

typedef enum imli_status
{
    IMLI_STATUS_OK = 0,
    IMLI_STATUS_NO_MEMORY,
    IMLI_STATUS_INVALID_CONTEXT,
    IMLI_STATUS_DATA_NOT_FOUND,
    IMLI_STATUS_GLYPH_NOT_FOUND,
    IMLI_STATUS_BUFFER_NO_ROOM,
    IMLI_STATUS_ISCII_DATA_NOT_FOUND,
    IMLI_STATUS_BAD_INPUT,
    IMLI_STATUS_SCRIPT_UNINITIALIZED,
    IMLI_STATUS_INPUT_METHOD_INVALID
} imli_status_t;

typedef enum imli_script
{
    SCRIPT_UNSUPPORTED = -1,
    SCRIPT_SANSKRIT    = 0,
    SCRIPT_HINDI,
    SCRIPT_TAMIL,
    SCRIPT_TELUGU,
    SCRIPT_KANNADA,
    SCRIPT_MALAYALAM,
    SCRIPT_ORIYA,
    SCRIPT_BANGLA,
    SCRIPT_GUJARATI,
    SCRIPT_PUNJABI,
    SCRIPT_ASAMIYA,
    SCRIPT_UNUSED1,
    SCRIPT_MARATHI,
    SCRIPT_BRAILLE,
    SCRIPT_ROMANTRANS,
    SCRIPT_MAX,
    SCRIPT_ASCII = 25
} imli_script_t;

typedef enum imli_char_type
{
    TYPE_INVALID   = -1,
    TYPE_CONSONANT = 0,
    TYPE_VOWEL,
    TYPE_SPECIAL,
    TYPE_ASCII
} imli_char_type_t;

typedef enum imli_compose_flag
{
    FLAG_NONE             = 0,
    FLAG_STANDALONE_VOWEL = 1,
    FLAG_INSERT_HALANT    = 2
} imli_compose_flag_t;

typedef enum
{
    KB_NONE = -1,
    INSCRIPT = 1,
    PHONETIC
} imli_keyboard_t;

typedef struct script_item_run
{
    imli_script_t script_code;
    size_t offset;
    size_t length;
} script_item_run_t;

IMPORT_C imli_status_t imli_context_new  (imli_context_t* ctx, const char *data_dir);
IMPORT_C imli_status_t imli_context_free (imli_context_t);
IMPORT_C int           imli_is_context_valid (imli_context_t ctx);
IMPORT_C char*         imli_home_dir     (imli_context_t ctx);

IMPORT_C const char*   imli_script_name (imli_script_t script);
IMPORT_C imli_script_t imli_script_code (const char* script_name);

IMPORT_C int imli_get_consonant_count(imli_context_t ctx);

IMPORT_C int imli_get_conjunct_count(imli_context_t ctx, byte_t consonant);

IMPORT_C syl_t imli_get_conjunct_code (imli_context_t ctx, byte_t consonant, byte_t conjunct2, byte_t conjunct3, byte_t conjunct4 );

IMPORT_C int imli_is_syl_full ( imli_context_t ctx, syl_t l, byte_t *c1, byte_t *c2, byte_t *c3, byte_t *c4 );

IMPORT_C imli_status_t imli_init_scripts(imli_context_t ctx, ...);

IMPORT_C imli_status_t imli_select_script(imli_context_t ctx, imli_script_t script);

IMPORT_C imli_script_t imli_get_active_script(imli_context_t ctx);

IMPORT_C void dump_tab(imli_context_t, imli_script_t);

IMPORT_C imli_status_t syl_to_glyphs(imli_context_t ctx, 
				   syl_t syl, imli_script_t script,
				   byte_t *outbuf, size_t capacity, 
				   size_t *length);

IMPORT_C imli_status_t syl_width(imli_context_t ctx, 
			       syl_t syl, imli_script_t script,
			       byte_t *length);

IMPORT_C imli_status_t syl_to_glyphs_with_attr (imli_context_t ctx, 
					      syl_t syl, imli_script_t script,
					      byte_t *outbuf, byte_t *attrbuf,
					      size_t capacity, 
					      size_t *length);

IMPORT_C imli_status_t syls_to_glyphs(imli_context_t ctx, 
				    const syl_t *text, size_t len,
				    imli_script_t script,
				    byte_t *outbuf, 
				    size_t capacity,
				    size_t *length);

IMPORT_C imli_status_t syls_to_glyphs_with_attr (imli_context_t ctx, 
					       const syl_t *text, size_t len,
					       imli_script_t script,
					       byte_t *outbuf, 
					       byte_t *attrbuf,
					       size_t capacity,
					       size_t *length);

IMPORT_C imli_status_t imli_script_itemize (imli_context_t ctx,
					  const syl_t *text, size_t len,
					  script_item_run_t **runs,
					  size_t *nruns);

IMPORT_C imli_status_t init_iscii_tables (imli_context_t ctx);

IMPORT_C byte_t iscii_code_from_script (imli_script_t script);

IMPORT_C imli_script_t script_from_iscii_code (byte_t code);

IMPORT_C imli_status_t imli_to_iscii (imli_context_t ctx, imli_script_t script,
				    const syl_t* inbuf, size_t insize, 
				    byte_t* outbuf, size_t* outsize);

IMPORT_C imli_status_t iscii_to_imli (imli_context_t ctx, 
				    byte_t iscii, 
				    byte_t *imli, 
				    imli_char_type_t *type,
				    imli_compose_flag_t *flags);

IMPORT_C imli_status_t imli_select_input_method (imli_context_t ctx,
					       imli_keyboard_t kb);

IMPORT_C imli_keyboard_t imli_get_active_input_method (imli_context_t ctx);

IMPORT_C syl_t imli_process_key (imli_context_t ctx,
			       char key,
			       imli_script_t script_prev_syl,
			       syl_t prev_syl,
			       imli_compose_flag_t flags,
			       int *delete_prev_syl);

IMPORT_C syl_t imli_syl_compose (imli_context_t ctx,
			       byte_t input, imli_char_type_t type,
			       imli_script_t script_prev_syl,
			       syl_t prev_syl,
			       imli_compose_flag_t flags,
			       int *delete_prev_syl);

IMPORT_C void dump_conjunct_table(imli_context_t ctx, FILE *fp);

IMPORT_C void dump_active_input_method(imli_context_t ctx, FILE *fp);
    
#ifdef __cplusplus
}
#endif

#endif
