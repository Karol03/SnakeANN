#include <QCoreApplication>
#include "Engine/game.hpp"
#include "utilities/generator.hpp"
#include "utilities/logger.hpp"


using namespace std;

int main(int argc, char *argv[])
{
    logger::Logging log(logger::Logging::Info);
    QCoreApplication a(argc, argv);
    Generator::initialize();
    Game game;
    game.run();
    return a.exec();
}
