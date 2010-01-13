/*************************************************************
 * llf.h
 *
 * This file is part of libimli
 *
 * Licensed under the GNU Library General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#ifndef __LLF_H__
#define __LLF_H__

EXPORT_C int imli_llf_read_chunk(FILE *fp, syl_t *buf, size_t len);

EXPORT_C int imli_llf_read_line(FILE *fp, syl_t *buf, size_t len);

#endif
