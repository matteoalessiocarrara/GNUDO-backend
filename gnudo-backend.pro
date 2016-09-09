TEMPLATE = subdirs
SUBDIRS = sqlite3pp backend t

sqlite3pp.subdir = lib/sqlite3pp
backend.subdir = src
t.subdir = src/test

backend.depends = sqlite3pp
t.depends = backend