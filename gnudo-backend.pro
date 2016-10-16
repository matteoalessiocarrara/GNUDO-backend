TEMPLATE = subdirs
SUBDIRS = sqlite3pp backend t

sqlite3pp.subdir = src/sqlite3pp
backend.subdir = src/gnudo-backend
t.subdir = src/gnudo-backend/test

backend.depends = sqlite3pp
t.depends = backend