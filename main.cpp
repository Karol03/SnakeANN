#include <QCoreApplication>
#include "Engine/game.hpp"
#include "utilities/generator.hpp"
#include "utilities/logger.hpp"

#include "NeuralNetwork/neuralnetwork.hpp"

int main(int argc, char *argv[])
{
  //  logger::Logging log(logger::Logging::Error_only);
    QCoreApplication a(argc, argv);
    Generator::initialize();
    Game game;
    game.run();
    return a.exec();
}
