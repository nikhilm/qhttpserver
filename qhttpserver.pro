CONFIG += ordered

TEMPLATE = subdirs

SUBDIRS += src \
           tests \
           examples

tests.depends = src
examples.depends = src
