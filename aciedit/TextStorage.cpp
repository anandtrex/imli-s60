/*************************************************************
 * TextStorage.cpp
 *
 * This file is part of Acharya Text Editor
 *
 * Licensed under the GNU General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#include <assert.h>
#include "TextStorage.h"
#include "libimli.h"
#include <QtDebug>

template <typename T>
T min (const T& lhs, const T& rhs)
{
    return (lhs <= rhs ? lhs : rhs);
}

template <typename T>
void swap (T& lhs, T& rhs)
{
    T& tmp = lhs;
    lhs = rhs;
    rhs = tmp;
}


////////////////// TextStorage::Selection //////////////////

TextStorage::Selection::Selection(const TextStorage& text,
                                  pos_t start, pos_t end)
    : text_(text), start_(start), end_(end), iter_(text_.chain_.Seek(start))
{
    if (end_ < start_)
        swap(start, end);
    if (end_ > text_.Size())
        end_ = text_.Size();
    length_ = end_ - start_;
    script_ = iter_.GetValue()->GetScript();
}

TextStorage::Selection& TextStorage::Selection::operator =(const TextStorage::Selection& rhs)
{
    start_ = rhs.start_;
    end_   = rhs.end_;
    iter_ = rhs.iter_;
    return *this;
}

bool TextStorage::Selection::IsValid() const
{
    return (length_ &&
            length_ <= text_.Size() &&
            end_ != SENTINEL_POS &&
            script_ != SCRIPT_UNSUPPORTED);
}

void TextStorage::Selection::NextRun() const
{
    do
    {
	length_ -= min(iter_.GetValue()->GetLength() - iter_.GetOffset(), length_);
    }
    while (((++iter_).GetValue()->GetScript() == script_) && length_);
    
    script_ = iter_.GetValue()->GetScript();
}

SylString TextStorage::Selection::GetData() const
{
    PieceChain::Iterator it = iter_;
    size_t length = length_;
    SylString s;
    do
    {
        Piece* piece = it.GetValue();
	size_t len = min(piece->GetLength() - it.GetOffset(), length);
	s.append((text_.syllables_.begin() += it.GetDataIndex()),
                 (text_.syllables_.begin() += it.GetDataIndex() + len));

	length -= len;
    }
    while (((++it).GetValue()->GetScript() == script_) && length);

    return s;
}

////////////////// TextStorage //////////////////
TextStorage::TextStorage(imli_context_t context)
    : userData_(0),
      callback_(0),
      chain_(),
      point_(chain_.End()),
      prev_(point_),
      mark_(point_),
      notify_(true),
      generateGlyphs_(true)
{
    context_ = context;
}

TextStorage::~TextStorage()
{}

void TextStorage::SetNotifier(NotifyCB* cb, void* userData)
{
    callback_ = cb;
    userData_ = userData;
}

imli_context_t TextStorage::GetContext()
{
    return context_;
}

size_t TextStorage::Size() const
{
    return chain_.GetLength();
}

pos_t TextStorage::GetInsertPos() const
{
    return (Size() ? point_.GetPosition() : 0);
}

void TextStorage::Clear()
{
    chain_.Clear();
    point_ = chain_.Begin();
    prev_ = point_;    
    syllables_.clear();
    glyphs_.clear();
    sylWidths_.clear();
    undoRecord_.Clear();
}

TextStorage::Selection TextStorage::GetText(pos_t start, pos_t end)
{
    return TextStorage::Selection(*this, start, end);
}

bool TextStorage::HasSelection() const
{
    return (mark_.IsValid() && mark_ != point_);
}

TextStorage::Selection TextStorage::GetSelection() const
{
    return Selection(*this, mark_.GetPosition(), point_.GetPosition());
}

/*!
  \brief Insert syllables at insertion point
  \return the number of syllables inserted or -1 in the case of error.
*/
int TextStorage::Insert(imli_script_t script, const syl_t* data, size_t len)
{
    pos_t insert_pos = GetInsertPos();
   //  undoRecord_.Clear();
//     emit UndoCleared();
    
    if (this->AtEnd())
    {
        fprintf(stderr, "Calling Append(): script = %d, insert_pos = %d, length = %d\n\n", script, insert_pos, len);
        return Append(script, data, len);
    }
    else if (this->AtStart())
    {
        fprintf(stderr, "Calling Prepend(): insert_pos = %d\n\n", insert_pos);
        return Prepend(script, data, len);
    }
    else
    {
        fprintf(stderr, "Calling InsertBetween(): insert_pos = %d\n\n", insert_pos);
        return InsertBetween(script, data, len);
    }
}

