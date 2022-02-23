QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES+= QT_MESSAGELOGCONTEXT

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# The application icon
RC_ICONS = rc/icon.ico

SOURCES += \
        BuildComponentBase.cpp \
        BuildComponents/Cinematic.cpp \
        BuildComponents/Cinematics.cpp \
        BuildComponents/Coordinates.cpp \
        BuildComponents/DelayedCommands.cpp \
        BuildComponents/Quest.cpp \
        BuildComponents/Quests.cpp \
        BuildComponents/Texts.cpp \
        BuildComponents/Ticks.cpp \
        BuildComponents/Variables.cpp \
        CommandParser.cpp \
        Helpers/CommandsHelper.cpp \
        Helpers/FileHelper.cpp \
        Helpers/JsonHelper.cpp \
        McTechnical.cpp \
        Minecraft/BlockCoordinates.cpp \
        Minecraft/CommandBlocksArea.cpp \
        Minecraft/McFunction.cpp \
        Minecraft/QtNbt.cpp \
        Project.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    BuildComponentBase.h \
    BuildComponents/Cinematic.h \
    BuildComponents/Cinematics.h \
    BuildComponents/Coordinates.h \
    BuildComponents/DelayedCommands.h \
    BuildComponents/Quest.h \
    BuildComponents/Quests.h \
    BuildComponents/Texts.h \
    BuildComponents/Ticks.h \
    BuildComponents/Variables.h \
    CommandParser.h \
    Helpers/CommandsHelper.h \
    Helpers/FileHelper.h \
    Helpers/JsonHelper.h \
    McTechnical.h \
    Minecraft/BlockCoordinates.h \
    Minecraft/CommandBlocksArea.h \
    Minecraft/McFunction.h \
    Minecraft/QtNbt.h \
    Project.h

RESOURCES += \
    rc/rc.qrc
