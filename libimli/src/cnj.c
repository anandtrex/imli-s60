/*************************************************************
 * cnj.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cnj_scan.c"

#define LINE_SIZE 4096

int init_conjunct_tables (imli_context_t ctx)
{
    char fullpath[128];
    char line_buf[LINE_SIZE];
    
    sprintf(fullpath, "%s\\%s\\%s", //needs to be backslash here TODO
	    imli_home_dir(ctx), 
	    "langfiles",
	    "generic.cnj");
    
    FILE* fp = fopen(fullpath, "rb"); //problem here
    if (!fp)
    {
		fprintf( stderr, 
			 "init_conjunct_tables(): failed to open file %s\n", 
			 fullpath);
		return -1;
    }
	
    while (NULL != fgets(line_buf, LINE_SIZE, fp))
    {
        scan(line_buf, strlen(line_buf), ctx);
    }

    fclose(fp);

    return 0;
}

/*!
  \file imli.h
*/

/*!
  \brief get the number of valid consonants
*/

EXPORT_C int imli_get_consonant_count(imli_context_t ctx)
{
    return N_CONSONANTS;
}

/*!
  \brief get the number of combinations of \a c
  \param ctx the IMLI context.
  \param c   the consonant for which the count is requested.
*/

EXPORT_C int imli_get_conjunct_count(imli_context_t ctx, byte_t c)
{
    return ctx->conjunct_table[c].count;
}

/*!
  \brief get the syllable code for the given consonants
  \param ctx the supplied context
  \param consonant the base consonant
  \param conjunct2 the second level conjunct
  \param conjunct3 the third level conjunct
  \param conjunct4 the fourth level conjunct
  \return the syllable equivalent of the above parameters.
*/

EXPORT_C syl_t imli_get_conjunct_code (imli_context_t ctx, byte_t consonant, byte_t conjunct2, byte_t conjunct3, byte_t conjunct4 )
{
    unsigned int value = (conjunct2) | (conjunct3 << 8) | (conjunct4 << 16);
    cnj_entry_t entry = ctx->conjunct_table[consonant];
    int i;
    for(i = 1; i <= entry.count; i++)
    {
        if (entry.conjuncts[i] == value)
            return SYLLABLE(consonant, i, 0);
    }
    return SYL_INVALID;
}

/*!
  \brief check if the given syllable \a l is "full". 
  
  Since a syllable can consist of upto 4 levels of conjuncts, this
  function checks whether a syllable is full. The constituent
  consonants are returned in \a c1, \a c2, \a c3 and \a c4.

  \returns 1 if the syllable is full and 0 otherwise.
*/

EXPORT_C int imli_is_syl_full ( imli_context_t ctx, syl_t l, byte_t *c1, byte_t *c2, byte_t *c3, byte_t *c4 )
{
    byte_t c = CONSONANT(l);
    byte_t cnj = CONJUNCT(l);
    *c1 = c;
    
    cnj_entry_t entry = ctx->conjunct_table[c];
    unsigned int value = entry.conjuncts[cnj];
    
    *c2 = value & 0xFF;
    *c3 = (value & 0x0000FF00) >> 8;
    *c4 = (value & 0x00FF0000) >> 16;
	
    return ( ( *c2 && *c3 && *c4) ? 1 : 0 );
}

const static char* const consonant_names[] = { //the extra const to prevent initialised data problem in symbian dll
    "nil","ka","kha","ga","gha","nga","cha","chha",
    "ja","jha","nja","ta","tta","dda","ddha","nna",
    "tha","ttha","da","dha","na","nex","nas","pa",
    "pha","ba","bha","ma","ya","yab","ra","rex",
    "rxl","rxxl","rra","la","lla","zha","va","ca",
    "sha","sa","ha","ddra","ddrha","ksha","jnya",
    "qa","xa","gga","za","fa","cca","vedic","nukhta"
};

EXPORT_C void dump_conjunct_table(imli_context_t ctx, FILE *fp)
{
    int i, j = 0;
    for (i = 1; i < N_CONSONANTS; i++)
    {
        cnj_entry_t entry = ctx->conjunct_table[i];
        fprintf(fp, "---------------------------------------\n");
        fprintf(fp, "consonant: %s  conjuncts: %d\n", 
                consonant_names[i], entry.count);
        fprintf(fp, "---------------------------------------\n");
        for(j = 1; j <= entry.count; j++)
        {
            unsigned int value = entry.conjuncts[j];
            char  cj2, cj3, cj4;//c,
            cj2 = (value & 0xFF);
            cj3 = (value & 0x0000FF00) >> 8;
            cj4 = (value & 0x00FF0000) >> 16;
            if (cj3)
            {
                if (cj4)
                    fprintf(fp, "%s + %s + %s\n", 
                            consonant_names[cj2], 
                            consonant_names[cj3],
                            consonant_names[cj4]);
                else
                    fprintf(fp, "%s + %s\n", 
                            consonant_names[cj2], 
                            consonant_names[cj3]);
            }
            else
            {
                fprintf(fp, "%s\n", consonant_names[cj2]);
            }
        }
    }
}