/*!
  \brief Insert syllables at \a position.
  \return the number of syllables inserted or -1 in the case of error.
*/
int TextStorage::InsertBetween(imli_script_t script, const syl_t* data, size_t len)
{
    size_t nglyphs = 0;
    assert(data != NULL);
    if (prev_.GetPosition() != point_.GetPosition() - 1)
    {
        //qDebug() << "adjusting prev...";
        
        prev_ = point_;
        prev_.DecOffset();
    }
    
    QByteArray glyphs;
    
    if (IsGlyphGenerationEnabled())
    {
        glyphs.clear();
        glyphs.reserve(len*16);
        size_t width = 1;
        
        for (size_t i = 0; i < len; i++)
        {
            syllables_.push_back(data[i]);
            syl_to_glyphs(context_, data[i], script, (byte_t*)glyphs.data() + nglyphs, glyphs.capacity() - nglyphs, &width);
            sylWidths_.push_back(width);
            nglyphs += width;
        }

        glyphs.resize(nglyphs);
    }
    
    Piece* piece = prev_.GetValue();
    pos_t  pieceOffset = prev_.GetOffset();
    //fprintf(stderr, "\npiece_index = %d, piece_len = %d, off = %d, len = %d\n", piece->offset, piece->length, pieceOffset, len);
    pos_t size = syllables_.size();
    pos_t position = prev_.GetDataIndex();
    int retval = len;
    //qDebug() << "[ poffset = "  << pieceOffset << "size = " << size << "dataindex = " << position << "]";
    
    // extend the previous piece if possible
    if (script == piece->script &&
        pieceOffset == piece->length - 1 &&
        position == size - len - 1)
    {
        //printf("Expanding... len = %d\n", piece->length + len);
        piece->length += len;
        piece->SetGlyphCount(piece->GetGlyphCount() + nglyphs);
        point_.UpdatePosition(point_.GetPosition() + len);
        prev_ = point_;
        prev_.DecOffset(1);
    }
    else
    {
        //qDebug() << "New piece...";
        // new piece
        Piece* newPiece = new Piece(script, size - len, len);
        newPiece->SetGlyphCount(nglyphs);
        
        // split if point is not at the start of the piece
        if (point_.GetOffset() != 0)
            point_ = chain_.Split(point_, sylWidths_);
        
        chain_.InsertBefore(point_, newPiece);
        point_.UpdatePosition(point_.GetPosition() + len);
        prev_ = point_;
        prev_.DecOffset();
    }
    if (retval > 0)
    {
        chain_.SetLength(chain_.GetLength() + len);
        chain_.SetGlyphCount(chain_.GetGlyphCount() + nglyphs);
    
        if (IsNotificationEnabled())
            emit TextModified(TextStorage::Event(*this, Chunk(script, glyphs), Insertion, nglyphs, Forward));
        
        //emit TextModified(TextStorage::Event(*this, Selection(*this, point_.GetPosition() - len, point_.GetPosition() - 1), Insertion));
        
        // if (callback_)
//             callback_(TextStorage::Event(*this, SelecXtion(*this, point_.GetPosition() - len, point_.GetPosition() - 1), Insertion), userData_);
    }
    return retval;
}

/*!  \brief Insert the syllables at the beginning of the
  sequence. NOTE: this function will be called only if the point is at
  0 and the sequence is not empty.
 
  \returns The number of syllables inserted or -1 in the case of
  error.
*/

