#include <SFML/Graphics.hpp>
#include <iostream>

bool gameOver = false;
const int fruitNum = 10;
int FruitX[fruitNum];
int FruitY[fruitNum];
int winX = 1000;
int winY = 800;

class Player
{
public:
    enum eDirection
    {
        STOP,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };
    eDirection dir = STOP;
    int score = 0;
    float speed = 0.1;
    float x;
    float y;
    float spriteW;
    float spriteH;
    float TailX[100];
    float TailY[100];
    float points[4][2] = {
        {x, y},
        {x + spriteW, y},
        {x, y + spriteH},
        {x + spriteW, y + spriteH}};
    Player(int xa, int ya, int xs, int ys)
    {
        x = xa;
        y = ya;
        spriteW = xs;
        spriteH = xs;
        findSpriteCenter();
    }
    void findSpriteCenter()
    {
        x -= spriteW / 2;
        y -= spriteH / 2;

        // std::cout << spriteH << "/" << spriteW << std::endl;
        // std::cout << x << "/" << y << std::endl;
    }

    void move()
    {
        switch (dir)
        {
        case RIGHT:
            x += speed;
            break;
        case LEFT:
            x -= speed;
            break;
        case UP:
            y -= speed;
            break;
        case DOWN:
            y += speed;
            break;
        default:
            break;
        }

        points[0][0] = x;
        points[0][1] = y;
        points[1][0] = x + spriteW;
        points[1][1] = y;
        points[2][0] = x;
        points[2][1] = y + spriteH;
        points[3][0] = x + spriteW;
        points[3][1] = y + spriteH;
    }

    bool outoFBound()
    {
        if (x + (spriteW) > winX)
        {
            x = winX - spriteW;
            return true;
        }
        else if (x < 0)
        {
            x = 0;
            return true;
        }
        else if (y + (spriteH) > winY)
        {
            y = winY - spriteH;
            return true;
        }
        else if (y < 0)
        {
            y = 0;
            return true;
        }
        else
        {
            return false;
        }
    }

    void eatFruit()
    {

        for (int i = 0; i < fruitNum; i++)
        {
            for (int k = 0; k < 4; k++)
            {
                if (points[k][0] >= FruitX[i] && points[k][0] <= FruitX[i] + 25)
                {
                    if (points[k][1] >= FruitY[i] && points[k][1] <= FruitY[i] + 25)
                    {
                        score++;
                        FruitX[i] = rand() % (winX - 25);
                        FruitY[i] = rand() % (winY - 25);
                    }
                }
            }
        }
    }
    void calculateTails()
    {
        std::cout << "Hello World";
    }
};

int main()
{
    int winCenterX = winX / 2;
    int winCenterY = winY / 2;
    sf::RenderWindow window(sf::VideoMode(winX, winY), "Game Window");

    // import font family
    sf::Font font;
    font.loadFromFile("./fonts/IBMPlexSansThaiLooped-Bold.ttf");

    // setup character
    sf::RectangleShape square(sf::Vector2f(25, 25));
    int spriteW = square.getSize().x;
    int spriteH = square.getSize().y;
    Player player = Player(winCenterX, winCenterY, spriteW, spriteH);

    // setup score text
    sf::Text score;
    score.setFont(font);
    score.setCharacterSize(50);
    score.setFillColor(sf::Color::White);
    score.setPosition(sf::Vector2f(25, 25));

    // setup gameover text

    sf::Text lost;
    lost.setFont(font);
    lost.setString("You Lost!");
    lost.setCharacterSize(50);
    lost.setFillColor(sf::Color::Red);
    lost.setPosition(sf::Vector2f(winCenterX - 25, winCenterY));

    // setup first fruit positions
    for (int i = 0; i < fruitNum; i++)
    {
        // std::cout << "Generating fruit number " << i + 1 << std::endl;
        FruitX[i] = rand() % (winX - 25);
        FruitY[i] = rand() % (winY - 25);
    }

    while (window.isOpen())
    {
        sf::Event event;
        if (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && !gameOver)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::D:
                    player.dir = player.RIGHT;
                    break;
                case sf::Keyboard::A:
                    player.dir = player.LEFT;
                    break;
                case sf::Keyboard::W:
                    player.dir = player.UP;
                    break;
                case sf::Keyboard::S:
                    player.dir = player.DOWN;
                    break;
                }
            }
        }
        player.move();
        player.eatFruit();
        player.calculateTails();

        gameOver = player.outoFBound();
        square.setPosition(sf::Vector2f(player.x, player.y));
        std::string s = std::to_string(player.score);
        score.setString(s);

        window.clear();

        for (int i = 0; i < fruitNum; i++)
        {
            // std::cout << std::endl
            sf::RectangleShape fruit(sf::Vector2f(25, 25));
            fruit.setPosition(sf::Vector2f(FruitX[i], FruitY[i]));
            fruit.setFillColor(sf::Color::Red);
            window.draw(fruit);
        }
        for (int i = 0; i < player.score; i++)
        {
            sf::RectangleShape tail(sf::Vector2f(25, 25));
            tail.setPosition(sf::Vector2f(player.TailX[i], player.TailY[i]));
            tail.setFillColor(sf::Color::White);
            window.draw(tail);
        }

        // Draw the stuff before displaying here
        window.draw(square);
        window.draw(score);

        if (gameOver)
        {
            // Initialize the text
            window.draw(lost);
            window.display();
        }
        else
        {
            window.display();
        }
    }
}