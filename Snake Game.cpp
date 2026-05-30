#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int GRID_SIZE = 20;

class Snake {
private:
    std::vector<sf::RectangleShape> body;
    sf::Vector2f direction;

public:
    Snake() {
        sf::RectangleShape head(sf::Vector2f(GRID_SIZE, GRID_SIZE));
        head.setFillColor(sf::Color::Green);
        head.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        body.push_back(head);

        direction = sf::Vector2f(GRID_SIZE, 0); // start moving right
    }

    void move() {
        for (int i = body.size() - 1; i > 0; --i)
            body[i].setPosition(body[i - 1].getPosition());

        body[0].move(direction);
    }

    void grow() {
        sf::RectangleShape newSegment(sf::Vector2f(GRID_SIZE, GRID_SIZE));
        newSegment.setFillColor(sf::Color::Green);
        newSegment.setPosition(body.back().getPosition());
        body.push_back(newSegment);
    }

    void setDirection(sf::Vector2f newDir) {
        if (body.size() > 1 && body[0].getPosition() + newDir == body[1].getPosition())
            return;
        direction = newDir;
    }

    sf::Vector2f getDirection() {
        return direction;
    }

    sf::Vector2f getHeadPosition() {
        return body[0].getPosition();
    }

    bool checkSelfCollision() {
        for (int i = 1; i < body.size(); ++i) {
            if (body[0].getPosition() == body[i].getPosition())
                return true;
        }
        return false;
    }

    void draw(sf::RenderWindow& window) {
        for (auto& segment : body)
            window.draw(segment);
    }

    int getLength() {
        return body.size();
    }
};

class Food {
private:
    sf::RectangleShape shape;

public:
    Food() {
        shape.setSize(sf::Vector2f(GRID_SIZE, GRID_SIZE));
        shape.setFillColor(sf::Color::Red);
        respawn();
    }

    void respawn() {
        int maxX = WINDOW_WIDTH / GRID_SIZE;
        int maxY = WINDOW_HEIGHT / GRID_SIZE;
        int x = std::rand() % maxX;
        int y = std::rand() % maxY;
        shape.setPosition(x * GRID_SIZE, y * GRID_SIZE);
    }

    sf::Vector2f getPosition() {
        return shape.getPosition();
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake Game - OOP");
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    Snake snake;
    Food food;

    sf::Clock clock;
    float moveDelay = 0.2f;

    // --- Game Over Text ---
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font. Make sure 'arial.ttf' is in the same folder.\n";
        return -1;
    }

    sf::Text gameOverText("Game Over!", font, 50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 25);

    bool gameOver = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (gameOver) {
            window.clear();
            window.draw(gameOverText);
            window.display();
            continue;
        }

        // Controls
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && snake.getDirection().y == 0)
            snake.setDirection(sf::Vector2f(0, -GRID_SIZE));
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && snake.getDirection().y == 0)
            snake.setDirection(sf::Vector2f(0, GRID_SIZE));
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && snake.getDirection().x == 0)
            snake.setDirection(sf::Vector2f(-GRID_SIZE, 0));
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && snake.getDirection().x == 0)
            snake.setDirection(sf::Vector2f(GRID_SIZE, 0));

        if (clock.getElapsedTime().asSeconds() > moveDelay) {
            snake.move();
            clock.restart();

            sf::Vector2f pos = snake.getHeadPosition();
            if (pos.x < 0 || pos.x >= WINDOW_WIDTH || pos.y < 0 || pos.y >= WINDOW_HEIGHT)
                gameOver = true;

            if (snake.checkSelfCollision())
                gameOver = true;

            if (snake.getHeadPosition() == food.getPosition()) {
                snake.grow();
                food.respawn();
            }
        }

        window.clear(sf::Color::Black);
        food.draw(window);
        snake.draw(window);
        window.display();
    }

    return 0;
}
