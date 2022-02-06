QT += core gui widgets

CONFIG += c++14

win32 {
	LIBS += ..\\PPK\\pthread\\lib\\pthread_static_lib.lib
	CONFIG += console
	QMAKE_LFLAGS_DEBUG += /NODEFAULTLIB:MSVCRT
}

SOURCES += \
	logger.cpp \
	main.cpp \
	mainwindow.cpp

HEADERS += \
	logger.h \
	mainwindow.h \
	pthread/include/_ptw32.h \
	pthread/include/pthread.h \
	pthread/include/sched.h \
	pthread/include/semaphore.h \
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
	pthread/lib/pthread_static_lib.lib

RESOURCES += \
	resources.qrc
