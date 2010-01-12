/*************************************************************
 * PieceChain.cpp
 *
 * This file is part of Acharya Text Editor
 *
 * Licensed under the GNU General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#include "PieceChain.h"
#include <stdio.h>
#include <QDebug>

pos_t PieceChain::Iterator::GetDataIndex() const
{
    return piece_->GetDataIndex() + pieceOffset_;
}

bool PieceChain::Iterator::IsValid() const
{
    return (piece_ != chain_.sentinel_ && piece_ && piece_->length != 0);
}

bool PieceChain::Iterator::operator != (const Iterator& other) const
{
    return (piece_ != other.piece_ ||
            pieceOffset_ != other.pieceOffset_);
}

bool PieceChain::Iterator::operator == (const Iterator& other) const
{
    return !(*this != other);
}

PieceChain::Iterator& PieceChain::Iterator::operator =(const Iterator& other)
{
    piece_ = other.piece_;
    pieceOffset_ = other.pieceOffset_;
    position_ = other.position_;
    return *this;
}

PieceChain::Iterator& PieceChain::Iterator::operator++ ()
{
    position_ += piece_->GetLength();
    pieceOffset_ = 0;
    piece_ = piece_->next;
    return *this;
}

PieceChain::Iterator PieceChain::Iterator::operator++(int)
{
    Iterator tmp = *this;
    ++tmp;
    return tmp;
}

PieceChain::Iterator& PieceChain::Iterator::operator-- ()
{
    piece_ = piece_->prev;
    position_ -= piece_->GetLength();
    pieceOffset_ = 0;
    return *this;
}

PieceChain::Iterator  PieceChain::Iterator::operator--(int)
{
    Iterator tmp = *this;
    --tmp;
    return tmp;
}

void PieceChain::Iterator::IncOffset(pos_t count)
{
    position_ += count;
    while (count)
    {
        if (pieceOffset_ + count < piece_->length)
        {
            pieceOffset_ += count;
            break;
        }
        else
        {
            count -= piece_->length - pieceOffset_;
            pieceOffset_ = 0;
            piece_ = piece_->next;
        }
    }
}

void PieceChain::Iterator::DecOffset(pos_t count)
{
    position_ -= count;
    while (count)
    {
        if (pieceOffset_ >= count)
        {
            pieceOffset_ -= count;
            break;
        }
        else
        {
            count -= pieceOffset_ + 1;
            piece_ = piece_->prev;
            pieceOffset_ = piece_->length - 1;
        }
    }
}

/*!
  \brief Constructs an empty piece chain.
*/

PieceChain::PieceChain()
    : length_(0), cachedPos_(SENTINEL_POS), cachedPiece_(0)
{
    sentinel_ = new Piece(SCRIPT_UNSUPPORTED,
                          0,
                          0);
    sentinel_->next = sentinel_;
    sentinel_->prev = sentinel_;
}

PieceChain::~PieceChain()
{
    Clear();
    delete sentinel_;
}

void PieceChain::Clear()
{
    Piece* p = sentinel_->next;
    while (p && p != sentinel_)
    {
        Piece* tmp = p;
        p = p->next;
        delete tmp;
    }
    sentinel_->next = sentinel_;
    sentinel_->prev = sentinel_;
    length_ = 0;
}

/*!
  \brief Logical beginning of the sequence
*/

PieceChain::Iterator PieceChain::Begin() const
{
    return Iterator(*this, 0, sentinel_->next, 0);
}

/*!
  \brief Logical end of the sequence
*/

PieceChain::Iterator PieceChain::End() const
{
    return Iterator(*this, length_, sentinel_, 0);
}

/*!
  \brief Invalidate the cache
*/
void PieceChain::InvalidateCache()
{
    cachedPos_ = 0xFFFFFFFF;
    cachedPiece_ = sentinel_;
}


/*!
  \brief Return the iterator representing the piece containing
  the position \a pos .
*/

