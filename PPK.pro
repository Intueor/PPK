QT += core gui widgets

CONFIG += c++14

SOURCES += \
    logger.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    logger.h \
    mainwindow.h \
    rapidxml/rapidxml.hpp \
    rapidxml/rapidxml_print.hpp \
    rapidxml/rapidxml_utils.hpp

FORMS += \
    mainwindow.ui

DISTFILES += \
	Doxyfile \
	LICENSE \
	PPK.desktop \
	PPK.png \
	README.md \
	cleanup \
	deployment \
	linux_project_settings.txt

RESOURCES += \
	resources.qrc
