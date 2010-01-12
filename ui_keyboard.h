/********************************************************************************
** Form generated from reading UI file 'keyboard.ui'
**
** Created: Wed Jan 6 18:10:45 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KEYBOARD_H
#define UI_KEYBOARD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VirtualKeyboard
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *vlayout;
    QHBoxLayout *numrow;
    QPushButton *grave_tilda;
    QPushButton *one_exclamation;
    QPushButton *two_at;
    QPushButton *three_hash;
    QPushButton *four_dollar;
    QPushButton *five_percentage;
    QPushButton *six_caret;
    QPushButton *seven_ampersand;
    QPushButton *eight_star;
    QPushButton *nine_openBrace;
    QPushButton *zero_closeBrace;
    QPushButton *minus_underscore;
    QPushButton *equals_plus;
    QHBoxLayout *qwertyrow;
    QPushButton *qQ;
    QPushButton *wW;
    QPushButton *eE;
    QPushButton *rR;
    QPushButton *tT;
    QPushButton *yY;
    QPushButton *uU;
    QPushButton *iI;
    QPushButton *oO;
    QPushButton *pP;
    QPushButton *openSquareBrace_openCurlyBrace;
    QPushButton *closeSquareBrace_closeCurlyBrace;
    QPushButton *backslash_pipe;
    QHBoxLayout *asdfrow;
    QPushButton *aA;
    QPushButton *sS;
    QPushButton *dD;
    QPushButton *fF;
    QPushButton *gG;
    QPushButton *hH;
    QPushButton *jJ;
    QPushButton *kK;
    QPushButton *lL;
    QPushButton *semicolon_colon;
    QPushButton *singleQuote_doubleQuote;
    QPushButton *enter;
    QHBoxLayout *zxcvrow;
    QPushButton *shift;
    QPushButton *zZ;
    QPushButton *xX;
    QPushButton *cC;
    QPushButton *vV;
    QPushButton *bB;
    QPushButton *nN;
    QPushButton *mM;
    QPushButton *comma_lessThan;
    QPushButton *period_greaterThan;
    QPushButton *frontslash_questionMark;
    QHBoxLayout *spacerow;
    QPushButton *tab;
    QPushButton *space;
    QPushButton *caps;
    QPushButton *backspace;

    void setupUi(QFrame *VirtualKeyboard)
    {
        if (VirtualKeyboard->objectName().isEmpty())
            VirtualKeyboard->setObjectName(QString::fromUtf8("VirtualKeyboard"));
        VirtualKeyboard->resize(360, 220);
        VirtualKeyboard->setMinimumSize(QSize(360, 220));
        VirtualKeyboard->setFrameShape(QFrame::StyledPanel);
        VirtualKeyboard->setFrameShadow(QFrame::Raised);
        VirtualKeyboard->setLineWidth(1);
        verticalLayoutWidget = new QWidget(VirtualKeyboard);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 361, 221));
        vlayout = new QVBoxLayout(verticalLayoutWidget);
        vlayout->setSpacing(0);
        vlayout->setObjectName(QString::fromUtf8("vlayout"));
        vlayout->setContentsMargins(0, 0, 0, 0);
        numrow = new QHBoxLayout();
        numrow->setSpacing(0);
        numrow->setObjectName(QString::fromUtf8("numrow"));
        numrow->setSizeConstraint(QLayout::SetDefaultConstraint);
        numrow->setContentsMargins(2, -1, 2, -1);
        grave_tilda = new QPushButton(verticalLayoutWidget);
        grave_tilda->setObjectName(QString::fromUtf8("grave_tilda"));
        grave_tilda->setMinimumSize(QSize(26, 35));
        grave_tilda->setMaximumSize(QSize(26, 35));
        grave_tilda->setFlat(true);

        numrow->addWidget(grave_tilda);

        one_exclamation = new QPushButton(verticalLayoutWidget);
        one_exclamation->setObjectName(QString::fromUtf8("one_exclamation"));
        one_exclamation->setMinimumSize(QSize(26, 35));
        one_exclamation->setMaximumSize(QSize(26, 35));
        one_exclamation->setFlat(true);

        numrow->addWidget(one_exclamation);

        two_at = new QPushButton(verticalLayoutWidget);
        two_at->setObjectName(QString::fromUtf8("two_at"));
        two_at->setMinimumSize(QSize(26, 35));
        two_at->setMaximumSize(QSize(26, 35));
        two_at->setFlat(true);

        numrow->addWidget(two_at);

        three_hash = new QPushButton(verticalLayoutWidget);
        three_hash->setObjectName(QString::fromUtf8("three_hash"));
        three_hash->setMinimumSize(QSize(26, 35));
        three_hash->setMaximumSize(QSize(26, 35));
        three_hash->setFlat(true);

        numrow->addWidget(three_hash);

        four_dollar = new QPushButton(verticalLayoutWidget);
        four_dollar->setObjectName(QString::fromUtf8("four_dollar"));
        four_dollar->setMinimumSize(QSize(26, 35));
        four_dollar->setMaximumSize(QSize(26, 35));
        four_dollar->setFlat(true);

        numrow->addWidget(four_dollar);

        five_percentage = new QPushButton(verticalLayoutWidget);
        five_percentage->setObjectName(QString::fromUtf8("five_percentage"));
        five_percentage->setMinimumSize(QSize(26, 35));
        five_percentage->setMaximumSize(QSize(26, 35));
        five_percentage->setFlat(true);

        numrow->addWidget(five_percentage);

        six_caret = new QPushButton(verticalLayoutWidget);
        six_caret->setObjectName(QString::fromUtf8("six_caret"));
        six_caret->setMinimumSize(QSize(26, 35));
        six_caret->setMaximumSize(QSize(26, 35));
        six_caret->setFlat(true);

        numrow->addWidget(six_caret);

        seven_ampersand = new QPushButton(verticalLayoutWidget);
        seven_ampersand->setObjectName(QString::fromUtf8("seven_ampersand"));
        seven_ampersand->setMinimumSize(QSize(26, 35));
        seven_ampersand->setMaximumSize(QSize(26, 35));
        seven_ampersand->setFlat(true);

        numrow->addWidget(seven_ampersand);

        eight_star = new QPushButton(verticalLayoutWidget);
        eight_star->setObjectName(QString::fromUtf8("eight_star"));
        eight_star->setMinimumSize(QSize(26, 35));
        eight_star->setMaximumSize(QSize(26, 35));
        eight_star->setFlat(true);

        numrow->addWidget(eight_star);

        nine_openBrace = new QPushButton(verticalLayoutWidget);
        nine_openBrace->setObjectName(QString::fromUtf8("nine_openBrace"));
        nine_openBrace->setMinimumSize(QSize(26, 35));
        nine_openBrace->setMaximumSize(QSize(26, 35));
        nine_openBrace->setFlat(true);

        numrow->addWidget(nine_openBrace);

        zero_closeBrace = new QPushButton(verticalLayoutWidget);
        zero_closeBrace->setObjectName(QString::fromUtf8("zero_closeBrace"));
        zero_closeBrace->setMinimumSize(QSize(26, 35));
        zero_closeBrace->setMaximumSize(QSize(26, 35));
        zero_closeBrace->setFlat(true);

        numrow->addWidget(zero_closeBrace);

        minus_underscore = new QPushButton(verticalLayoutWidget);
        minus_underscore->setObjectName(QString::fromUtf8("minus_underscore"));
        minus_underscore->setMinimumSize(QSize(26, 35));
        minus_underscore->setMaximumSize(QSize(26, 35));
        minus_underscore->setFlat(true);

        numrow->addWidget(minus_underscore);

        equals_plus = new QPushButton(verticalLayoutWidget);
        equals_plus->setObjectName(QString::fromUtf8("equals_plus"));
        equals_plus->setMinimumSize(QSize(26, 35));
        equals_plus->setMaximumSize(QSize(26, 35));
        equals_plus->setFlat(true);

        numrow->addWidget(equals_plus);


        vlayout->addLayout(numrow);

        qwertyrow = new QHBoxLayout();
        qwertyrow->setSpacing(0);
        qwertyrow->setObjectName(QString::fromUtf8("qwertyrow"));
        qwertyrow->setSizeConstraint(QLayout::SetDefaultConstraint);
        qwertyrow->setContentsMargins(2, -1, 2, -1);
        qQ = new QPushButton(verticalLayoutWidget);
        qQ->setObjectName(QString::fromUtf8("qQ"));
        qQ->setMinimumSize(QSize(26, 35));
        qQ->setMaximumSize(QSize(26, 35));
        qQ->setFlat(true);

        qwertyrow->addWidget(qQ);

        wW = new QPushButton(verticalLayoutWidget);
        wW->setObjectName(QString::fromUtf8("wW"));
        wW->setMinimumSize(QSize(26, 35));
        wW->setMaximumSize(QSize(26, 35));
        wW->setFlat(true);

        qwertyrow->addWidget(wW);

        eE = new QPushButton(verticalLayoutWidget);
        eE->setObjectName(QString::fromUtf8("eE"));
        eE->setMinimumSize(QSize(26, 35));
        eE->setMaximumSize(QSize(26, 35));
        eE->setFlat(true);

        qwertyrow->addWidget(eE);

        rR = new QPushButton(verticalLayoutWidget);
        rR->setObjectName(QString::fromUtf8("rR"));
        rR->setMinimumSize(QSize(26, 35));
        rR->setMaximumSize(QSize(26, 35));
        rR->setFlat(true);

        qwertyrow->addWidget(rR);

        tT = new QPushButton(verticalLayoutWidget);
        tT->setObjectName(QString::fromUtf8("tT"));
        tT->setMinimumSize(QSize(26, 35));
        tT->setMaximumSize(QSize(26, 35));
        tT->setFlat(true);

        qwertyrow->addWidget(tT);

        yY = new QPushButton(verticalLayoutWidget);
        yY->setObjectName(QString::fromUtf8("yY"));
        yY->setMinimumSize(QSize(26, 35));
        yY->setMaximumSize(QSize(26, 35));
        yY->setFlat(true);

        qwertyrow->addWidget(yY);

        uU = new QPushButton(verticalLayoutWidget);
        uU->setObjectName(QString::fromUtf8("uU"));
        uU->setMinimumSize(QSize(26, 35));
        uU->setMaximumSize(QSize(26, 35));
        uU->setFlat(true);

        qwertyrow->addWidget(uU);

        iI = new QPushButton(verticalLayoutWidget);
        iI->setObjectName(QString::fromUtf8("iI"));
        iI->setMinimumSize(QSize(26, 35));
        iI->setMaximumSize(QSize(26, 35));
        iI->setFlat(true);

        qwertyrow->addWidget(iI);

        oO = new QPushButton(verticalLayoutWidget);
        oO->setObjectName(QString::fromUtf8("oO"));
        oO->setMinimumSize(QSize(26, 35));
        oO->setMaximumSize(QSize(26, 35));
        oO->setFlat(true);

        qwertyrow->addWidget(oO);

        pP = new QPushButton(verticalLayoutWidget);
        pP->setObjectName(QString::fromUtf8("pP"));
        pP->setMinimumSize(QSize(26, 35));
        pP->setMaximumSize(QSize(26, 35));
        pP->setFlat(true);

        qwertyrow->addWidget(pP);

        openSquareBrace_openCurlyBrace = new QPushButton(verticalLayoutWidget);
        openSquareBrace_openCurlyBrace->setObjectName(QString::fromUtf8("openSquareBrace_openCurlyBrace"));
        openSquareBrace_openCurlyBrace->setMinimumSize(QSize(26, 35));
        openSquareBrace_openCurlyBrace->setMaximumSize(QSize(26, 35));
        openSquareBrace_openCurlyBrace->setFlat(true);

        qwertyrow->addWidget(openSquareBrace_openCurlyBrace);

        closeSquareBrace_closeCurlyBrace = new QPushButton(verticalLayoutWidget);
        closeSquareBrace_closeCurlyBrace->setObjectName(QString::fromUtf8("closeSquareBrace_closeCurlyBrace"));
        closeSquareBrace_closeCurlyBrace->setMinimumSize(QSize(26, 35));
        closeSquareBrace_closeCurlyBrace->setMaximumSize(QSize(26, 35));
        closeSquareBrace_closeCurlyBrace->setFlat(true);

        qwertyrow->addWidget(closeSquareBrace_closeCurlyBrace);

        backslash_pipe = new QPushButton(verticalLayoutWidget);
        backslash_pipe->setObjectName(QString::fromUtf8("backslash_pipe"));
        backslash_pipe->setMinimumSize(QSize(26, 35));
        backslash_pipe->setMaximumSize(QSize(26, 35));
        backslash_pipe->setFlat(true);

        qwertyrow->addWidget(backslash_pipe);


        vlayout->addLayout(qwertyrow);

        asdfrow = new QHBoxLayout();
        asdfrow->setSpacing(0);
        asdfrow->setObjectName(QString::fromUtf8("asdfrow"));
        asdfrow->setSizeConstraint(QLayout::SetDefaultConstraint);
        asdfrow->setContentsMargins(2, -1, 2, -1);
        aA = new QPushButton(verticalLayoutWidget);
        aA->setObjectName(QString::fromUtf8("aA"));
        aA->setMinimumSize(QSize(26, 35));
        aA->setMaximumSize(QSize(26, 35));
        aA->setFlat(true);

        asdfrow->addWidget(aA);

        sS = new QPushButton(verticalLayoutWidget);
        sS->setObjectName(QString::fromUtf8("sS"));
        sS->setMinimumSize(QSize(26, 35));
        sS->setMaximumSize(QSize(26, 35));
        sS->setFlat(true);

        asdfrow->addWidget(sS);

        dD = new QPushButton(verticalLayoutWidget);
        dD->setObjectName(QString::fromUtf8("dD"));
        dD->setMinimumSize(QSize(26, 35));
        dD->setMaximumSize(QSize(26, 35));
        dD->setFlat(true);

        asdfrow->addWidget(dD);

        fF = new QPushButton(verticalLayoutWidget);
        fF->setObjectName(QString::fromUtf8("fF"));
        fF->setMinimumSize(QSize(26, 35));
        fF->setMaximumSize(QSize(26, 35));
        fF->setFlat(true);

        asdfrow->addWidget(fF);

        gG = new QPushButton(verticalLayoutWidget);
        gG->setObjectName(QString::fromUtf8("gG"));
        gG->setMinimumSize(QSize(26, 35));
        gG->setMaximumSize(QSize(26, 35));
        gG->setFlat(true);

        asdfrow->addWidget(gG);

        hH = new QPushButton(verticalLayoutWidget);
        hH->setObjectName(QString::fromUtf8("hH"));
        hH->setMinimumSize(QSize(26, 35));
        hH->setMaximumSize(QSize(26, 35));
        hH->setFlat(true);

        asdfrow->addWidget(hH);

        jJ = new QPushButton(verticalLayoutWidget);
        jJ->setObjectName(QString::fromUtf8("jJ"));
        jJ->setMinimumSize(QSize(26, 35));
        jJ->setMaximumSize(QSize(26, 35));
        jJ->setFlat(true);

        asdfrow->addWidget(jJ);

        kK = new QPushButton(verticalLayoutWidget);
        kK->setObjectName(QString::fromUtf8("kK"));
        kK->setMinimumSize(QSize(26, 35));
        kK->setMaximumSize(QSize(26, 35));
        kK->setFlat(true);

        asdfrow->addWidget(kK);

        lL = new QPushButton(verticalLayoutWidget);
        lL->setObjectName(QString::fromUtf8("lL"));
        lL->setMinimumSize(QSize(26, 35));
        lL->setMaximumSize(QSize(26, 35));
        lL->setFlat(true);

        asdfrow->addWidget(lL);

        semicolon_colon = new QPushButton(verticalLayoutWidget);
        semicolon_colon->setObjectName(QString::fromUtf8("semicolon_colon"));
        semicolon_colon->setMinimumSize(QSize(26, 35));
        semicolon_colon->setMaximumSize(QSize(26, 35));
        semicolon_colon->setFlat(true);

        asdfrow->addWidget(semicolon_colon);

        singleQuote_doubleQuote = new QPushButton(verticalLayoutWidget);
        singleQuote_doubleQuote->setObjectName(QString::fromUtf8("singleQuote_doubleQuote"));
        singleQuote_doubleQuote->setMinimumSize(QSize(26, 35));
        singleQuote_doubleQuote->setMaximumSize(QSize(26, 35));
        singleQuote_doubleQuote->setFlat(true);

        asdfrow->addWidget(singleQuote_doubleQuote);

        enter = new QPushButton(verticalLayoutWidget);
        enter->setObjectName(QString::fromUtf8("enter"));
        enter->setMinimumSize(QSize(53, 35));
        enter->setMaximumSize(QSize(53, 35));
        enter->setFlat(true);

        asdfrow->addWidget(enter);


        vlayout->addLayout(asdfrow);

        zxcvrow = new QHBoxLayout();
        zxcvrow->setSpacing(0);
        zxcvrow->setObjectName(QString::fromUtf8("zxcvrow"));
        zxcvrow->setSizeConstraint(QLayout::SetDefaultConstraint);
        zxcvrow->setContentsMargins(2, -1, 2, -1);
        shift = new QPushButton(verticalLayoutWidget);
        shift->setObjectName(QString::fromUtf8("shift"));
        shift->setMinimumSize(QSize(80, 35));
        shift->setMaximumSize(QSize(80, 35));
        shift->setCheckable(true);
        shift->setFlat(true);

        zxcvrow->addWidget(shift);

        zZ = new QPushButton(verticalLayoutWidget);
        zZ->setObjectName(QString::fromUtf8("zZ"));
        zZ->setMinimumSize(QSize(26, 35));
        zZ->setMaximumSize(QSize(26, 35));
        zZ->setFlat(true);

        zxcvrow->addWidget(zZ);

        xX = new QPushButton(verticalLayoutWidget);
        xX->setObjectName(QString::fromUtf8("xX"));
        xX->setMinimumSize(QSize(26, 35));
        xX->setMaximumSize(QSize(26, 35));
        xX->setFlat(true);

        zxcvrow->addWidget(xX);

        cC = new QPushButton(verticalLayoutWidget);
        cC->setObjectName(QString::fromUtf8("cC"));
        cC->setMinimumSize(QSize(26, 35));
        cC->setMaximumSize(QSize(26, 35));
        cC->setFlat(true);

        zxcvrow->addWidget(cC);

        vV = new QPushButton(verticalLayoutWidget);
        vV->setObjectName(QString::fromUtf8("vV"));
        vV->setMinimumSize(QSize(26, 35));
        vV->setMaximumSize(QSize(26, 35));
        vV->setFlat(true);

        zxcvrow->addWidget(vV);

        bB = new QPushButton(verticalLayoutWidget);
        bB->setObjectName(QString::fromUtf8("bB"));
        bB->setMinimumSize(QSize(26, 35));
        bB->setMaximumSize(QSize(26, 35));
        bB->setFlat(true);

        zxcvrow->addWidget(bB);

        nN = new QPushButton(verticalLayoutWidget);
        nN->setObjectName(QString::fromUtf8("nN"));
        nN->setMinimumSize(QSize(26, 35));
        nN->setMaximumSize(QSize(26, 35));
        nN->setFlat(true);

        zxcvrow->addWidget(nN);

        mM = new QPushButton(verticalLayoutWidget);
        mM->setObjectName(QString::fromUtf8("mM"));
        mM->setMinimumSize(QSize(26, 35));
        mM->setMaximumSize(QSize(26, 35));
        mM->setFlat(true);

        zxcvrow->addWidget(mM);

        comma_lessThan = new QPushButton(verticalLayoutWidget);
        comma_lessThan->setObjectName(QString::fromUtf8("comma_lessThan"));
        comma_lessThan->setMinimumSize(QSize(26, 35));
        comma_lessThan->setMaximumSize(QSize(26, 35));
        comma_lessThan->setFlat(true);

        zxcvrow->addWidget(comma_lessThan);

        period_greaterThan = new QPushButton(verticalLayoutWidget);
        period_greaterThan->setObjectName(QString::fromUtf8("period_greaterThan"));
        period_greaterThan->setMinimumSize(QSize(26, 35));
        period_greaterThan->setMaximumSize(QSize(26, 35));
        period_greaterThan->setFlat(true);

        zxcvrow->addWidget(period_greaterThan);

        frontslash_questionMark = new QPushButton(verticalLayoutWidget);
        frontslash_questionMark->setObjectName(QString::fromUtf8("frontslash_questionMark"));
        frontslash_questionMark->setMinimumSize(QSize(26, 35));
        frontslash_questionMark->setMaximumSize(QSize(26, 35));
        frontslash_questionMark->setFlat(true);

        zxcvrow->addWidget(frontslash_questionMark);


        vlayout->addLayout(zxcvrow);

        spacerow = new QHBoxLayout();
        spacerow->setSpacing(0);
        spacerow->setObjectName(QString::fromUtf8("spacerow"));
        spacerow->setSizeConstraint(QLayout::SetDefaultConstraint);
        tab = new QPushButton(verticalLayoutWidget);
        tab->setObjectName(QString::fromUtf8("tab"));
        tab->setMinimumSize(QSize(55, 35));
        tab->setMaximumSize(QSize(55, 35));
        tab->setFlat(true);

        spacerow->addWidget(tab);

        space = new QPushButton(verticalLayoutWidget);
        space->setObjectName(QString::fromUtf8("space"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(space->sizePolicy().hasHeightForWidth());
        space->setSizePolicy(sizePolicy);
        space->setMinimumSize(QSize(150, 35));
        space->setMaximumSize(QSize(150, 35));
        space->setAutoRepeat(true);
        space->setFlat(true);

        spacerow->addWidget(space);

        caps = new QPushButton(verticalLayoutWidget);
        caps->setObjectName(QString::fromUtf8("caps"));
        caps->setMinimumSize(QSize(65, 35));
        caps->setMaximumSize(QSize(65, 35));
        caps->setFlat(true);

        spacerow->addWidget(caps);

        backspace = new QPushButton(verticalLayoutWidget);
        backspace->setObjectName(QString::fromUtf8("backspace"));
        backspace->setMinimumSize(QSize(75, 35));
        backspace->setMaximumSize(QSize(75, 35));
        backspace->setAutoRepeat(true);
        backspace->setFlat(true);

        spacerow->addWidget(backspace);


        vlayout->addLayout(spacerow);


        retranslateUi(VirtualKeyboard);

        QMetaObject::connectSlotsByName(VirtualKeyboard);
    } // setupUi

    void retranslateUi(QFrame *VirtualKeyboard)
    {
        VirtualKeyboard->setWindowTitle(QApplication::translate("VirtualKeyboard", "Frame", 0, QApplication::UnicodeUTF8));
        VirtualKeyboard->setStyleSheet(QApplication::translate("VirtualKeyboard", "QPushButton{\n"
"	color:white;\n"
"	background-color:black;\n"
"	border: 1px solid grey;\n"
"	margin: 0px;\n"
"}\n"
"\n"
"QPushButton:pressed{\n"
"	background-color: white;\n"
"}\n"
"\n"
"QPushButton:disabled{\n"
"	background-color:grey;\n"
"}", 0, QApplication::UnicodeUTF8));
        grave_tilda->setStyleSheet(QString());
        grave_tilda->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        one_exclamation->setStyleSheet(QString());
        one_exclamation->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        two_at->setStyleSheet(QString());
        two_at->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        three_hash->setStyleSheet(QString());
        three_hash->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        four_dollar->setStyleSheet(QString());
        four_dollar->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        five_percentage->setStyleSheet(QString());
        five_percentage->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        six_caret->setStyleSheet(QString());
        six_caret->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        seven_ampersand->setStyleSheet(QString());
        seven_ampersand->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        eight_star->setStyleSheet(QString());
        eight_star->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        nine_openBrace->setStyleSheet(QString());
        nine_openBrace->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        zero_closeBrace->setStyleSheet(QString());
        zero_closeBrace->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        minus_underscore->setStyleSheet(QString());
        minus_underscore->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        equals_plus->setStyleSheet(QString());
        equals_plus->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        qQ->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        wW->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        eE->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        rR->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        tT->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        yY->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        uU->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        iI->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        oO->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        pP->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        openSquareBrace_openCurlyBrace->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        closeSquareBrace_closeCurlyBrace->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        backslash_pipe->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        aA->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        sS->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        dD->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        fF->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        gG->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        hH->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        jJ->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        kK->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        lL->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        semicolon_colon->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        singleQuote_doubleQuote->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        enter->setText(QApplication::translate("VirtualKeyboard", "Entr", 0, QApplication::UnicodeUTF8));
        shift->setText(QApplication::translate("VirtualKeyboard", "Shift", 0, QApplication::UnicodeUTF8));
        zZ->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        xX->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        cC->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        vV->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        bB->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        nN->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        mM->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        comma_lessThan->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        period_greaterThan->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        frontslash_questionMark->setText(QApplication::translate("VirtualKeyboard", "PushButton", 0, QApplication::UnicodeUTF8));
        tab->setText(QApplication::translate("VirtualKeyboard", "Tab", 0, QApplication::UnicodeUTF8));
        space->setText(QApplication::translate("VirtualKeyboard", "Space", 0, QApplication::UnicodeUTF8));
        caps->setText(QApplication::translate("VirtualKeyboard", "Caps", 0, QApplication::UnicodeUTF8));
        backspace->setText(QApplication::translate("VirtualKeyboard", "<-", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class VirtualKeyboard: public Ui_VirtualKeyboard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KEYBOARD_H
