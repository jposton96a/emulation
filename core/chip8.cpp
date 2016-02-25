//
// Created by jposton on 2/17/16.
//

#include <sstream>
#include "chip8.h"

void chip8::initialize() {
    program_counter = 0x200;
    opcode = 0;
    mem_index = 0;
    stack_pointer = 0;

    clearStack();
    clearRegisters();
    clearDisplay();
    clearMemory();

    for(int i = 0; i < 16; ++i)
        key[i] = 0;


    // Load fontset
    for(int i = 0; i < 80; ++i)
        memory[i] = fontset[i];

    delay_timer = 0;
    sound_timer = 0;

    srand (time(NULL));
}

unsigned char* chip8::getGraphics() {
    return gfx;
}

void chip8::setKeys(byte* keyStates){
    for(int i = 0; i < 16; i++)
        key[i] = keyStates[i];

    for(int i = 0; i < 16; i++)
        if(key[i] == 1)
            printf("Key Pressed: %d\n", i);
}

void chip8::loadGame(string gamePath) {
    ifstream gameFile;
    gameFile.open (gamePath, ios::binary);

    char* memblock;
    if(gameFile.is_open()){
        gameFile.seekg (0, ios::end);
        int size = gameFile.tellg();
        memblock = new char [size];

        gameFile.seekg (0, ios::beg);
        gameFile.read (memblock, size);
        gameFile.close();

        for(int i = 0; i < size; ++i)
            memory[i + 512] = memblock[i]; // Program is loaded in memory at location 0x200 == 512

        gameFile.close();
        delete[] memblock;
    }
    else cout << "Unable to open file";
}

void chip8::emulateCycle() {

    // Fetch opcode
    opcode = memory[program_counter] << 8 | memory[program_counter + 1];

    decodeOpcode(opcode);

    // Update timers
    if(delay_timer > 0)
        --delay_timer;

    if(sound_timer > 0)
    {
        if(sound_timer == 1)
            printf("BEEP!\n");
        --sound_timer;
    }

    if(false);
}

void chip8::decodeOpcode(unsigned short opcode){
    std::stringstream stream;
    stream << std::hex << opcode;
    string opcodeStr = stream.str();

    // Get opcode parameters
    int X = (opcode & 0x0F00) >> 8;
    int Y = (opcode & 0x00F0) >> 4;
    unsigned char N = opcode & 0x000F;
    short NN = opcode & 0x00FF;
    int NNN = opcode & 0x0FFF;

    // Execute opcode
    switch(opcode & 0xF000)
    {
        case 0x0000:
            switch(opcode & 0x000F)
            {
                case 0x0000: x00E0(); break;
                case 0x000E: x00EE(); break;
                default:
                    printf ("Unknown opcode: 0x%X\n", opcode);
            }
            break;

        case 0x1000: x1NNN(NNN); break;
        case 0x2000: x2NNN(NNN); break;
        case 0x3000: x3XNN(X, NN); break;
        case 0x4000: x4XNN(X, NN); break;
        case 0x5000: x5XY0(X, Y); break;
        case 0x6000: x6XNN(X, NN); break;
        case 0x7000: x7XNN(X, NN); break;

        case 0x8000:
            switch(opcode & 0x000F)
            {
                case 0x0000: x8XY0(X, Y); break;
                case 0x0001: x8XY1(X, Y); break;
                case 0x0002: x8XY2(X, Y); break;
                case 0x0003: x8XY3(X, Y); break;
                case 0x0004: x8XY4(X, Y); break;
                case 0x0005: x8XY5(X, Y); break;
                case 0x0006: x8XY6(X, Y); break;
                case 0x0007: x8XY7(X, Y); break;
                case 0x000E: x8XYE(X, Y); break;
                default:
                    printf ("Unknown opcode: 0x%X\n", opcode);
            }
            break;

        case 0x9000: x9XY0(X, Y); break;
        case 0xA000: xANNN(NNN); break;
        case 0xB000: xBNNN(NNN); break;
        case 0xC000: xCXNN(X, NN); break;
        case 0xD000: xDXYN(X, Y, N); break;

        case 0xE000:
            switch(opcode & 0x00FF)
            {
                case 0x009E: xEX9E(X); break;
                case 0x00A1: xEXA1(X); break;
                default:
                    printf ("Unknown opcode: 0x%X\n", opcode);
            }
            break;

        case 0xF000:
            switch(opcode & 0x00FF)
            {
                case 0x0007: xFX07(X); break;
                case 0x000A: xFX0A(X); break;
                case 0x0015: xFX15(X); break;
                case 0x0018: xFX18(X); break;
                case 0x001E: xFX1E(X); break;
                case 0x0029: xFX29(X); break;
                case 0x0033: xFX33(X); break;
                case 0x0055: xFX55(X); break;
                case 0x0065: xFX65(X); break;
                default:
                    printf ("Unknown opcode: 0x%X\n", opcode);
            }
            break;

        default:
            printf ("Unknown opcode: 0x%X\n", opcode);
    }
}

