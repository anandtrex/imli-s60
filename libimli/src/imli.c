/*************************************************************
 * imli.c
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

#include <stdlib.h>
#include <string.h>
#include "libimli.h"
#include "imli_private.h"


/*! 
  \file imli.h
  \typedef syl_t
  \brief A syllable

  Represents a syllable - the fundamental object of the IMLI library.

*/

/*!
  \typedef imli_context_t
  \brief Opaque handle to the context structure
*/


/*!
  \typedef \enum imli_status imli_status_t
  \brief Status code

  imli_status_t is used to indicate the status that can occur when
  functions of the IMLI library are called. They are mostly used as
  return values.

  \li \c IMLI_STATUS_OK             - Indicates success
  \li \c IMLI_STATUS_NO_MEMORY      - Indicates an out of memory condition
  \li \c IMLI_STATUS_DATA_NOT_FOUND -  Required data not available
*/

/*!
  \typedef \enum imli_script imli_script_t
  \brief Scripts supported by the library
*/

/*!
  \typedef \enum script_item_run script_item_run_t
  \brief A run of text in the same script. Currently unused.
*/

/*!
  \param [out] ctx receives the newly created context
  \param data_dir specified the data directory
  \return 0 upon success; -1 on failure
*/

EXPORT_C imli_status_t imli_context_new  (imli_context_t* ctx, const char *data_dir)
{
    imli_context_t new_ctx = (imli_context_t) malloc(sizeof(struct imli_context));
    if(!new_ctx)
    {
        *ctx = 0;
        return IMLI_STATUS_NO_MEMORY;
    }
    
    if (!data_dir)
        new_ctx->base_dir = get_home_dir();
    else
        new_ctx->base_dir = strdup(data_dir);

    if(!new_ctx->base_dir)
    {     
        free(new_ctx);
        *ctx = 0;
        return IMLI_STATUS_DATA_NOT_FOUND;
    }
    
    memset(new_ctx->conjunct_table, 0, sizeof(new_ctx->conjunct_table));
    if(init_conjunct_tables(new_ctx) != 0)
    {
        imli_context_free(new_ctx);
        *ctx = 0;
        return IMLI_STATUS_DATA_NOT_FOUND;
    }
    unsigned int i;
    for(i = 0; i < (sizeof(new_ctx->scripts)/sizeof(new_ctx->scripts[0])); i++)
        new_ctx->scripts[i] = NULL;

    new_ctx->active_script = SCRIPT_UNSUPPORTED;
    new_ctx->iscii = 0;
    new_ctx->valid = 1;
    
    *ctx = new_ctx;
    return IMLI_STATUS_OK;
}

/*!
  \param ctx receives the newly created context
  \return 0 upon success; -1 on failure
*/

EXPORT_C imli_status_t imli_context_free (imli_context_t ctx)
{
	return IMLI_STATUS_OK; //TODO temporary hack
	
    int i = 0;
    if (!imli_is_context_valid(ctx)) return IMLI_STATUS_OK;

    if(ctx->base_dir)
    {
        free(ctx->base_dir);
    }

    for(i = 0; i < (int)SCRIPT_MAX; i++)
    {
        script_info_t sinfo = ctx->scripts[i];
        if (sinfo)
        {
            if (sinfo->data) //TODO problem here. Look into it.
                free(sinfo->data);
            if (sinfo->glyphs)
                free(sinfo->glyphs);
            free(sinfo);
        }
    }
    if (ctx->iscii) free(ctx->iscii);
    free(ctx);
    return IMLI_STATUS_OK;
}

/*!
  \brief Check if \a ctx is a valid IMLI context
  \returns 1 if true and 0 if false
*/
EXPORT_C int imli_is_context_valid (imli_context_t ctx)
{
    return (ctx && ctx->valid);
}

/*!
  \brief Returns the full path of the IMLI data directory.
  \param ctx the supplied context
  \return full path of the data dir or NULL
*/

EXPORT_C char *imli_home_dir (imli_context_t ctx)
{
    return strdup(ctx->base_dir);
}

/*!
  \typedef \enum imli_script imli_script_t
  \brief Scripts supported by the library.
*/

static const char* const scripts_[] = { //the extra const to prevent initialised data problem in symbian dll
    "sanskrit",
    "hindi",
    "tamil",
    "telugu",
    "kannada",
    "malayalam",
    "oriya",
    "bangla",
    "gujarati",
    "punjabi",
    "asamiya",
    "filler1",
    "marathi",
    "braille",
    "romantrans"
};
/*!
  \brief Get the script name from the script code

  The returned value is allocated statically and should not be freed.

  \param script the script code
  \return string containing the equivalent script name
*/

EXPORT_C const char *imli_script_name (imli_script_t script)
{
    //const char *name = NULL;
    return scripts_[script];
}

/*!
  \brief Get the script code from script name

  Does a case insensitive match of \a name against the list of 
  supported script names. SCRIPT_UNSUPPORTED is returned if the
  match fails.

  \param name the name of the script
  \return one of the imli_script_t
*/

EXPORT_C imli_script_t imli_script_code (const char *name)
{
    int i = (int) SCRIPT_SANSKRIT;
    for ( ; i < (int)SCRIPT_MAX; i++)
    {
        if (strcasecmp(name, scripts_[i]) == 0)
            return (imli_script_t)i;
    }
    return SCRIPT_UNSUPPORTED;
}

/*!
  \brief Make \a script the active script
*/

EXPORT_C imli_status_t imli_select_script(imli_context_t ctx, imli_script_t script)
{    
    script_info_t sinfo = NULL;
    imli_status_t status = IMLI_STATUS_OK;
    ctx->active_script = script;
    if (script != SCRIPT_ASCII)
    {
        sinfo = ctx->scripts[script];
        if (!sinfo)
            status = imli_init_scripts(ctx, script, SCRIPT_UNSUPPORTED);
    }
    return status;
}

/*!
  \brief Get the currently selected script
*/

EXPORT_C imli_script_t imli_get_active_script(imli_context_t ctx)
{
    return ctx->active_script;
}
