TARGET = t

TEMPLATE = app
CONFIG += debug c++11

SOURCES += test.cpp

linux:LIBS += -L../ -lbackend
linux:LIBS += -L../../sqlite3pp/src/sqlite3pp/ -lsqlite3pp
linux:LIBS += -L../../sqlite3pp/src/sqlite3/ -lsqlite3
linux:LIBS += -ldl
linux:!android:LIBS += -lpthread

debug:QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -pedantic