int TextStorage::Prepend(imli_script_t script, const syl_t* data, size_t len)
{
    size_t nglyphs = 0;
    QByteArray glyphs;
    assert(data != 0);

    if (IsGlyphGenerationEnabled())
    {
        glyphs.clear();
        glyphs.reserve(len*16);
        size_t width;
        
        for (size_t i = 0; i < len; i++)
        {
            syllables_.push_back(data[i]);
            width = 1;
            syl_to_glyphs(context_, data[i], script, (byte_t*)glyphs.data() + nglyphs, glyphs.capacity() - nglyphs, &width);
            sylWidths_.push_back(width);
            nglyphs += width;
        }
        
        glyphs.resize(nglyphs);
    }
    
    Piece* newPiece = new Piece(script, syllables_.size() - len, len);
    newPiece->SetGlyphCount(nglyphs);

    chain_.InsertAfter(chain_.End(), newPiece);
    //fprintf(stderr, "DEBUG: TextStorage::Prepend(...): new length = %d\n\n", chain_.GetLength() + len);
    chain_.SetLength(chain_.GetLength() + len);
    chain_.SetGlyphCount(chain_.GetGlyphCount() + nglyphs);
    point_ = chain_.Begin();
    point_.IncOffset(len);
    prev_ = point_;
    prev_.DecOffset();

    if (IsNotificationEnabled())
        emit TextModified(TextStorage::Event(*this, Chunk(script, glyphs), Insertion, nglyphs, Forward));

//     if (callback_)
//         callback_(TextStorage::Event(*this, Selection(*this, 0, len-1), Insertion, nglyphs), userData_);

    return len;
}

/*!
  \brief Insert the syllables at the end.
  \returns The number of syllables inserted or -1 in the case of error.
*/
int TextStorage::Append(imli_script_t script, const syl_t* data, size_t len)
{
    size_t txtLen = chain_.GetLength();
    size_t nglyphs = 0;
    
    assert(data != NULL);

    prev_ = point_;
    prev_.DecOffset();

    if (IsGlyphGenerationEnabled())
    {
        glyphs_.clear();
        glyphs_.reserve(len*16);
        size_t width = 1;

        
        for (size_t i = 0; i < len; i++)
        {    
            syllables_.push_back(data[i]);
            width = 1;
            syl_to_glyphs(context_, data[i], script, (byte_t*)glyphs_.data() + nglyphs, glyphs_.capacity() - nglyphs, &width);
            sylWidths_.push_back(width);
            nglyphs += width;
        }
        glyphs_.resize(nglyphs);
    }

    if (Size() <= 0)
    {
        //qDebug() << "Append(): new piece due to !Size()";
        
        // empty chain
        Piece* newPiece = new Piece(script, syllables_.size()-len, len);
        newPiece->SetGlyphCount(nglyphs);
        chain_.InsertBefore(point_, newPiece);
        point_ = chain_.End();
    }
    else
    {        
        Piece* piece = prev_.GetValue();

        //  extend the current piece if possible
        if (script == piece->script && (piece->offset + piece->length + len == syllables_.size()))
        {
            piece->length += len;
            piece->SetGlyphCount(piece->GetGlyphCount() + nglyphs);
        }
        else
        {
            //qDebug() << "Append(): new piece - not able to extend cur piece";
            Piece* newPiece = new Piece(script, syllables_.size()-len, len);
            newPiece->SetGlyphCount(nglyphs);
            chain_.InsertBefore(point_, newPiece);
        }
    }
    //fprintf(stderr, "DEBUG: TextStorage::Append(...): new length = %d\n\n", txtLen + len);
    chain_.SetLength(txtLen + len);
    chain_.SetGlyphCount(chain_.GetGlyphCount() + nglyphs);
    point_.UpdatePosition(chain_.GetLength());
    prev_.IncOffset(len);
    
    if (IsNotificationEnabled())
        emit TextModified(TextStorage::Event(*this, Chunk(script, glyphs_), Insertion, nglyphs, Forward));
    
    //fprintf(stderr, "Post Append(): position = %u\n\n", point_.GetPosition());
    // emit TextModified(TextStorage::Event(*this, Selection(*this, prev_.GetPosition() - len, prev_.GetPosition()), Insertion));

//     if (callback_)
//         callback_(TextStorage::Event(*this, Selection(*this, prev_.GetPosition() - len, prev_.GetPosition()), Insertion), userData_);

    return len;
}

