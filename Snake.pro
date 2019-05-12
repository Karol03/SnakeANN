QT -= gui

CONFIG += c++14 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Engine/stage.cpp \
    main.cpp \
    NeuralNetwork/member.cpp \
    NeuralNetwork/neuralnetwork.cpp \
    NeuralNetwork/neuralnetworkhandler.cpp \
    NeuralNetwork/population.cpp \
    utilities/generator.cpp \
    utilities/logger.cpp \
    Engine/feed.cpp \
    Engine/snake.cpp \
    utilities/filehandler.cpp \
    Engine/drawing.cpp \
    Engine/control.cpp \
    Engine/validation.cpp \
    Engine/game.cpp \
    Engine/controller.cpp \
    Engine/drawer.cpp \
    Engine/control.cpp \
    Engine/controller.cpp \
    Engine/drawer.cpp \
    Engine/drawing.cpp \
    Engine/feed.cpp \
    Engine/game.cpp \
    Engine/snake.cpp \
    Engine/stage.cpp \
    Engine/validation.cpp \
    NeuralNetwork/member.cpp \
    NeuralNetwork/neuralnetwork.cpp \
    NeuralNetwork/neuralnetworkhandler.cpp \
    NeuralNetwork/population.cpp \
    utilities/filehandler.cpp \
    utilities/generator.cpp \
    utilities/logger.cpp \
    main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += C:\SFML-2.5.1\include
DEPENDPATH += C:\SFML-2.5.1\include

LIBS += -LC:\SFML-2.5.1\lib\

CONFIG(debug, debug|relase): LIBS += -lsfml-audio-d -lsfml-system-d -lsfml-network-d -lsfml-main-d -lsfml-window-d -lsfml-graphics-d
CONFIG(debug, debug|relase): LIBS += -lsfml-audio -lsfml-system -lsfml-network -lsfml-main -lsfml-window -lsfml-graphics

HEADERS += \
    Engine/stage.h \
    NeuralNetwork/member.hpp \
    NeuralNetwork/neuralnetwork.hpp \
    NeuralNetwork/neuralnetworkhandler.hpp \
    NeuralNetwork/population.hpp \
    utilities/generator.hpp \
    utilities/logger.hpp \
    Engine/direction.h \
    Engine/feed.h \
    Engine/object.h \
    Engine/objectype.h \
    Engine/snake.h \
    utilities/filehandler.hpp \
    Engine/state.hpp \
    Engine/drawing.hpp \
    Engine/control.hpp \
    Engine/validation.hpp \
    Engine/game.hpp \
    Engine/controller.hpp \
    Engine/drawer.hpp \
    Engine/control.hpp \
    Engine/controller.hpp \
    Engine/direction.h \
    Engine/drawer.hpp \
    Engine/drawing.hpp \
    Engine/feed.h \
    Engine/game.hpp \
    Engine/object.h \
    Engine/objectype.h \
    Engine/snake.h \
    Engine/stage.h \
    Engine/state.hpp \
    Engine/validation.hpp \
    NeuralNetwork/member.hpp \
    NeuralNetwork/neuralnetwork.hpp \
    NeuralNetwork/neuralnetworkhandler.hpp \
    NeuralNetwork/population.hpp \
    utilities/filehandler.hpp \
    utilities/generator.hpp \
    utilities/logger.hpp

DISTFILES += \
    graphics/feed.png \
    graphics/head.png \
    graphics/tail_edge.png \
    graphics/tail_end.png \
    graphics/tail_straight.png