// Private functions

void chip8::clearStack() {
    for(int i = 0; i < 16; i++)
        stack[i] = 0;
}

void chip8::clearRegisters() {
    for(int i = 0; i < 16; i++)
        V[i] = 0;
}

void chip8::clearDisplay() {
    // Set each pixel to black
    for(int i = 0; i < 64*32; i++)
        gfx[i] = 0;
    drawFlag = true;
}

void chip8::clearMemory() {
    for(int i = 0; i < 4096; i++)
        memory[i] = 0;
}

/*************************************/
/********* Opcode Functions: *********/
/*************************************/

void chip8::x0NNN(int) { }
void chip8::x00E0() {
    clearDisplay();
    program_counter+=2;
}
void chip8::x00EE() {
    program_counter = stack[--stack_pointer] + 2;
    /*
    if(stack_pointer > 0) {
        stack_pointer--;
        if(stack[stack_pointer] == 0){
            printf("Invalid return from subroutine.\n");
            return;
        }
        program_counter = stack[stack_pointer];
        stack[stack_pointer] = 0;
    }
    else
        printf("Invalid return from subroutine.\n");
    */
}

void chip8::x1NNN(int NNN) {
    program_counter = NNN;
}
void chip8::x2NNN(int NNN) {
    stack[stack_pointer++] = program_counter;
    program_counter = NNN;
}
void chip8::x3XNN(unsigned char X, short NN) {
    if(V[X] == NN)
        program_counter+=2;
    program_counter+=2;
}
void chip8::x4XNN(unsigned char X, short NN) {
    if(V[X] != NN)
        program_counter+=2;
    program_counter+=2;
}
void chip8::x5XY0(unsigned char X, unsigned char Y) {
    if(V[X] == V[Y])
        program_counter+=2;
    program_counter+=2;
}
void chip8::x6XNN(unsigned char X, short NN) {
    V[X] = NN;
    program_counter += 2;
}
void chip8::x7XNN(unsigned char X, short NN) {
    V[X] += NN;
    program_counter += 2;
}

void chip8::x8XY0(unsigned char X, unsigned char Y) {
    V[X] = V[Y];
    program_counter += 2;
}
void chip8::x8XY1(unsigned char X, unsigned char Y) {
    V[X] = V[X] | V[Y];
    program_counter += 2;}
void chip8::x8XY2(unsigned char X, unsigned char Y) {
    V[X] = V[X] & V[Y];
    program_counter += 2;
}
void chip8::x8XY3(unsigned char X, unsigned char Y) {
    V[X] = V[X] ^ V[Y];
    program_counter += 2;
}
void chip8::x8XY4(unsigned char X, unsigned char Y) {
    if(V[Y] > (0xFF - V[X]))
        V[0xF] = 1; //carry
    else
        V[0xF] = 0;
    V[X] += V[Y];
    program_counter += 2;
}
void chip8::x8XY5(unsigned char X, unsigned char Y) {
    if(V[X] > V[Y])
        V[0xF] = 1; //carry
    else
        V[0xF] = 0;
    V[X] -= V[Y];
    program_counter += 2;}