void TextStorage::Dump()
{
    PieceChain::Iterator it = chain_.Begin();
    int n = 0;
    for( ; it != chain_.End(); ++it, ++n)
    {
        if (it.IsValid())
        {
            Piece* p = it.GetValue();
            fprintf(stderr, "\n{#%d, script=%d, off=%d, len=%d, glyph_len = %d, syl=%d}\n", n, p->script, p->offset, p->length, p->glyphCount, syllables_[p->offset]);
        }
        else
            fprintf(stderr, "{#%d Invalid iterator!!!}\n", n);
    }
}

/*!
  \brief Remove \a length syllables starting from \e point.
  \return The number of syllables removed or -1 in the case of errors.
*/
int TextStorage::Remove(size_t length, Direction dir)
{
    //qDebug() << ">>>>>>>>> Remove() - length = " << length << " <--------";
    
    if (dir == Backward)
        this->MoveBy(length, Backward);

    if (!point_.IsValid())
        return -1;

    pos_t position = point_.GetPosition();
    
    if (position + length > chain_.GetLength())
        return -1;

    /* Case 1: The segment of text to be deleted is within a
       piece. This has the following subcases: 
       
       1.a) The segment lies at the end of the piece. In this case,
       deletion is done by simply adjusting the length of the piece.

       1.b) The segment lies at the start of the piece. In this case,
       both the offset and the length are adjusted suitably.

       1.c) The segment lies inside the piece. In this case, the piece
       is split both at the start and at the end.
    */
    UndoRecord undo;
    undo.type = UndoRecord::Deletion;
    undo.start = point_.GetPosition();
    undo.end   = undo.start + length;
    
    pos_t offset = point_.GetOffset();
    Piece* left_piece = point_.GetValue();
    if (offset + length <= left_piece->GetLength())
    {
        size_t nglyphs = 0;
        /* record undo information */
        undo.from = new Piece(*left_piece);
        undo.to   = undo.from;
        undo.nsyllables = length;
        
        // deletion range within a piece
        if (offset == 0)
        {
            pos_t index = point_.GetDataIndex();
            for (size_t p = 0; p  < length; p++)
                nglyphs += sylWidths_[index + p];

            //adjust offset and length
            left_piece->offset += length;
            left_piece->length -= length;
            
            if (left_piece->GetLength() == 0)
            {           
                chain_.Erase(point_);
                ++point_;
            }
        }
        else if (offset + length == left_piece->GetLength())
        {
            pos_t index = point_.GetDataIndex();
            for (size_t p = 0; p < length; p++)
                nglyphs += sylWidths_[index + p];
            
            left_piece->length -= length;
            
            if (left_piece->length == 0)
            {
                chain_.Erase(point_);
                ++point_;
            }
        }
        else
        {
            PieceChain::Iterator left_iter = chain_.Split(point_, sylWidths_);
            PieceChain::Iterator right_iter = point_;
            right_iter.IncOffset(length);
            point_ = chain_.Split(right_iter, sylWidths_);
            nglyphs = CountGlyphs(left_iter, right_iter);
            chain_.Erase(left_iter, point_);
            point_.UpdatePosition(point_.GetPosition() - length - 1);
        }
        undo.nglyphs = nglyphs;
        undoRecord_ = undo;
        emit UndoRecorded();
        
        chain_.SetLength(chain_.GetLength() - length);
        return nglyphs;
    }

    // deletion range covers multiple spans
    // fix the lower bound of the range
    PieceChain::Iterator left_iter = ((offset == 0) ? point_ : chain_.Split(point_, sylWidths_));
    point_.IncOffset(length);
    PieceChain::Iterator right_iter = ((left_iter.GetValue()->GetLength() == length)
                                       ? point_
                                       : chain_.Split(point_, sylWidths_));
    size_t nglyphs = CountGlyphs(left_iter, right_iter);
    prev_ = point_;
    prev_.DecOffset();
    
    undo.from = new Piece(*left_iter.GetValue());
    undo.to   = new Piece(*prev_.GetValue());
    undo.nglyphs = nglyphs;
    undo.nsyllables = length;
    undoRecord_ = undo;
    emit UndoRecorded();

    if (left_iter.GetValue() == right_iter.GetValue())
        chain_.Erase(left_iter);
    else
        chain_.Erase(left_iter, right_iter);

    chain_.SetLength(chain_.GetLength() - length);
    point_.UpdatePosition(point_.GetPosition() - length);

    return length;
}

