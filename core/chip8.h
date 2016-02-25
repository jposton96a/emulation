//
// Created by jposton on 2/17/16.
//

#ifndef CHIP8_EMU_CHIP8_H
#define CHIP8_EMU_CHIP8_H

#define byte unsigned char

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

const int REGISTER_SIZE = 16;
const int STACK_SIZE = 16;
const int DISPLAY_HEIGHT = 32;
const int DISPLAY_WIDTH = 64;
const int MEMORY_SIZE = 4096;
const int FONTSET_SIZE = 80;
const unsigned char fontset[FONTSET_SIZE] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,		// 0
        0x20, 0x60, 0x20, 0x20, 0x70,		// 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0,		// 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0,		// 3
        0x90, 0x90, 0xF0, 0x10, 0x10,		// 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0,		// 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0,		// 6
        0xF0, 0x10, 0x20, 0x40, 0x40,		// 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0,		// 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0,		// 9
        0xF0, 0x90, 0xF0, 0x90, 0x90,		// A
        0xE0, 0x90, 0xE0, 0x90, 0xE0,		// B
        0xF0, 0x80, 0x80, 0x80, 0xF0,		// C
        0xE0, 0x90, 0x90, 0x90, 0xE0,		// D
        0xF0, 0x80, 0xF0, 0x80, 0xF0,		// E
        0xF0, 0x80, 0xF0, 0x80, 0x80		// F
};

class chip8 {
    unsigned short opcode;

    unsigned char memory[MEMORY_SIZE]; // CPU Memory
    unsigned char V[REGISTER_SIZE]; // CPU Registers
    unsigned short stack[STACK_SIZE]; // CPU stack

    unsigned short mem_index;
    unsigned short program_counter;
    unsigned short stack_pointer;

    unsigned char gfx[DISPLAY_WIDTH * DISPLAY_HEIGHT]; // 64x32px black & white screen
    unsigned char delay_timer;
    unsigned char sound_timer;

    unsigned char key[16];

public:
    void initialize();
    void loadGame(std::string);
    void emulateCycle();
    void setKeys(byte*);
    bool drawFlag = true;

    unsigned char* getGraphics();

private:
    void clearDisplay();
    void clearRegisters();
    void clearStack();
    void clearMemory();
    void decodeOpcode(unsigned short);

    void x0NNN(int);
    void x00E0();
    void x00EE();

    void x1NNN(int);
    void x2NNN(int);
    void x3XNN(unsigned char, short);
    void x4XNN(unsigned char, short);
    void x5XY0(unsigned char, unsigned char);
    void x6XNN(unsigned char, short);
    void x7XNN(unsigned char, short);

    void x8XY0(unsigned char, unsigned char);
    void x8XY1(unsigned char, unsigned char);
    void x8XY2(unsigned char, unsigned char);
    void x8XY3(unsigned char, unsigned char);
    void x8XY4(unsigned char, unsigned char);
    void x8XY5(unsigned char, unsigned char);
    void x8XY6(unsigned char, unsigned char);
    void x8XY7(unsigned char, unsigned char);
    void x8XYE(unsigned char, unsigned char);

    void x9XY0(unsigned char, unsigned char);

    void xANNN(int);
    void xBNNN(int);
    void xCXNN(unsigned char, short);
    void xDXYN(unsigned char, unsigned char, unsigned char);

    void xEX9E(unsigned char);
    void xEXA1(unsigned char);

    void xFX07(unsigned char);
    void xFX0A(unsigned char);
    void xFX15(unsigned char);
    void xFX18(unsigned char);
    void xFX1E(unsigned char);
    void xFX29(unsigned char);
    void xFX33(unsigned char);
    void xFX55(unsigned char);
    void xFX65(unsigned char);

};


#endif //CHIP8_EMU_CHIP8_H
