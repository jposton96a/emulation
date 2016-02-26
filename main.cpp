#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "core/chip8.h"

using namespace std;

void setKeys(chip8 chip)
{
    byte keyStates[16];
    keyStates[1] = sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) ? 1 : 0;
    keyStates[2] = sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) ? 1 : 0;
    keyStates[3] = sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) ? 1 : 0;
    keyStates[0xC] = sf::Keyboard::isKeyPressed(sf::Keyboard::Num4) ? 1 : 0;
    keyStates[4] = sf::Keyboard::isKeyPressed(sf::Keyboard::Q) ? 1 : 0;
    keyStates[5] = sf::Keyboard::isKeyPressed(sf::Keyboard::W) ? 1 : 0;
    keyStates[6] = sf::Keyboard::isKeyPressed(sf::Keyboard::E) ? 1 : 0;
    keyStates[0xD] = sf::Keyboard::isKeyPressed(sf::Keyboard::R) ? 1 : 0;
    keyStates[7] = sf::Keyboard::isKeyPressed(sf::Keyboard::A) ? 1 : 0;
    keyStates[8] = sf::Keyboard::isKeyPressed(sf::Keyboard::S) ? 1 : 0;
    keyStates[9] = sf::Keyboard::isKeyPressed(sf::Keyboard::D) ? 1 : 0;
    keyStates[0xE] = sf::Keyboard::isKeyPressed(sf::Keyboard::F) ? 1 : 0;
    keyStates[0xA] = sf::Keyboard::isKeyPressed(sf::Keyboard::Z) ? 1 : 0;
    keyStates[0] = sf::Keyboard::isKeyPressed(sf::Keyboard::X) ? 1 : 0;
    keyStates[0xB] = sf::Keyboard::isKeyPressed(sf::Keyboard::C) ? 1 : 0;
    keyStates[0xF] = sf::Keyboard::isKeyPressed(sf::Keyboard::V) ? 1 : 0;
    chip.setKeys(keyStates);
    while(sf::Keyboard::isKeyPressed(sf::Keyboard::P)){
        int x = 0;
    }
}

int main(int argc, char **argv)
{
    sf::RenderWindow window(sf::VideoMode(640, 320), "Chip8 Emulator");
    chip8 myChip8;

    // Initialize the Chip8 system and load the game into the memory
    myChip8.initialize();
    myChip8.loadGame("res/roms/PONG2");

    sf::Clock clock;
    clock.restart();
    const sf::Time FRAMETIME = sf::microseconds(1000000.f / 60);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // The clock cycle for the CPU needs to be 60Hz
        sf::Time timeElapsed = clock.restart();
        if(timeElapsed > FRAMETIME)
            sf::sleep(FRAMETIME - timeElapsed);

        // Emulate one cycle
        myChip8.emulateCycle();

        // If the draw flag is set, update the screen
        if(myChip8.drawFlag) {
            window.clear();

            unsigned char* gfx = myChip8.getGraphics();

            sf::Vector2f pixelSize(10, 10);
            sf::RectangleShape pixel;
            pixel.setSize(pixelSize);

            for(int x = 0; x < 64; x++){
                for(int y = 0; y < 32; y++){
                    pixel.setFillColor(gfx[x + (y * 64)] == 1 ? sf::Color::White : sf::Color::Black);
                    pixel.setPosition(pixelSize.x * x, pixelSize.y * y);
                    window.draw(pixel);
                }
            }

            window.display();

            myChip8.drawFlag = false;
        }

        // Store key press state (Press and Release)
        setKeys(myChip8);
    }

    return 0;
}