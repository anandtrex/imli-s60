/*************************************************************
 * TextStorage.h
 *
 * This file is part of Acharya Text Editor
 *
 * Licensed under the GNU General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#ifndef __TEXT_H__
#define __TEXT_H__

#include <QObject>
#include <libimli.h>
#include <vector>
#include <deque>
#include <string>
#include <stack>
#include "PieceChain.h"

typedef std::basic_string<syl_t> SylString;
typedef std::basic_string<byte_t> GlyphString;

typedef unsigned int pos_t;

class TextStorage : public QObject
{
    Q_OBJECT

public:
    enum Action
    {
        None = 0,
        Insertion,
        Removal,
        Replacement
    };

    enum MoveOption
    {
        Syllables = 1,
        Glyphs
    };

    enum Direction
    {
        Forward = 1,
        Backward
    };
    
    enum Marker
    {
	Point = 1,
	Mark
    };

    TextStorage(imli_context_t context);
    ~TextStorage();
    imli_context_t GetContext();
    
    bool AtEnd();
    bool AtStart();
    
    int MoveTo(pos_t pos, MoveOption option = Glyphs, Marker marker = Point);
    int MoveBy(pos_t pos, Direction dir = Forward, Marker marker = Point);
    int Insert(imli_script_t script, const syl_t* data, size_t len);
    int Remove(size_t length, Direction dir = Forward);
    int DeleteSyl();
    int DeletePreviousSyl();
    int RemoveSelection();
    
    void Clear();
    int Replace(pos_t offset, const syl_t* data, size_t len, imli_script_t script);
    int Replace(const syl_t* data, size_t len, imli_script_t script);
    int Transliterate(pos_t offset, size_t length, imli_script_t script);
    pos_t GetInsertPos() const;
    size_t Size() const;

    int GetSyl(imli_script_t* script, syl_t* syl);
    int GetPrevSyl(imli_script_t* script, syl_t* syl);    
    void Substitute(imli_script_t script, syl_t syl);
    byte_t GetSylWidthAtPoint() const;
    SylString GetWord();

    void Dump();
    class Selection
    {
    public:
        bool IsValid() const;
        void NextRun() const;
        size_t RunLength() const;
        SylString GetData() const;
        GlyphString GetGlyphs(char* outbuf, size_t* length) const;
        imli_script_t GetScript() const
        {
            return script_;
        }
        Selection& operator =(const Selection&);
        pos_t StartPos() const
        {
            return start_;
        }
        pos_t EndPos() const
        {
            return end_;
        }
    private:
        friend class TextStorage;
        Selection(const TextStorage& text, pos_t start, pos_t end);
        const TextStorage& text_;
        pos_t start_, end_;
        mutable size_t length_;
        mutable imli_script_t script_;
        mutable PieceChain::Iterator iter_;
    };

    TextStorage::Selection GetText(pos_t start = 0, pos_t end = SENTINEL_POS);
    TextStorage::Selection GetSelection() const;    
    bool HasSelection() const;

    struct Chunk
    {
        const imli_script_t script;
        const QByteArray& glyphs;
        Chunk(imli_script_t s, QByteArray& b)
            : script(s), glyphs(b)
            {}        
    };

    struct Event
    {
        TextStorage& source;
        const Chunk& chunk;
        Action action;
        size_t nglyphs;
	Direction dir;
        Event(TextStorage& src, const Chunk& c, Action act, size_t n = 0, Direction d = Forward) : source(src), chunk(c), action(act), nglyphs(n), dir(d)
        {}
    };

    typedef void (NotifyCB)(const Event& event, void*);
    void SetNotifier(NotifyCB* cb, void* userData);
    friend class Selection;

signals:
    void TextModified(const TextStorage::Event&);
    void UndoRecorded();
    void UndoCleared();

protected:
    int InsertBetween(imli_script_t script, const syl_t* data, size_t len);
    int Prepend(imli_script_t script, const syl_t* data, size_t len);
    int Append(imli_script_t script, const syl_t* data, size_t len);
    int Remove(PieceChain::Iterator begin, PieceChain::Iterator end);
    
    byte_t GetSylWidth(PieceChain::Iterator it) const;
    void UpdateGlyphCount(Piece* p);
    size_t CountGlyphs(PieceChain::Iterator start, PieceChain::Iterator end);
    void AdjustGlyphCount(PieceChain::Iterator lhs, PieceChain::Iterator rhs);
    imli_context_t context_;
    void* userData_;
    NotifyCB* callback_;

    // stores the syllables
    std::deque<syl_t>  syllables_;
    // stores the length of each syllable
    std::deque<byte_t> sylWidths_;
    // the piece chain
    PieceChain chain_;
    // current point of insertion
    PieceChain::Iterator point_;
    // iter for the previous syllable
    PieceChain::Iterator prev_;
    // iter to denote selection start
    PieceChain::Iterator mark_;
    // container to store glyphs during notification
    QByteArray glyphs_;
    // flag to control whether notifications are sent
    bool notify_;
    // flag to control whether glyphs are generated when syllables are inserted
    bool generateGlyphs_;
    
public:
    void EnableNotification(bool flag){ notify_ = flag; }
    bool IsNotificationEnabled() { return notify_; }
             
    void EnableGlyphGeneration(bool flag) { generateGlyphs_ = flag; }
    bool IsGlyphGenerationEnabled() { return generateGlyphs_; }

signals:
    void InvalidState(QString);

protected:
    /* undo information */
    struct UndoRecord
    {
        enum Type {Deletion = 1, Insertion} type;
    
        Piece* from;
        Piece* to;
        size_t nglyphs;
        size_t nsyllables;
        pos_t start;
        pos_t end;

        void Clear();

        UndoRecord()
            : from(0), to(0), nglyphs(0), nsyllables(0)
            {}

        void operator =(const UndoRecord& rhs)
            {
                Clear();
                type = rhs.type;
                from = rhs.from;
                to   = rhs.to;
                nglyphs = rhs.nglyphs;
                nsyllables = rhs.nsyllables;
                start = rhs.start;
                end   = rhs.end;
            }
    };

    //std::stack<UndoRecord> undoStack_;
    UndoRecord undoRecord_;

public:
    bool CanUndo();
    Selection Undo();
};
#endif