PieceChain::Iterator PieceChain::Seek(pos_t pos) const
{
    //fprintf(stderr, "Seek(): pos = %u, cachedPos = %u\n\n", pos, cachedPos_);
    if (!length_ || pos >= length_) return this->End();

    //if (pos < cachedPos_)
    {
        // invalidate cache
        cachedPos_ = 0;
        cachedPiece_ = sentinel_->next;
    }

    Piece* p = cachedPiece_;
    pos_t start = cachedPos_;

    if (p == sentinel_) return this->End();

    int n = 0;
    while (pos >= start + p->length)
    {
        start += p->length;
        p = p->next;
        ++n;
    }

    // update cache if necessary
    if (n > 50)
    {
        cachedPiece_ = p;
        cachedPos_   = start;
    }

    return Iterator(*this, pos, p, pos - start);
}

/*!
  \brief Insert \a pce after the piece pointed to by \a it .
*/
void PieceChain::InsertAfter(PieceChain::Iterator it, Piece* pce)
{
    if (it.GetPosition() <= cachedPos_) InvalidateCache();

    Piece* p = it.piece_;
    pce->next = p->next;
    pce->next->prev = pce;
    p->next = pce;
    pce->prev = p;
}

/*!
  \brief Insert \a p before the piece pointed to by \a it .
*/
void PieceChain::InsertBefore(PieceChain::Iterator it, Piece* pce)
{
    if (it.GetPosition() <= cachedPos_) InvalidateCache();

    Piece* p = it.piece_;
    pce->prev = p->prev;
    pce->prev->next = pce;
    p->prev = pce;
    pce->next = p;
}

/*!  
  \brief Unlink the piece pointed to by \a it from the chain.
  After this, \a it is invalid and if used can corrupt the 
  piece chain.
*/
void PieceChain::Erase(PieceChain::Iterator it)
{
    Piece* prev = it.piece_->prev;
    Piece* next = it.piece_->next;
    prev->next = next;
    next->prev = prev;
}

/*!
  \brief Remove pieces from \a start till \a end from the chain.
*/
void PieceChain::Erase(PieceChain::Iterator start,
                       PieceChain::Iterator end)
{
    Piece* left = start.piece_->prev;
    left->next = end.piece_;
    left->next->prev = left;
}

/*!
  \brief split the piece pointed to by \a it and return the "right" side of the split
*/
PieceChain::Iterator PieceChain::Split(PieceChain::Iterator it,
                                       pos_t offset)
{
    if (offset == 0) return it;
    Piece* old = it.piece_;
    Piece* p = new Piece(old->script,
                         old->offset + offset,
                         old->length - offset);
    old->length = offset;

    InsertAfter(it, p);
    return PieceChain::Iterator(*this, it.GetPosition(), p);
}

/*!  \brief split the piece pointed to by \a it and return the "right"
 *  side of the split. Also adjust the glyph counts with reference to \a widths 
 */
PieceChain::Iterator PieceChain::Split(PieceChain::Iterator it,
                                       const std::deque<byte_t>& widths)
{
    pos_t offset = it.GetOffset();
    if (offset == 0) return it;

    Piece* old = it.piece_;
    Piece* p = new Piece(old->script,
                         old->offset + offset,
                         old->length - offset);
    old->length = offset;
    InsertAfter(it, p);
    /* update glyph counts by summing the smaller piece and
     * subtracting from the old glyph count.
     */
    size_t nglyphs = 0;
    if (p->length <= old->length)
    {
        for (pos_t pos = p->offset; pos < p->offset + p->length; pos++)
            nglyphs += widths[pos];
        old->glyphCount -= nglyphs;
        p->glyphCount = nglyphs;
    }
    else
    {
        for (pos_t pos = old->offset; pos < old->offset + old->length; pos++)
            nglyphs += widths[pos];
        p->glyphCount = old->glyphCount - nglyphs;
        old->glyphCount = nglyphs;
    }
    return PieceChain::Iterator(*this, it.GetPosition(), p);
}

void PieceChain::Restore(Piece* from, Piece* to = 0)
{
    if (sentinel_->next == sentinel_)
    {
        //qDebug() << "Restore: Empty chain...";
        // empty chain
	sentinel_->next = from;
	sentinel_->prev = to ? to : from;
    }
    else
    {
        //qDebug() << "Restore: non empty chain...";
        from->prev->next = from;
        if (to && to != from)
            to->next->prev = to;
        else
            from->next->prev = from;
    }
}
