/*************************************************************
 * glyphs.c
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
#include <string.h>
#include <stdarg.h>
#include <assert.h>

typedef struct entry
{
    byte_t c;
    byte_t cnj;
    byte_t v;
    byte_t rhs[128];
    byte_t rhs_len;
} *entry_t;

static int parse_line (char *buf, entry_t e)
{
    char *p = buf;
    byte_t rhs[128];
    e->c = 0;
    e->cnj = 0;
    e->v = 0;
    memset(e->rhs, 0, sizeof(e->rhs));
    
    int idx = 0;
    
    if (*p++ != '[')
	return -1;
    
    memset(rhs, 0, sizeof(rhs));
    
    while(*p != ',')    
	e->c = e->c * 10 + (*p++ - '0');

    ++p;
    while(*p != ',')    
	e->cnj = e->cnj * 10 + (*p++ - '0');
    
    ++p;
    while(*p != ']')
	e->v = e->v * 10 + (*p++ - '0');

    p += 4;
    
    while(*p != '\0' && *p != '\r' && *p != '\n')
	e->rhs[idx++] = *p++;
    e->rhs_len = idx;

    return 0;
}

static int load_tab_file (imli_context_t ctx, imli_script_t script)
{
    char filename[256];
    const char *name = imli_script_name(script);
    if (!name) return -1;

    snprintf(filename, sizeof(filename), "%s\\tabfiles\\%s.tab",//TODO needs to be backslash here 
	     ctx->base_dir,imli_script_name(script));

    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
		fprintf(stderr, "Failed to open %s\n\n", filename);
		return -1;
    }
    script_info_t sinfo = ctx->scripts[script];
    assert(sinfo->glyphs == 0);
    if (!sinfo->glyphs)
    {
		sinfo->glyphs = (byte_t**) malloc(sizeof(byte_t*)*MAX_CONSONANTS*MAX_CONJUNCTS*MAX_VOWELS);
    }
    
    if(!sinfo->data)
    {
		sinfo->data = (byte_t*) malloc(sizeof(byte_t)*MAX_CONSONANTS*MAX_CONJUNCTS*MAX_VOWELS*64);
		if (!sinfo->data) return -1;
    }

    int c, cj, v;
    char rhs[128];
    char line[512];
    char *pline = line;
    
    byte_t *data = sinfo->data;
    while (!feof(fp))
    {
		c = cj = v = -1;
		memset(rhs, 0, sizeof(rhs));
		//getline(&pline, &n, fp);
		fgets(line, sizeof(line), fp);
		struct entry e;
		int retcode = parse_line(pline, &e);
		if (retcode < 0)
		{
			fprintf(stderr, "Warning: malformed line in tab file!\n");
			continue;
		}
		else if(retcode > 0)
		{
			fprintf(stderr, "Ignoring comment line...\n");
			continue;
		}
	
		byte_t len = e.rhs_len;
		syl_t index = SYLLABLE(e.c, e.cnj, e.v);
	
		if (len && (index != 0xFFFF))
		{
			sinfo->glyphs[index] = data;
			*data++ = len;
			memcpy(data, e.rhs, len);
			data += len;
		}
    }
    fclose(fp);
    fprintf(stderr, "DEBUG: load_tab_file(%d)\n", script);
    return 0;
}

/*!
  \file imli.h
*/

/*!
  \brief Initialize the given scripts by loading all the required 
  data files.
  \param ctx the supplied context.
  \param ... variable length list of imli_script_t values terminated by SCRIPT_UNSUPPORTED.
  \return IMLI_STATUS_OK on success or the corresponding status code on error.
*/

EXPORT_C imli_status_t imli_init_scripts (imli_context_t ctx, ...)
{
    va_list arg_list;
    va_start(arg_list, ctx);
    imli_script_t script;
    for (;;)
    {
	script = va_arg(arg_list, imli_script_t);
	if (script == SCRIPT_UNSUPPORTED) 
	    break;

        if (script == SCRIPT_ASCII)
            continue;
        
        ctx->scripts[script] = (script_info_t)malloc(sizeof(struct script_info));
	ctx->scripts[script]->script_code = script;
	ctx->scripts[script]->glyphs = NULL;
	ctx->scripts[script]->data   = NULL;
	ctx->scripts[script]->im     = NULL;
	load_tab_file(ctx, script);
    }    
    va_end(arg_list);
    return (imli_status_t)0;
}

