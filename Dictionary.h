/*************************************************************
 * Dictionary.h
 *
 * This file is part of Acharya Text Editor
 *
 * Licensed under the GNU General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include <vector>
#include <string>
#include <libimli.h>

typedef std::basic_string<syl_t> SylString;

class Dictionary
{
public:
    Dictionary();
    ~Dictionary();
    Dictionary(const Dictionary&);
    void operator = (const Dictionary&);
    void Clear();
    
    void Insert(const syl_t*, size_t);
    int MatchPrefix(const syl_t*, size_t, std::vector<SylString>&);
private:
    struct Node;
    Node* InsertRec(Node*, const syl_t*, size_t);
    Node* GetNode(const syl_t*, size_t);
    void Collect(Node*, SylString&, std::vector<SylString>&);

    Node* root_;
    int count_;
};

#endif
