#include <QCoreApplication>
#include "Engine/game.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Game game;
    game.run();
    return a.exec();
}
