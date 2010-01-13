/*************************************************************
 * phonetic.c
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

syl_t phonetic_process_key(imli_context_t ctx,
			   imli_input_method_t im, 
			   char key, 
			   imli_script_t script_prev_syl,
			   syl_t prev_syl,			  
			   imli_compose_flag_t flags,
			   int *delete_prev_syl);

static imli_input_method_t 
open_phonetic_input_method_compat (imli_context_t ctx, imli_script_t script);

imli_input_method_t 
open_phonetic_input_method (imli_context_t ctx, imli_script_t script)
{
    char buf[128];
    char *basedir = imli_home_dir(ctx);
    const char *script_name = imli_script_name(script);
    imli_input_method_t im = (imli_input_method_t)malloc(sizeof(struct imli_input_method));
    if (!im) return NULL;
    im->keyboard = PHONETIC;
    im->compose  = phonetic_process_key;
    snprintf(buf, sizeof(buf), "%s_phonetic.txt", 
	     script_name);
    if (load_keymap(im, basedir, buf) == 0)
    {
	im->tag = 1;
    }
    else
    {
	free(im);
	im = open_phonetic_input_method_compat(ctx, script);
    }
    free(basedir);
    return im;
}

typedef struct
{
    unsigned short type;
    byte_t         code;
    byte_t         unused;
} key_entry_t;

/* for older .key files */
static imli_input_method_t 
open_phonetic_input_method_compat (imli_context_t ctx, imli_script_t script)
{
    char path[128];
    FILE *fp;
    char *basedir = imli_home_dir(ctx);
    const char *script_name = imli_script_name(script);
    key_entry_t entries[128];
    imli_input_method_t im = NULL;
    int i;
    snprintf(path, sizeof(path), "%s\\langfiles\\%s_phonetic.key", 
	     basedir, 
	     script_name);
    if ((fp = fopen(path, "rb")) == NULL)
    {
	free(basedir);
	return NULL;
    }

    if (fread(entries, sizeof(key_entry_t), 128, fp) != 128)
    {
	fclose(fp);
	return NULL;
    }
    fclose(fp);
    free(basedir);
    im = (imli_input_method_t)malloc(sizeof(struct imli_input_method));
    if (!im) return NULL;
    im->keyboard = PHONETIC;
    im->compose  = phonetic_process_key;
    
    for(i = 0; i < 128; i++)
    {
	im->codes[i] = entries[i].code;
	im->types[i] = (imli_char_type_t)entries[i].type;
    }
    im->tag = 1; 
    return im;
}
							    
syl_t phonetic_process_key(imli_context_t ctx,
			   imli_input_method_t im, 
			   char key, 
			   imli_script_t script_prev_syl,
			   syl_t prev_syl,			  
			   imli_compose_flag_t flags,
			   int *delete_prev_syl)
{
    *delete_prev_syl = 0;

    if (!im) return SYL_INVALID;
     
    return imli_syl_compose(ctx, 
			    im->codes[(int)key], im->types[(int)key],
			    script_prev_syl,
			    prev_syl,
			    flags,
			    delete_prev_syl);
}