/*!
  \brief Get the glyphs corresponding to the syllable.
  \param ctx the supplied context
  \param syl the syllable for which the glyphs are to be generated
  \param script the script code in which the glyphs should be generated
  \param outbuf a buffer to receive the output glyphs
  \param capacity specifies the number of bytes the receiver can hold. 
  \param [out] length returns the number of bytes of the glyph string.
  \return IMLI_STATUS_OK on success, IMLI_STATUS_GLYPH_NOT_FOUND in case
  of errors. 
*/

EXPORT_C imli_status_t 
syl_to_glyphs(imli_context_t ctx, 
	      syl_t syl, imli_script_t script,
	      byte_t *outbuf, size_t capacity, size_t *length)
{
    script_info_t sinfo;

    if (!outbuf || syl == SYL_INVALID)
        return IMLI_STATUS_OK;
    
    if (IS_SYL_ASCII(syl) || script == SCRIPT_ASCII)
    {
		*outbuf = syl & 0xFF;
		*length = 1;
		return IMLI_STATUS_OK;
    }

    sinfo = ctx->scripts[script];
    if (!sinfo) return IMLI_STATUS_GLYPH_NOT_FOUND;

    const byte_t *data = sinfo->glyphs[syl];
    if (data)
    {
		byte_t len = *data++;
		byte_t i;
	
		if (len > capacity) return IMLI_STATUS_BUFFER_NO_ROOM;
	
		for(i= 0; i < len; i++)
			outbuf[i] = data[i];
		*length = len;
		return IMLI_STATUS_OK;
    }
    else
    {
		outbuf[0] = '?';
		*length = 1;
    }
    return IMLI_STATUS_GLYPH_NOT_FOUND;
}

/*!
  \brief Get the width of the syllable in terms of number of glyphs
  that it will be displayed as.
  \param ctx the supplied context
  \param syl the syllable for which the length is to be returned
  \param script the script code for the syllable
  \param [out] length returns the number of bytes of the glyph string.
  \return IMLI_STATUS_OK on success, IMLI_STATUS_GLYPH_NOT_FOUND in
  case of errors.
*/

EXPORT_C imli_status_t 
syl_width(imli_context_t ctx, 
	  syl_t syl, imli_script_t script,
	  byte_t *length)
{
    script_info_t sinfo = NULL;
    imli_status_t status = IMLI_STATUS_OK;

    if (IS_SYL_ASCII(syl) || script == SCRIPT_ASCII)
    {
	*length = 1;
	return status;
    }
    if (!(ctx->scripts[script]))
        imli_init_scripts(ctx, script, SCRIPT_UNSUPPORTED);
    sinfo = ctx->scripts[script];
    if (sinfo)
    {
	const byte_t *data = sinfo->glyphs[syl];
	if (data)
	{
	    *length = *data;
	}
	else
	{
	    *length = 1; /* '?' */
	    status = IMLI_STATUS_GLYPH_NOT_FOUND;
	}
    }
    else 
    {
        *length = 1; /* '?' */
	status = IMLI_STATUS_GLYPH_NOT_FOUND;
    }

    return status;
}

/*!
  \brief Get the glyphs corresponding to the syllable along with its attributes.
  \param ctx the supplied context
  \param syl the syllable for which the glyphs are to be generated
  \param script the script code in which the glyphs should be generated
  \param outbuf a buffer to receive the output glyphs
  \param capacity specifies the number of bytes the receiver can hold. 
  \param attrbuf a buffer where the logical attribute for each glyph will be stored.
  \param [out] length returns the number of bytes of the glyph string.
  \return IMLI_STATUS_OK on success, IMLI_STATUS_GLYPH_NOT_FOUND in case
  of errors. 
*/

imli_status_t 
EXPORT_C syl_to_glyphs_with_attr(imli_context_t ctx, 
			syl_t syl, imli_script_t script,
			byte_t *outbuf, byte_t *attrbuf,
			size_t capacity, 			
			size_t *length)
{
    script_info_t sinfo = ctx->scripts[script];
    if (!sinfo) return IMLI_STATUS_GLYPH_NOT_FOUND;

    if (IS_SYL_ASCII(syl) || script == SCRIPT_ASCII)
    {
	*outbuf  = syl & 0xFF;
	*attrbuf = ATTR_SYL_STOP;
	if (*outbuf == ' ' || *outbuf == '\t') 
	    *attrbuf |= ATTR_SYL_WHITESPACE;
	*length  = 1;
	return IMLI_STATUS_OK;
    }

    const byte_t *data = sinfo->glyphs[syl];
    if (data)
    {
	byte_t len = *data++;
	byte_t i;

	if (len > capacity) return IMLI_STATUS_BUFFER_NO_ROOM;

	for(i= 0; i < len; i++)
	{
	    outbuf[i] = data[i];
	    attrbuf[i] = 0;
	}	
	*length = len;
	attrbuf[0] = ATTR_SYL_STOP;	
	return IMLI_STATUS_OK;
    }
    else
    {
	*outbuf = '?';
	*attrbuf = ATTR_SYL_STOP;
	*length = 1;
    }
    return IMLI_STATUS_GLYPH_NOT_FOUND;
}

