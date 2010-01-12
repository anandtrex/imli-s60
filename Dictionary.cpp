/*************************************************************
 * Dictionary.cpp
 *
 * This file is part of Acharya Text Editor
 *
 * Licensed under the GNU General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#include <cassert>
#include <iostream>
#include "Dictionary.h"

struct Dictionary::Node
{
    Node() 
	: syl(SYL_INVALID), 
	  lokid(0), 
	  eqkid(0), 
	  hikid(0), 
	  flag(0),
	  freq(0)
	{}
    ~Node()
	{
	    delete lokid;
	    delete eqkid;
	    delete hikid;
	}
    
    syl_t syl;
    Node* lokid;
    Node* eqkid;
    Node* hikid;
    int flag:2;
    int freq:30;
};

Dictionary::Dictionary()
    : root_(0), count_(0)
{
}

Dictionary::~Dictionary()
{
    Clear();
    //TODO
}

Dictionary::Dictionary(const Dictionary& other)
{
    //FIXME: implement deep copy
    root_ = other.root_;
    count_ = other.count_;
}

void Dictionary::operator =(const Dictionary& rhs)
{
    //FIXME: implement deep copy
    root_ = rhs.root_;
    count_ = rhs.count_;
}

void Dictionary::Clear()
{
    delete root_;
    root_ = 0;
    count_ = 0;
}

/*!
  \brief Insert the string of syllables given by \a str into the dictionary.
  \note str needs to be terminated by a SYL_INVALID syllable.
 */
void Dictionary::Insert(const syl_t* str, size_t n)
{
    if (n)
    {
	root_ = InsertRec(root_, str, n);
	assert(root_);
    }
}

Dictionary::Node* Dictionary::InsertRec(Dictionary::Node* node, const syl_t* str, size_t n)
{
    if (!n)
	return node;
    
    if (!node)
    {
	node = new Node();
	node->syl = *str;
    }
    
    if (node->syl != SYL_INVALID && *str < node->syl)
    {
	node->lokid = InsertRec(node->lokid, str, n);
    }
    else if (*str == node->syl && node->syl != SYL_INVALID)
    {
	if (--n)
	{
	    node->eqkid = InsertRec(node->eqkid, ++str, n);
	}
	else
	{
	    node->flag = 1;
	    node->freq++;
	}
    }
    else
    {
	node->hikid = InsertRec(node->hikid, str, n);
    }
    
    return node;
}

/*!
  \brief Find all strings starting with \a prefix. The results are stashed in \a list in sorted order.
  \returns the number of strings starting with \a prefix
 */
int Dictionary::MatchPrefix(const syl_t* prefix, size_t len, std::vector<SylString>& list)
{
    Node* node = GetNode(prefix, len);
    if (!node)
	return 0;
    list.clear();
    SylString buf;
    Collect(node->eqkid, buf, list);
    return list.size();
}

Dictionary::Node* Dictionary::GetNode(const syl_t* str, size_t len)
{
    Node* node = root_;
    while (node)
    {
	if (*str < node->syl && node->syl != SYL_INVALID)
	    node = node->lokid;
	else if (*str > node->syl && node->syl != SYL_INVALID)
	    node = node->hikid;
	else 
	{
	    str++;
	    len--;
	    if (len == 0)
		break;
	    node = node->eqkid;
	}   
    }
    return node;
}

void Dictionary::Collect(Dictionary::Node* node, SylString& buf, std::vector<SylString>& list)
{
    if (!node)
	return;
    
    Collect(node->lokid, buf,list);
    buf += node->syl;
    
    if (node->flag)
    {
	SylString tmp = buf;
	list.push_back(tmp);
    }
    
    Collect(node->eqkid, buf,list);
    buf.erase(buf.length() - 1, 1);
    Collect(node->hikid, buf,list);
}
