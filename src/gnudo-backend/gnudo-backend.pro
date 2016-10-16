TARGET = backend

TEMPLATE = lib
CONFIG = static debug c++11

SOURCES = gnudo.cpp managers.cpp objects.cpp
HEADERS = dbdefs.hpp gnudo.hpp managers.hpp objects.hpp

debug:QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -pedantic
