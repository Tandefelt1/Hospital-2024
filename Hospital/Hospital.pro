TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        careperiod.cpp \
        cli.cpp \
        date.cpp \
        hospital.cpp \
        main.cpp \
        person.cpp \
        utils.cpp

HEADERS += \
    careperiod.hh \
    cli.hh \
    date.hh \
    hospital.hh \
    person.hh \
    utils.hh
