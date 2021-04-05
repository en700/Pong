#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
sf::Music effect;
void movingBallY( sf::CircleShape& ball,float xspeed, float& yspeed, int height) {
    ball.move(xspeed,yspeed);
    if (ball.getPosition().y > height-15 || ball.getPosition().y<95) {
        yspeed *=-1;

        effect.setPitch(0.5f);
        effect.openFromFile("paddle.wav");
        effect.play();
    }

}
void movingBallX(sf::CircleShape& ball,int& scoreLeft, int& scoreRight, int width, sf::Text& scoreTextL, sf::Text& scoreTextR) {

    if (ball.getPosition().x > width+15) {
        effect.setPitch(1.2f);
        effect.openFromFile("paddle.wav");
        effect.play();
        ball.move(-width+30,0);
        scoreLeft++;
        scoreTextL.setString(std::to_string(scoreLeft));
    }
    if (ball.getPosition().x < -15) {

        effect.setPitch(1.2f);
        effect.openFromFile("paddle.wav");
        effect.play();
        ball.move(width-30,0);
        scoreRight++;
        scoreTextR.setString(std::to_string(scoreRight));
    }
}

void ballColor(sf::CircleShape& ball, int paddle ) {
    if(paddle == 1) {
        ball.setFillColor(sf::Color::Red);
    } else {
        ball.setFillColor(sf::Color::Blue);
    }
}

void paddleDown(int lr, sf::RectangleShape& paddle, int height ) {
    if(lr == 1) {
        if((sf::Keyboard::isKeyPressed(sf::Keyboard::S))&& paddle.getPosition().y<height - 100) {
            paddle.move(0, .06);
        }
    } else if(lr == 2) {
        if((sf::Keyboard::isKeyPressed(sf::Keyboard::Down))&& paddle.getPosition().y< height - 100) {
            paddle.move(0, .06);
        }
    }
}
void paddleUp(int lr, sf::RectangleShape& paddle ) {
    if(lr == 1) {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && paddle.getPosition().y>180) {

            paddle.move(0, -.06);
        }
    } else if(lr == 2) {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && paddle.getPosition().y>180) {

            paddle.move(0, -.06);
        }
    }
}