int TextStorage::MoveTo(pos_t pos, MoveOption option, Marker marker)
{
    if (pos == 0)
    {
	if (marker == Point)
	{
	    point_ = chain_.Begin();
	    prev_ = chain_.End();
	}
	else
	{
	    mark_ = chain_.Begin();
	}
        return 0;
    }

    int delta = 0;
    PieceChain::Iterator iter = chain_.Begin();
    
    if (option == Glyphs)
    {   
        // brute-force method to move in terms of glyphs. needs to be
        // more clever to minimize the amount of work done.
    
        pos_t glyph_pos = 0;
        while (glyph_pos + iter.GetValue()->GetGlyphCount() < pos)
        {
            glyph_pos += iter.GetValue()->GetGlyphCount();
            ++iter;
        }

        while (glyph_pos < pos)
        {
            glyph_pos += GetSylWidth(iter);
            iter.IncOffset();
        }

        delta = glyph_pos - pos;
    }
    else
    {
        // move in terms of syllables
        //FIXME: what should delta hold?
        if (pos >= Size())
        {            
            iter = chain_.End();
            iter.UpdatePosition(Size());
        }
        else
        {
            iter = chain_.Seek(pos);
        }
    }

    if (marker == Point)
    {
	point_ = iter;
	prev_ = point_;
	prev_.DecOffset();
    }
    else
    {
	mark_ = iter;
    }
    return delta;
}

/*!
  \brief Return the syllable and its associated script code at \e
  point. This is intended to be used for syllable composition.
  \returns 0 on success and -1 on error.
*/

int TextStorage::GetSyl(imli_script_t* script, syl_t* syl)
{
    int retval = 0;
    if (!point_.IsValid())
    {
        fprintf(stderr, "ERROR: point is invalid!!!, pos = %u\n\n", point_.GetPosition());
        retval = -1;
        //return -1;
    }
    *script = point_.GetValue()->GetScript();
    *syl    = syllables_[point_.GetDataIndex()];

    return retval;
}

/*!
  \brief Return the syllable and its associated script code \e before
  point. This is intended to be used for syllable composition.
  \returns 0 on success and -1 on error.
*/

int TextStorage::GetPrevSyl(imli_script_t* script, syl_t* syl)
{
    int retval = 0;
    
    if (prev_.GetPosition() != point_.GetPosition() - 1)
    {
        prev_ = point_;
        prev_.DecOffset();
    }
    
    if (!prev_.IsValid())
    {
        fprintf(stderr, "ERROR: prev_point is invalid!!!, pos = %u\n\n", prev_.GetPosition());
        return -1;
    }
    
    *script = prev_.GetValue()->GetScript();
    *syl    = syllables_[prev_.GetDataIndex()];

    return retval;
}

/*!
  \brief Helper for a special case operation. During a syllable
  composition, the preceeding syllable is replaced with the composed
  syllable. Implementing this via the \fn Remove followed by an \fn
  Insert is time consuming especially considering that syllable
  composition happens 80% of the time.
*/

