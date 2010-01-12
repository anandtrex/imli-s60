/*************************************************************
 * PieceChain.h
 *
 * This file is part of Acharya Text Editor
 *
 * Licensed under the GNU General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#ifndef __PIECE_CHAIN__H__
#define __PIECE_CHAIN__H__

#include <libimli.h>
#include <stdio.h>
#include <deque>

typedef unsigned int pos_t;

#define SENTINEL_POS 0xFFFFFFFF

class TextStorage;
class Piece;

class PieceChain
{
public:
    PieceChain();
    ~PieceChain();
    void Clear();

    class Iterator
    {
    public:
        friend class PieceChain;
        Piece* GetValue() const
            {
                return piece_;
            }
        pos_t  GetOffset() const
            {
                return pieceOffset_;
            }
        void UpdateOffset(pos_t off) 
            {
                pieceOffset_ = off;
            }
        pos_t  GetPosition() const
            {
                return position_;
            }
        pos_t  GetDataIndex() const;
        bool IsValid() const;
        bool operator != (const Iterator& other) const;        
        bool operator == (const Iterator& other) const;
        Iterator& operator =(const Iterator& other);
        Iterator& operator++ ();
        Iterator  operator++(int);
        Iterator& operator-- ();
        Iterator  operator--(int);
        void IncOffset(pos_t count = 1);
        void DecOffset(pos_t count = 1);
        void UpdatePosition(pos_t pos)
            {
		position_ = pos;
            };
    private:
        Iterator(const PieceChain& pc, pos_t position, Piece* piece, pos_t offset = 0)
            : chain_(pc), position_(position), pieceOffset_(offset), piece_(piece)
            {}
        Iterator(const PieceChain& pc)
            : chain_(pc)
            {}
        const PieceChain& chain_;
        pos_t  position_;
        pos_t  pieceOffset_;
        Piece* piece_;
    };

    Iterator Begin() const;
    Iterator End() const;
    Iterator Seek(pos_t position) const;
    void InsertAfter(Iterator it, Piece* piece);
    void InsertBefore(Iterator it, Piece* piece);
    void Prepend(Piece* p);
    void Append(Piece* p);
    void Erase(Iterator it);
    void Erase(PieceChain::Iterator start,
	       PieceChain::Iterator end);
    Iterator Split(Iterator it, pos_t offset);
    Iterator Split(Iterator it, const std::deque<byte_t>& widths);
    void Restore(Piece* from, Piece* to);
        
    size_t GetLength() const
        {
            return length_;
        }
    void SetLength(size_t length)
        {
            length_ = length;
        }
    size_t GetGlyphCount()
        {
            return nglyphs_;
        }
    void SetGlyphCount (size_t count)
        {
            nglyphs_ = count;
        }
    void DumpChain();
    void InvalidateCache();
private:
    friend class Iterator;
    // prevent copying
    PieceChain(const PieceChain&)
        {}
    void operator =(const PieceChain&)
        {}

    // total no. of syllables in the chain
    size_t length_;
    // total no. of glyphs in the chain
    size_t nglyphs_;

    // recently accessed position in the chain
    mutable pos_t  cachedPos_;
    mutable Piece* cachedPiece_;

    // serves as both the head and tail
    Piece* sentinel_;
};

class Piece
{
public:
    imli_script_t GetScript() const
        {
            return script;
        }
    pos_t GetDataIndex() const
        {
            return offset;
        }
    size_t GetLength() const
        {
            return length;
        }
    size_t GetGlyphCount() const
        {
            return glyphCount;
        }
    bool operator != (const Piece& other)
        {
            return (script != other.script ||
                    offset != other.offset ||
                    length != other.length);
        }
    bool operator ==(const Piece& other)
        {
            return !(*this != other);
        }
private:
    Piece(imli_script_t s, pos_t off, size_t len)
        : script(s), offset(off), length(len), glyphCount(0),
          next(0), prev(0)
        {}
    void SetGlyphCount(size_t count)
        {
            glyphCount = count;
        }
    imli_script_t script;
    pos_t  offset;
    size_t length;
    size_t glyphCount;
    Piece* next;
    Piece* prev;
    friend class PieceChain;
    friend class PieceChain::Iterator;
    friend class TextStorage;
};

#endif
