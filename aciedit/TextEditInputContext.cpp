#include <QtCore>
#include "TextEditInputContext.h"
#include "TextEdit.h"

TextEditInputContext::TextEditInputContext()
{
}

bool TextEditInputContext::filterEvent(const QEvent* event)
{
	 if (event->type() == QEvent::RequestSoftwareInputPanel) {
	 //TODO show the keyboard here
		 TextEdit *w = qobject_cast<TextEdit*>(this->sender());
		 if(w){
			 qDebug()<<"Ignored Event";
			 return true;
		 }
	 }
	     return false;
}

QString TextEditInputContext::identifierName()
{
     return "TextEditInputContext";
}

void TextEditInputContext::reset()
{
}

QString TextEditInputContext::language()
{
     return "en_US";
}

bool TextEditInputContext::isComposing() const
{
    return false;
}