void TextStorage::Substitute(imli_script_t script, syl_t syl)
{
    byte_t old_width = sylWidths_[prev_.GetDataIndex()];
    syllables_[prev_.GetDataIndex()] = syl;
    size_t new_width = 1;
    //syl_width(context_, syl, script, &new_width);
    QByteArray glyphs;
    if (IsGlyphGenerationEnabled())
    {
        glyphs.reserve(16);
        syl_to_glyphs(context_, syl, script, (byte_t*)glyphs.data(), glyphs.capacity(), &new_width);
        sylWidths_[prev_.GetDataIndex()] = new_width;
        glyphs.resize(new_width);
        // adjust piece's glyph count.
        Piece* p = prev_.GetValue();
        p->SetGlyphCount(p->GetGlyphCount() + (new_width - old_width));
    }
    
    if (IsNotificationEnabled())
        emit TextModified(TextStorage::Event(*this, Chunk(script, glyphs), Replacement, old_width));
    
    //    emit TextModified(Event(*this, Selection(*this, point_.GetPosition(), prev_.GetPosition()), Replacement, old_width));

  //   if (callback_)
//         callback_(Event(*this, Selection(*this, prev_.GetPosition(), point_.GetPosition()), Replacement, old_width), userData_);
}

/*
  \brief Move the \a marker by \a n syllables from the current position in
  the direction specified by \a direction. \returns the number of
  glyphs moved with a negative value implying moving backwards in the
  buffer and positive value moving forwards in the buffer.
*/

int TextStorage::MoveBy(pos_t units, TextStorage::Direction dir, Marker marker)
{
    int delta = 0;
    PieceChain::Iterator iter = (marker == Point) ? point_ : mark_;
    
    if (dir == TextStorage::Forward)
    {
        while (units--)
        {
            pos_t cur_pos = iter.GetPosition();
            delta += sylWidths_[iter.GetDataIndex()];
            if (cur_pos < Size())
                iter.IncOffset();
        }
    }
    else
    {
        while (units--)
        {
            if (this->AtStart())
                return delta;
            
            iter.DecOffset();
            delta += sylWidths_[iter.GetDataIndex()];
        }
    }
    if (marker == Point)
    {
	point_ = iter;
	prev_ = point_;
	prev_.DecOffset();
    }
    else
    {
	mark_ = iter;
    }
    
    return (dir == TextStorage::Forward ? delta : -delta);
}

/*
  \brief Count and store the number of glyphs of the piece \a p
  and store the value inside \a p
*/
void TextStorage::UpdateGlyphCount(Piece* p)
{
    size_t width = 0;
    for(pos_t pos = p->offset; pos < p->offset + p->length; pos++)
        width += sylWidths_[pos];

    p->glyphCount = width;
}

/* Count the number of glyphs for the syllables present in the range
 * [start, end]. Assumes start.GetPosition() < end.GetPosition()
 */
size_t TextStorage::CountGlyphs(PieceChain::Iterator start, PieceChain::Iterator end)
{
    size_t retval = 0;

    if (!start.IsValid())
        return retval;

    // within the piece?
    if (start.GetValue() == end.GetValue())
    {
        //fprintf(stderr, "Case1: retval = %d\n\n", retval);
        //TODO: see if the range [start, end] spans the entire piece in which case there
        // is no need for the following computation as the glyph count can be taken from
        // the piece itself.
        for (pos_t index = start.GetDataIndex(); index < end.GetDataIndex(); index++)
            retval += sylWidths_[index];
    }
    else
    {
        //fprintf(stderr, "Case2 - start: retval = %d\n\n", retval);
        for (pos_t i = 0; i < start.GetValue()->GetLength() - start.GetOffset(); i++)
            retval += sylWidths_[start.GetDataIndex() + i];

        //fprintf(stderr, "Case2 - middle: retval = %d\n\n", retval);
        while ((++start).GetValue() != end.GetValue())
        {
            retval += start.GetValue()->GetGlyphCount();
        }
        //fprintf(stderr, "Case2 - end: data_offset = %d\n\n", end.GetOffset());
        //      if (end.IsValid())
        //          for (pos_t i = end.GetOffset(); i >= 0; i--)
        //          {
        //              fprintf(stderr, "Case2 - end: i = %d, index = %d\n\n", i, end.GetDataIndex());
        //              retval += sylWidths_[end.GetDataIndex() - i];
        //          }
    }
    return retval;
}

