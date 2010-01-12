TEMPLATE = app
TARGET = aciedit
QT += core \
    gui \
    network
HEADERS += TextEditInputContext.h \
    CentralWidget.h \
    Dictionary.h \
    ListBox.h \
    PieceChain.h \
    TextEdit.h \
    TextStorage.h \
    aciedit.h \
    aciedit.loc \
    all.h \
    keyboard.h \
    libimli.h \
    version.h
SOURCES += TextEditInputContext.cpp \
    CentralWidget.cpp \
    Dictionary.cpp \
    ListBox.cpp \
    PieceChain.cpp \
    TextEdit.cpp \
    TextStorage.cpp \
    aciedit_reg.rss \
    main.cpp \
    unicode.cpp \
    aci.cpp \
    keyboard.cpp \
    aciedit.cpp \
    aciedit.rss
FORMS += aciedit.ui \
    keyboard.ui
RESOURCES += aciedit.qrc
LIBS += -llibimli.dll
symbian: { 
    TARGET.UID3 = 0xE6A4C06A
    TARGET.EPOCALLOWDLLDATA = 1
    MMP_RULES += "DEBUGGABLE_UDEBONLY"
    dynamiclibrary.sources = IFiles/libimli.dll
    dynamiclibrary.path = \sys\bin
    DEPLOYMENT += dynamiclibrary
    langfiles.sources = IFiles/langfiles/*.key
    langfiles.sources += IFiles/langfiles/generic.cnj
    langfiles.sources += IFiles/langfiles/generic.con
    langfiles.sources += IFiles/langfiles/generic.spl
    langfiles.sources += IFiles/langfiles/generic.vow
    langfiles.path = langfiles
    DEPLOYMENT += langfiles
    tabfiles.sources = IFiles/tabfiles/*.tab
    tabfiles.path = tabfiles
    DEPLOYMENT += tabfiles
    fontmap.sources = IFiles/fontmap.ini
    DEPLOYMENT += fontmap
}
