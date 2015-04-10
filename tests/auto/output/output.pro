CONFIG += testcase link_pkgconfig
TARGET = tst_output

QT += testlib
QT += core-private gui-private compositor compositor-private

INCLUDEPATH += ../compositor

!contains(QT_CONFIG, no-pkg-config) {
    PKGCONFIG += wayland-client wayland-server
} else {
    LIBS += -lwayland-client -lwayland-server
}

SOURCES += tst_output.cpp \
           ../compositor/testcompositor.cpp