void chip8::x8XY6(unsigned char X, unsigned char Y) {
    if (V[X] % 2 == 0)
        V[0xF] = 0;
    else
        V[0xF] = 1;
    V[X] = V[X] >> 1;
    program_counter += 2;
}
void chip8::x8XY7(unsigned char X, unsigned char Y) {
    if (V[Y] > V[X])
        V[0xF] = 0;
    else
        V[0xF] = 1;
    V[X] = V[Y] - V[X];
    program_counter += 2;
}
void chip8::x8XYE(unsigned char X, unsigned char Y) {
    if (V[X] < 128)
        V[0xF] = 0;
    else
        V[0xF] = 1;
    V[X] = V[X] << 1;
    program_counter += 2;
}

void chip8::x9XY0(unsigned char X, unsigned char Y) {
    if(V[X] != V[Y])
        program_counter+=2;
    program_counter+=2;
}

void chip8::xANNN(int NNN) {
    mem_index = NNN;
    program_counter += 2;
}
void chip8::xBNNN(int NNN) {
    program_counter = NNN + V[0];
}
void chip8::xCXNN(unsigned char X, short NN) {
    V[X] = (rand() % 255) & NN;
    program_counter+=2;
}
void chip8::xDXYN(unsigned char X, unsigned char Y, unsigned char N) {
    unsigned short pixel;

    V[0xF] = 0;
    for (int yLine = 0; yLine < N; yLine++) {
        pixel = memory[mem_index + yLine];
        for (int xLine = 0; xLine < 8; xLine++) {
            if ((pixel & (0x80 >> xLine)) != 0) {
                if (gfx[(V[X] + xLine + ((V[Y] + yLine) * DISPLAY_WIDTH))] == 1)
                    V[0xF] = 1;
                //gfx[X + xLine + ((Y + yLine) * DISPLAY_WIDTH)] ^= 1;
                gfx[V[X] + xLine + ((V[Y] + yLine) * DISPLAY_WIDTH)] ^= 1;
            }
        }
    }

    drawFlag = true;
    program_counter += 2;
}

void chip8::xEX9E(unsigned char X) {
    if(key[V[X]] != 0)
        program_counter += 2;

    program_counter += 2;
}
void chip8::xEXA1(unsigned char X) {
    if(key[V[X]] == 0)
        program_counter += 2;
    program_counter += 2;
}

void chip8::xFX07(unsigned char X) {
    V[X] = delay_timer;
    program_counter += 2;
}
void chip8::xFX0A(unsigned char X) {
    bool keyPress = false;

    for (int i = 0; i < 16; ++i) {
        if (key[i] != 0) {
            V[X] = i;
            keyPress = true;
        }
    }

    // If we didn't received a keypress, skip this cycle and try again.
    if (keyPress)
        program_counter += 2;
}
void chip8::xFX15(unsigned char X) {
    delay_timer = V[X];
    program_counter += 2;
}
void chip8::xFX18(unsigned char X) {
    sound_timer = V[X];
    program_counter += 2;
}
void chip8::xFX1E(unsigned char X) {
    mem_index += V[X];
    program_counter += 2;
}
void chip8::xFX29(unsigned char X) {
    mem_index = V[X] * 5; // Height of characters is 5 lines
    program_counter += 2;
}
void chip8::xFX33(unsigned char X) {
    memory[mem_index]     = V[X] / 100;
    memory[mem_index + 1] = (V[X] / 10) % 10;
    memory[mem_index + 2] = (V[X] % 100) % 10;
    program_counter += 2;
}
void chip8::xFX55(unsigned char X) {
    for (int i = 0; i <= X; i++)
        memory[mem_index + i] = V[i];

    // On the original interpreter, when the operation is done, I = I + X + 1.
    mem_index += X + 1;
    program_counter+=2;
}
void chip8::xFX65(unsigned char X) {
    for (int i = 0; i <= X; ++i)
        V[i] = memory[mem_index + i];

    // On the original interpreter, when the operation is done, I = I + X + 1.
    mem_index += X + 1;
    program_counter += 2;
}