EXPORT_C imli_status_t
syls_to_glyphs(imli_context_t ctx, 
	       const syl_t *text, size_t len,
	       imli_script_t script,
	       byte_t *outbuf, size_t capacity, size_t *length)
{
    unsigned int i;
    size_t n;
    imli_status_t status;
    for(i = 0; i < len; i++)
    {
	status = syl_to_glyphs(ctx, text[i], script, outbuf, capacity, &n);
	outbuf += n;
	capacity -= n;
	*length += n;		
    }
    return IMLI_STATUS_OK;
}

EXPORT_C imli_status_t
syls_to_glyphs_with_attr(imli_context_t ctx, 
			 const syl_t *text, size_t len,
			 imli_script_t script,
			 byte_t *outbuf, 
			 byte_t *attrbuf,
			 size_t capacity,
			 size_t *length)
{
    unsigned int i;
    size_t n;
    imli_status_t status;
    for(i = 0; i < len; i++)
    {
	status = syl_to_glyphs_with_attr(ctx, text[i], script, outbuf, attrbuf, capacity, &n);
	//FIXME: what to do with length?
	if (status == IMLI_STATUS_OK)
	{
	    outbuf += n;
	    capacity -= n;
	}
	else if (status == IMLI_STATUS_GLYPH_NOT_FOUND)
	{
	    *outbuf = '?';
	    outbuf++;
	    capacity--;
	}
	else return status;
    }
    return IMLI_STATUS_OK;
}


EXPORT_C imli_status_t 
imli_script_itemize (imli_context_t ctx,
		     const syl_t *text, size_t len,
		     struct script_item_run **runs,
		     size_t *nruns)
{
    syl_t syl = text[0];
    int run_idx;
    unsigned int i;
    int run_array_len = 16;
    script_item_run_t *run_array = 0;

    if (!IS_SYL_SWITCH(syl)) return IMLI_STATUS_BAD_INPUT;
    run_array = (script_item_run_t*)malloc(sizeof(struct script_item_run)* run_array_len);
    if (!run_array) return IMLI_STATUS_NO_MEMORY;
    run_idx = 0;
    run_array[run_idx].script_code = (imli_script_t)(syl & 0x00FF);
    run_array[run_idx].offset      = 0;
        
    for(i = 1; i < len; i++)
    {
	syl = text[i];
	if (syl == SYL_TERMINATOR) 
	    continue;
	if (IS_SYL_SWITCH(syl))
	{
	    if((syl & 0xFF) == run_array[run_idx].script_code)
		continue;
	    // else finish the run
	    run_array[run_idx].length = i - run_array[run_idx].offset;
	    run_idx++;
	    // expand the array if required
	    if (run_idx >= run_array_len)
	    {
		run_array_len *= 2;
		run_array = 
		    (script_item_run_t*)realloc(run_array, 
						sizeof(struct script_item_run)* run_array_len);
	    }
	    // start a new run
	    run_array[run_idx].script_code = (imli_script_t)(syl & 0xFF);
	    run_array[run_idx].offset      = i;	    
	}
    }
    if (run_array_len > (run_idx + 1))
	run_array = (script_item_run_t*)realloc(run_array,
						sizeof(struct script_item_run)* (run_idx+1));
    *runs = run_array;
    *nruns = run_idx + 1;
    return IMLI_STATUS_OK;
}

EXPORT_C void dump_tab (imli_context_t ctx, imli_script_t script)
{
    byte_t c, cj, v;
    char buf[32];
    size_t len = 0;
    for(c = 0; c < MAX_CONSONANTS; c++)
    {
        for(cj = 0; cj < MAX_CONJUNCTS; cj++)
        {
            for(v = 0; v < MAX_VOWELS; v++)
            {
		script_info_t sinfo = ctx->scripts[script];
		if (sinfo)
		{
		    byte_t *data = sinfo->glyphs[SYLLABLE(c, cj, v)];
		    if (data)
		    {
			len = *data++;
			byte_t i;
			for(i = 0; i < len; i++)
			    buf[i] = data[i];
			buf[len] = '\0';
			fprintf(stdout, "[%d,%d,%d] = %s (%d bytes)\n", c, cj, v, buf, len);
		    }
		}
                else
                {
		    fprintf(stderr, "No SINFO!\n");
                }
            }
        }
    }
}