inline byte_t TextStorage::GetSylWidth(PieceChain::Iterator it) const
{
    return sylWidths_[it.GetDataIndex()];
}

byte_t TextStorage::GetSylWidthAtPoint() const
{
    if (point_.IsValid())
        return sylWidths_[point_.GetDataIndex()];
    else return 0;
}

/*!
 * \brief Delete the syllable at the current insertion point.
 * \returns The width of the deleted syllable in terms of glyphs
 */
int TextStorage::DeleteSyl()
{
    if (!point_.IsValid())
    {
        emit InvalidState(QString("!!! %1 !!!").arg(point_.GetPosition()));
        return 0;
    }

    if (this->AtEnd())
    {
        emit InvalidState(QString("??? %1 ???").arg(point_.GetPosition()));
        return 0;
    }

    pos_t offset = point_.GetOffset();
    Piece* left_piece = point_.GetValue();
    size_t nglyphs = sylWidths_[point_.GetDataIndex()];
    UndoRecord undo;
    undo.type = UndoRecord::Deletion;
    undo.from = new Piece(*left_piece);
    undo.to   = undo.from;
    undo.nsyllables = 1;
    undo.nglyphs    = nglyphs;
    undo.start      = point_.GetPosition();
    undo.end        = undo.start + 1;
    undoRecord_ = undo;
    emit UndoRecorded();

    PieceChain::Iterator tmp = point_;
    
    if (offset == 0)
    {
        //qDebug() << "Case 1 point.pos = " << point_.GetPosition();
        //adjust offset and length
        left_piece->offset++;
        left_piece->length--;
        left_piece->glyphCount -= nglyphs;
        if (left_piece->GetLength() == 0)
        {
            ++point_;
            point_.UpdateOffset(0);
            //point_.UpdatePosition(point_.GetPosition() - 1);
            
            qDebug() << "removing empty piece... point.pos = " << point_.GetPosition() << " point.offset = "  << point_.GetOffset() << " point.dataidx = " << point_.GetDataIndex() ;
            chain_.Erase(tmp);
            qDebug() << "case1 after piece removal: point.pos = " << point_.GetPosition() << " point.offset = " << point_.GetOffset() << " sentinel? = " << (point_ == chain_.End());
        }
    }
    else if (offset == left_piece->GetLength() - 1)
    {
        //qDebug() << "Case 2 << point.offset = " << offset << " piece.len = " << left_piece->GetLength();
        point_.IncOffset();
        point_.UpdatePosition(point_.GetPosition() - 1);
        left_piece->length--;
        left_piece->glyphCount -= nglyphs;
        if (left_piece->GetLength() == 0)
        {
            //qDebug() << "removing empty piece...";
            chain_.Erase(tmp);
            if (point_.GetPosition() >= Size())
                point_ = chain_.End();
        }
    }
    else
    {
	//qDebug() << "Case 3 tmp.pos = " << tmp.GetPosition() << " point.pos = " << point_.GetPosition();
        point_ = chain_.Split(point_, sylWidths_);
	//qDebug() << "Case 3 after split: tmp.pos = " << tmp.GetPosition() << " point.pos = " << point_.GetPosition();
        assert(tmp.GetValue() != point_.GetValue());
        left_piece = point_.GetValue();
        left_piece->offset++;
        left_piece->length--;
        left_piece->glyphCount -= nglyphs;

        if (point_.GetValue()->GetLength() <= 0)
        {
            // qDebug() << "removing empty piece...";
            chain_.Erase(point_);
            ++point_;
            point_.UpdateOffset(0);
        }
    }

    prev_ = point_;
    prev_.DecOffset(1);
    assert(point_.GetPosition() - prev_.GetPosition() == 1);
    assert(prev_.GetOffset() == prev_.GetValue()->GetLength() - 1);
    
    chain_.SetLength(chain_.GetLength() - 1);
    chain_.SetGlyphCount(chain_.GetGlyphCount() - nglyphs);
    //assert(point_.GetPosition() <= Size());
    return nglyphs;
}

