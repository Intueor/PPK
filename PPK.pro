QT += core gui widgets sql

CONFIG += c++14

win32 {
	LIBS += ..\\PPK\\pthread\\lib\\pthread_static_lib.lib
	CONFIG += console
	QMAKE_LFLAGS_DEBUG += /NODEFAULTLIB:MSVCRT /LTCG
	QMAKE_LFLAGS_RELEASE += /LTCG
}

SOURCES += \
	dialogs/dialog-about.cpp \
	dialogs/dialog-message.cpp \
	dialogs/dialog-settings.cpp \
	logger.cpp \
	main.cpp \
	mainwindow.cpp \
	msqlrelationaldelegate.cpp \
	mtableview.cpp \
	utilites.cpp

HEADERS += \
	dialogs/dialog-about.h \
	dialogs/dialog-message.h \
	dialogs/dialog-settings.h \
	logger.h \
	mainwindow.h \
	msqlrelationaldelegate.h \
	mtableview.h \
	pthread/include/_ptw32.h \
	pthread/include/pthread.h \
	pthread/include/sched.h \
	pthread/include/semaphore.h \
	rapidxml/rapidxml.hpp \
	rapidxml/rapidxml_print.hpp \
	rapidxml/rapidxml_utils.hpp \
	utilites.h \
	widgets-serializer.h

FORMS += \
	dialogs/dialog-about.ui \
	dialogs/dialog-message.ui \
	dialogs/dialog-settings.ui \
	mainwindow.ui

DISTFILES += \
	Doxyfile \
	LICENSE \
	PPK.desktop \
	PPK.png \
	README.md \
	cleanup \
	default.db \
	deployment \
	linux_project_settings.txt \
	pthread/lib/pthread_static_lib.lib

RESOURCES += \
	resources.qrc
