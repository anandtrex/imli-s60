/*************************************************************
 * llf.c
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
#include "libimli.h"
#include "llf.h"

/*!
  \file llf.h
*/

/*!
  \brief Parse and read a chunk of syllables from the given LLF file
  \param fp File pointer for the LLF file
  \param buf Receives the syllables read from the file
  \param len No. of syllables that \a buf can hold
  \returns The number of syllables actually copied or -1 in the case of error
 */
int imli_llf_read_block(FILE *fp, syl_t *buf, size_t len)
{
    // parse the header when called for the first time
    byte_t header[32];
    int n = 0;
   
    if(ftell(fp) == 0)
    {
	if(fread(&header, 1, sizeof(header), fp) != sizeof(header))
	{
	    fprintf(stderr, "Oopps...\n");	   
	    return -1;
	}
	
	if(header[0] != 'L' || header[1] != 'L' || header[2] != 'L')
	    return -1;
	// copy the initial script code syl to the buffer before copying 
	// the data
	*buf++ = SWITCH_CODE | header[5];
	len--;
	n = 1;
    }
    return n + fread(buf, sizeof(syl_t), len, fp);
}

/*!
  \brief Parse and read a "line" of syllables from the given LLF file where
  the SYL_NEWLINE terminates the line. It is added to the buffer.
  \param fp File pointer for the LLF file
  \param buf Receives the syllables read from the file
  \param len No. of syllables that \a buf can hold
  \returns The number of syllables actually copied or -1 in the case of error
 */

EXPORT_C int imli_llf_read_line(FILE *fp, syl_t *buf, size_t len)
{
    // parse the header when called for the first time
    byte_t header[32];
    size_t n = 0;
    if(ftell(fp) == 0)
    {
	if(fread(&header, 1, sizeof(header), fp) != sizeof(header))
	    return -1;
	if(header[0] != 'L' || header[1] != 'L' || header[2] != 'L')
	    return -1;
	// copy the initial language syl to the buffer before copying 
	// the data
	*buf++ = SWITCH_CODE | header[5];
	n++;
	len--;
    }
    syl_t syl;
    while (++n < len)
    {
	if(fread(&syl, sizeof(syl_t), 1, fp) != 1)
	{
	    n--;
	    break;
	}
	*buf++ = syl;
	if(syl == SYL_NEWLINE)
	    break;
    }
    return n;
}