int TextStorage::DeletePreviousSyl()
{
    //FIXME: add subsyllable editing facility
    if (!AtStart())
    {
        //qDebug() << "DeletePreviousSyl: point.pos = " << point_.GetPosition();
        imli_script_t script;
        syl_t syl;
        
        if (GetPrevSyl(&script, &syl) != 0)
        {
            emit InvalidState(QString("[[[ pos = %1 off = %2]]]").arg(point_.GetPosition()).arg(point_.GetOffset()));    
            return 0;
        }
        
        if (!IS_SYL_ASCII(syl) && !IS_SYL_SPECIAL(syl))
        {
            if (VOWEL(syl) > 0 && CONSONANT(syl))
            {
                syl_t newsyl = SYLLABLE(CONSONANT(syl), CONJUNCT(syl), 0);
                Substitute(script, newsyl);
            }
            else if (CONJUNCT(syl) > 0)
            {
                syl_t newsyl = SYLLABLE(CONSONANT(syl), 0, 0);
                Substitute(script, newsyl);
            }
            else 
            {
                this->MoveBy(1, Backward);
                return DeleteSyl();
            }
        }
        else 
        {
            this->MoveBy(1, Backward);
            return DeleteSyl();
        }
    }
    else
    {
        emit InvalidState(QString("### pos = %1 off = %2###").arg(point_.GetPosition()).arg(point_.GetOffset()));
    }
    return 0;
}

inline bool TextStorage::AtEnd()
{
    return (point_.GetPosition() >= Size());
}

inline bool TextStorage::AtStart()
{
    /* unlike in AtEnd(), we can safely use the == operator for testing */
    return (point_ == chain_.Begin());
}

int TextStorage::RemoveSelection()
{
    //qDebug() << "RemoveSelection(): Mark.pos = " << mark_.GetPosition() << " Point.pos = " << point_.GetPosition();
    return Remove(point_.GetPosition() - mark_.GetPosition(), TextStorage::Backward);
}

TextStorage::Selection TextStorage::Undo()
{
    if (undoRecord_.type == UndoRecord::Insertion)
        return Selection(*this, 0, 0);
    
    if (undoRecord_.from == 0)
        return Selection(*this, 0, 0);
    
//     qDebug() << "UndoRecord: type = " << undoRecord_.type 
//              << " nsyls = " << undoRecord_.nsyllables
//              << " nglyphs = " << undoRecord_.nglyphs
//              << " from piece   = " << undoRecord_.from
//              << " to piece   = " << undoRecord_.to
//              << " start  = " << undoRecord_.start
//              << " end    = " << undoRecord_.end;

    chain_.Restore(undoRecord_.from, undoRecord_.to);
    chain_.SetLength(chain_.GetLength() + undoRecord_.nsyllables);
    chain_.SetGlyphCount(chain_.GetGlyphCount() + undoRecord_.nglyphs);
    
    pos_t start, end;
    start = undoRecord_.start;
    end   = undoRecord_.end;
    
    MoveTo(start, TextStorage::Syllables);
    undoRecord_.Clear();
    emit UndoCleared();

    return Selection(*this, start, end);
}

void TextStorage::UndoRecord::Clear()
{
    //if (from) delete from;
//     if (from != to && to)   delete to;
    nsyllables = nglyphs = 0;
    start = end = 0;
    from = to = 0;
}

bool TextStorage::CanUndo()
{
    return (undoRecord_.from && undoRecord_.nsyllables && undoRecord_.nglyphs);
}

/*!
  \brief Get the word till point.
*/
SylString TextStorage::GetWord()
{
    PieceChain::Iterator it = prev_;
    SylString s;
    
    while (it.IsValid())
    {
	syl_t syl = syllables_[it.GetDataIndex()];
	if (!IS_SYL_SPECIAL(syl) && !IS_SYL_ASCII(syl))
	    s.insert((size_t)0, (size_t)1, (char)syl);
	else
	    break;
	it.DecOffset();
    }
    return s;
}
