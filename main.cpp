#include <QCoreApplication>

#include <SFML/Graphics.hpp>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    sf::Window window(sf::VideoMode(800, 600), "My window");
    return a.exec();
}
