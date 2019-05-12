#include <QCoreApplication>
#include <ctime>
#include "Engine/game.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
   // srand( time(NULL) );
    Game game;
    game.run();
    return a.exec();
}