int main() {
    int width{1200}, height{850}, scoreRight{0}, scoreLeft{0};
    float xspeed{.1}, yspeed{.1};
    int gameState = 0;

    // Create the main window
    sf::RenderWindow app(sf::VideoMode(width, height), "Mega Pong");

    // Load font
    sf::Font myFont;
    myFont.loadFromFile("arial.ttf");

    //Loading Screen text
    sf::Text loadingScreen;
    loadingScreen.setFont(myFont);
    loadingScreen.setString("Welcome to MEGA PONG \n PRESS [ENTER] TO PLAY");
    loadingScreen.setOrigin(loadingScreen.getLocalBounds().width/2,loadingScreen.getLocalBounds().height/2);
    loadingScreen.setPosition(width/2,height/2);
    loadingScreen.setColor(sf::Color::White);

    //score text for left
    sf::Text scoreTextL;
    scoreTextL.setFont(myFont);
    scoreTextL.setString(std::to_string(scoreLeft));
    scoreTextL.setOrigin(scoreTextL.getLocalBounds().width/2,scoreTextL.getLocalBounds().height/2);
    scoreTextL.setPosition(40, 20);
    scoreTextL.setColor(sf::Color::Black);

    //score text right
    sf::Text scoreTextR;
    scoreTextR.setFont(myFont);
    scoreTextR.setString(std::to_string(scoreRight));
    scoreTextR.setOrigin(scoreTextL.getLocalBounds().width/2,scoreTextL.getLocalBounds().height/2);
    scoreTextR.setPosition(width-40, 20);
    scoreTextR.setColor(sf::Color::Black);

    // Load a game shapes to display
    sf::CircleShape ball(15);
    ball.setOrigin(15, 15);
    ball.setPosition(width/2, height/2);
    ball.setFillColor(sf::Color::Red);

    //right paddle
    sf::RectangleShape paddleRight(sf::Vector2f(30, 200));
    paddleRight.setOrigin(15/2, 200/2);
    paddleRight.setPosition(width-15,height/2);
    paddleRight.setFillColor(sf::Color::Blue);

    //left paddle
    sf::RectangleShape paddleLeft(sf::Vector2f(30,200));
    paddleLeft.setOrigin(15/2,200/2);
    paddleLeft.setPosition(1,height/2);
    paddleLeft.setFillColor(sf::Color::Red);

    //score board barrier
    sf::RectangleShape barrier(sf::Vector2f(1200,80));
    barrier.setOrigin(width/2,height/2);
    barrier.setPosition(width/2,height/2);
    barrier.setFillColor(sf::Color::White);

    //score board title
    sf::Text title;
    title.setFont(myFont);
    title.setString("MEGA PONG");
    title.setOrigin(title.getLocalBounds().width/2,title.getLocalBounds().height/2);
    title.setPosition(width/2,25);
    title.setColor(sf::Color::Black);

    //end card
    sf::Text endCardLeft;
    endCardLeft.setFont(myFont);
    endCardLeft.setString("The Left Player has won!\n[PRESS ENTER TO PLAY AGAIN]");
    endCardLeft.setOrigin(endCardLeft.getLocalBounds().width/2,endCardLeft.getLocalBounds().height/2);
    endCardLeft.setPosition(width/2, height/2);
    endCardLeft.setColor(sf::Color::White);

    sf::Text endCardRight;
    endCardRight.setFont(myFont);
    endCardRight.setString("The Right Player has won!\n[PRESS ENTER TO PLAY AGAIN]");
    endCardRight.setOrigin(endCardRight.getLocalBounds().width/2,endCardRight.getLocalBounds().height/2);
    endCardRight.setPosition(width/2, height/2);
    endCardRight.setColor(sf::Color::White);

    //load sounds
    //paddle
    sf::SoundBuffer paddle;
    if(!paddle.loadFromFile("Paddle.wav")) {
        return -1;
    }
    sf::Sound paddleSound;
    paddleSound.setBuffer(paddle);

    sf::Sound wall;
    wall.setBuffer(paddle);
    wall.setPitch(0.5);

    //point
    sf::SoundBuffer blare;
    if(!blare.loadFromFile("blare.wav")) {
        return -1;
    }
    sf::Sound blareSound;
    blareSound.setBuffer(blare);


    sf::Music gameMusic;
    gameMusic.openFromFile("GameMusic.wav");
    gameMusic.play();
    gameMusic.setVolume(.75);
    gameMusic.setLoop(true);



    // Start the game loop
    while (app.isOpen()) {

        sf::Event event;
        while (app.pollEvent(event)) {
            // Close window : exit
            if (event.type == sf::Event::Closed) {
                app.close();
            }

        }


        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && (gameState == 0 || gameState == 2)) {
            gameState = 1;
            ball.setPosition(width/2, height/2);
            scoreLeft = 0;
            scoreRight = 0;
            ballColor(ball, 1);
            scoreTextR.setString(std::to_string(scoreRight));
            scoreTextL.setString(std::to_string(scoreLeft));
            paddleRight.setPosition(width-15,height/2);
            paddleLeft.setPosition(1,height/2);

        }

        if(scoreLeft == 10 || scoreRight == 10) {
            gameState = 2;
        }


        // Process close window event




        //game logic

        if(gameState == 1) {
            movingBallY(ball,xspeed,yspeed, height);
            movingBallX(ball,scoreLeft,scoreRight,width,scoreTextL,scoreTextR);

            //Keyboard Controls
            //Right
            paddleDown(2,paddleRight,height);
            paddleUp(2,paddleRight);

            //Left
            paddleDown(1,paddleLeft,height);
            paddleUp(1,paddleLeft);
        }






        //colision detection
        //Right
        if(ball.getGlobalBounds().intersects(paddleRight.getGlobalBounds())) {

            float dist = ball.getPosition().y-paddleRight.getPosition().y;

            yspeed = dist/115.0 * .1;

            xspeed = sqrt(.14*.14 - xspeed*yspeed)*-1;

            paddleSound.play();

            ballColor(ball, 2);

            // changeColor(ball);

        }

        //Left
        if(ball.getGlobalBounds().intersects(paddleLeft.getGlobalBounds())) {

            float dist = ball.getPosition().y-paddleLeft.getPosition().y;

            yspeed = dist/115.0 * .1;

            xspeed = sqrt(.14*.14 - xspeed*yspeed)*1;

            paddleSound.play();

            ballColor(ball, 1);

            // changeColor(ball);

        }

        // Clear screen
        app.clear();
        if(gameState == 0) {
            app.draw(loadingScreen);
        }
        if(gameState == 1) {


            // Draw the sprite
            app.draw(barrier);
            app.draw(ball);
            app.draw(paddleRight);
            app.draw(paddleLeft);
            app.draw(scoreTextL);
            app.draw(scoreTextR);
            app.draw(title);
        }
        if(gameState == 2) {
            if(scoreRight == 10) {
                app.draw(endCardRight);
            } else if(scoreLeft == 10) {
                app.draw(endCardLeft);
            }


        }



        // Update the window
        app.display();
    }

    return EXIT_SUCCESS;
}

