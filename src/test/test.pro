TARGET = t

TEMPLATE = app
CONFIG += debug c++11

SOURCES += test.cpp

linux:LIBS += -L../ -lbackend
linux:LIBS += -L../../lib/sqlite3pp/src -lsqlite3pp
linux:LIBS += -L../../lib/sqlite3pp/lib/sqlite3/ -lsqlite3
linux:LIBS += -ldl
linux:!android:LIBS += -lpthread

debug:QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -pedantic
