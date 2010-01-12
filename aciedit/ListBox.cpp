/*************************************************************
 * ListBox.cpp
 *
 * This file is part of Acharya Text Editor
 *
 * Licensed under the GNU General Public License v2
 *
 * Author: V.Krishnakumar <vkrishnakumar@users.sf.net>
 *
 * Copyright Ushodaya Enterprises Ltd., 2007
 *************************************************************/

#include "ListBox.h"
#include "libimli.h"

ListBox::ListBox(imli_context_t ctx, std::vector<SylString>& wordlist, QTextCharFormat& f, QWidget* parent)
    : QListWidget(parent),
      ctx_(ctx), 
      data_(wordlist),
      format_(f)
{
    CreateItems();
    setCurrentRow(0);
}

ListBox::~ListBox()
{
}

void ListBox::CreateItems()
{
    if (format_.fontPointSize() > 16.0)
	format_.setFontPointSize(16.0);

    for (size_t i = 0; i < data_.size(); i++)
    {
		ListItem* item = new ListItem(ctx_, data_[i], this);
		item->setFont(format_.font());
    }
    setMinimumSize(minimumSize().width(), sizeHint().height());
}

void ListBox::focusOutEvent(QFocusEvent* event)
{
    deleteLater();
}

void ListBox::keyPressEvent(QKeyEvent* event)
{
    int key = event->key();
    if (key == Qt::Key_Escape)
    {
		focusPreviousChild();
		deleteLater();
		event->accept();
    }
    else
    {
		QListWidget::keyPressEvent(event);
    }
}

ListItem::ListItem(imli_context_t ctx, SylString s, QListWidget* parent)
    : QListWidgetItem(parent),
      ctx_(ctx),
      str_(s)
{
    SylsToGlyphs();
}

void ListItem::SylsToGlyphs()
{
    QByteArray glyphs;
    imli_script_t script = imli_get_active_script(ctx_);
    glyphs.reserve(str_.size()*8);

    size_t len = 0;
    imli_status_t status =
	syls_to_glyphs(ctx_,
		       str_.data(),
		       str_.length(),
		       script,
		       (byte_t*)glyphs.data(),
		       glyphs.capacity(),
		       &len);

    if (status == IMLI_STATUS_OK)
    {
		glyphs.resize(len);
		setText(QString::fromAscii(glyphs, glyphs.size()));
    }
}
