/*************************************************************
 * imli_private.h
 *
 * This file is part of libimli
 *
 * Licensed under the GNU Library General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#ifndef __IMLI_PRIVATE_H__
#define __IMLI_PRIVATE_H__

#include "libimli.h"

#ifdef WIN32
#define strdup _strdup
#define strcasecmp _stricmp
#endif

#define N_CONSONANTS 55

typedef struct script_info *script_info_t;

typedef struct imli_input_method *imli_input_method_t;
/* one for each supported script */
struct script_info
{
    imli_script_t script_code;
    byte_t **glyphs;
    byte_t  *data;
    imli_input_method_t im;
};

typedef struct _iscii_imli_map_rec
{
    imli_char_type_t type;
    byte_t value;
    imli_compose_flag_t flags;
} iscii_imli_map_rec;

typedef struct iscii_context
{
    iscii_imli_map_rec iscii_imli_map[256];
    byte_t* vowels;
    byte_t* consonants;
    byte_t* vowels_m_form;
    byte_t* specials;
    byte_t cur_lang;
    int nvowels;
    int nconsonants;
    int nspecials;
} *iscii_t;

typedef struct cnj_entry
{
    char count;
    unsigned int conjuncts[64];
} cnj_entry_t;

struct imli_context
{
    cnj_entry_t conjunct_table[64];
    script_info_t scripts[SCRIPT_MAX];
    imli_script_t active_script;    
    char *base_dir;
    iscii_t iscii;
    imli_status_t error_status;
    byte_t nconsonants;
    int valid; // set to 0 if invalid
};

typedef syl_t (*imli_compose_fn_t)(imli_context_t ctx,
				   imli_input_method_t im,
				   char input,
				   imli_script_t script_prev_syl,
				   syl_t prev_syl,
				   imli_compose_flag_t flags,
				   int *delete_prev_syl);

struct imli_input_method
{
    char codes[128];
    imli_char_type_t types[128];
    imli_keyboard_t keyboard;
    imli_compose_fn_t compose;
    unsigned long tag; 		/* just in case */
};

/* conf.c */
char* get_home_dir();

/* cnj.c */
int init_conjunct_tables (imli_context_t);

/* im.c */
int load_keymap(imli_input_method_t im, const char *basedir, const char *file);

/* inscript.c */
imli_input_method_t 
open_inscript_input_method (imli_context_t ctx, imli_script_t script);

/* phonetic.c */
imli_input_method_t
open_phonetic_input_method (imli_context_t ctx, imli_script_t script);

#endif /* __IMLI_PRIVATE_H__ */
