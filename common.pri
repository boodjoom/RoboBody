
Platform = win32
Configuration = release
Compiler = unknown

win32 {
    contains(QMAKE_HOST.arch, x86_64): {
        Platform = x64
    }
}
else {
    qnx {
        Platform = qnx
    }
    else {
        unix {
            Platform = unix
        }
        else {
            Platform = unknown
        }
    }
}

windows {
    *-g++* {
        # MinGW
        QMAKE_CXXFLAGS += -Wno-unused-local-typedefs #-Wno-unused-parameter
        Compiler = mingw
    }

    *-msvc* {
        # MSVC. All others (see below).
        Compiler = msvc
    }

    *-msvc2010* {
        # MSVC 2010
        Compiler = msvc2010
    }

    *-msvc2012* {
        # MSVC 2012
        Compiler = msvc2012
    }

    *-msvc2013* {
        # MSVC 2013
        Compiler = msvc2013
    }
}

qnx {
    *-g++*  {
        Compiler = gcc

        # gcc-4.4.2 cannot recognize parameter -Wno-unused-local-typedefs
        QMAKE_CXXFLAGS += -Wno-unused-parameter
    }

    *-qcc*  {
        Compiler = qcc
    }
}


CONFIG(debug, debug|release) {
    Configuration = debug
}

CONFIG(release, debug|release){
#    DEFINES += QT_NO_DEBUG_OUTPUT
}

equals(TEMPLATE, lib){
DESTDIR      = $$PWD/lib/qt$$QT_VERSION/$$Platform-$$Compiler/$$Configuration/
} else {
DESTDIR      = $$PWD/bin/qt$$QT_VERSION/$$Platform-$$Compiler/$$Configuration/
}

OBJECTS_DIR  = $$PWD/obj/qt$$QT_VERSION/$$Platform-$$Compiler/$$Configuration/$$TARGET

MOC_DIR      = $$PWD/moc/qt$$QT_VERSION/$$Platform-$$Compiler/$$Configuration/$$TARGET
