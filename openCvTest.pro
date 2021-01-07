QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FqImageMat.cpp \
    FqImageWidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    FqImageMat.hpp \
    FqImageWidget.hpp \
    mainwindow.hpp

FORMS += \
    FqImageWidget.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -LD:/Apps/DevelopmentLib/openCv3/opencv/build/x64/vc14/lib/ -lopencv_world3413
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/Apps/DevelopmentLib/openCv3/opencv/build/x64/vc14/lib/ -lopencv_world3413d
else:unix: LIBS += -LD:/Apps/DevelopmentLib/openCv3/opencv/build/x64/vc14/lib/ -lopencv_world3413

INCLUDEPATH += D:/Apps/DevelopmentLib/openCv3/opencv/build/include
DEPENDPATH += D:/Apps/DevelopmentLib/openCv3/opencv/build/include

