QT += core widgets

CONFIG += c++17

TARGET = biblioteca_virtuale
TEMPLATE = app

SOURCES += \
    main.cpp \
    model/Media.cpp \
    model/Book.cpp \
    model/Film.cpp \
    model/MagazineArticle.cpp \
    model/Biblioteca.cpp \
    model/User.cpp \
    model/UserAuthenticator.cpp \
    model/MediaFactory.cpp \
    view/MainWindow.cpp \
    view/LoginDialog.cpp \
    view/MediaWidgetVisitor.cpp \
    view/AddMediaDialog.cpp \
    view/EditMediaDialog.cpp \
    view/MediaCollectorVisitor.cpp \
    persistence/JsonSerializer.cpp

HEADERS += \
    model/Media.h \
    model/Book.h \
    model/Film.h \
    model/MagazineArticle.h \
    model/MediaVisitor.h \
    model/Biblioteca.h \
    model/User.h \
    model/UserAuthenticator.h \
    model/Container.h \
    model/Exceptions.h \
    model/MediaFactory.h \
    view/MainWindow.h \
    view/LoginDialog.h \
    view/MediaWidgetVisitor.h \
    view/AddMediaDialog.h \
    view/EditMediaDialog.h \
    view/MediaCollectorVisitor.h \
    persistence/JsonSerializer.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

