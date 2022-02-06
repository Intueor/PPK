QT += core gui widgets

CONFIG += c++14

win32 {
LIBS += ../PPK/pthread/lib/pthreadVC2.lib
CONFIG += console
}

SOURCES += \
	logger.cpp \
	main.cpp \
	mainwindow.cpp

HEADERS += \
	logger.h \
	mainwindow.h \
	pthread/include/pthread.h \
	pthread/include/sched.h \
	pthread/include/semaphore.h \
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
	linux_project_settings.txt \
	pthread/lib/pthreadVC2 — копия.lib \
	pthread/lib/pthreadVC2.lib \
	pthread/lib/Текстовый документ.txt \
	pthread/lib/Текстовый документ.txt

RESOURCES += \
	resources.qrc
