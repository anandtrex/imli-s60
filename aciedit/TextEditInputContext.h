#ifndef TEXTEDITINPUTCONTEXT_H
#define TEXTEDITINPUTCONTEXT_H

#include <QtGui/QInputContext>

class TextEditInputContext : public QInputContext
 {
     Q_OBJECT

 public:
     TextEditInputContext();
     
     QString identifierName();
     QString language();
     bool isComposing() const;
     void reset();

     bool filterEvent(const QEvent* event);
};

#endif // TEXTEDITINPUTCONTEXT_H
