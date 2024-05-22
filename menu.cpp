#include <SFML/Graphics.hpp>
#include <iostream>

enum Difficulty { EASY = 0, MEDIUM, HARD };

class Menu {
public:
    Menu(float width, float height);
    void draw(sf::RenderWindow &window);
    int handleClick(sf::Vector2i mousePos);

private:
    sf::Font font;
    sf::Text menu[3];
    sf::RectangleShape borders[3];
    const std::string labels[3] = { "Facile", "Moyen", "Difficile" };

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
};

Menu::Menu(float width, float height) {
    if (!font.loadFromFile("arial.ttf")) {
        // Handle error
        std::cerr << "Error loading font\n";
    }

    if (!backgroundTexture.loadFromFile("menufond.jpg")) {
        // Handle error
        std::cerr << "Error loading background image\n";
    }

    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        width / backgroundSprite.getGlobalBounds().width,
        height / backgroundSprite.getGlobalBounds().height
    );

    for (int i = 0; i < 3; i++) {
        menu[i].setFont(font);
        menu[i].setFillColor(sf::Color::White);
        menu[i].setString(labels[i]);
        menu[i].setPosition(sf::Vector2f(width / 2 - menu[i].getGlobalBounds().width / 2, height / (3 + 1) * (i + 1)));

        // Set up the borders
        borders[i].setSize(sf::Vector2f(menu[i].getGlobalBounds().width + 20, menu[i].getGlobalBounds().height + 20));
        borders[i].setOutlineColor(sf::Color::White);
        borders[i].setOutlineThickness(2);
        borders[i].setFillColor(sf::Color::Transparent);
        borders[i].setPosition(menu[i].getPosition().x - 10, menu[i].getPosition().y - 10);
    }
}

void Menu::draw(sf::RenderWindow &window) {
    window.draw(backgroundSprite);
    for (int i = 0; i < 3; i++) {
        window.draw(borders[i]);
        window.draw(menu[i]);
    }
}

int Menu::handleClick(sf::Vector2i mousePos) {
    for (int i = 0; i < 3; i++) {
        if (borders[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            return i;
        }
    }
    return -1; // No button was clicked
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Menu SFML");
    Menu menu(window.getSize().x, window.getSize().y);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int result = menu.handleClick(sf::Mouse::getPosition(window));
                if (result != -1) {
                    std::cout << "Difficulty selected: " << result << std::endl;
                    window.close(); // Close the window after selection
                }
            }
        }

        window.clear();
        menu.draw(window);
        window.display();
    }

    return 0;
}
