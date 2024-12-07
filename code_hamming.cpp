//
// Created by user on 06-Dec-24.
//

#include "code_hamming.h"
uint8_t encodeHamming(uint8_t value) {
    bool d1 = (value >> 3) & 1;
    bool d2 = (value >> 2) & 1;
    bool d3 = (value >> 1) & 1;
    bool d4 = value & 1;

    // 1 ^ d1  === !d1
    bool p1 = !d1 ^ d3 ^ d4;
    bool p2 = !d1 ^ d2 ^ d4;
    bool p3 = !d1 ^ d2 ^ d3;
    bool p4 = !p1 ^ d1 ^ p2 ^ d2 ^ p3 ^ d3 ^ d4;

    std::bitset<8> hammingCode;
    hammingCode[7] = p1; hammingCode[6] = d1;
    hammingCode[5] = p2; hammingCode[4] = d2;
    hammingCode[3] = p3; hammingCode[2] = d3;
    hammingCode[1] = p4; hammingCode[0] = d4;

    return static_cast<uint8_t>(hammingCode.to_ulong());
}

uint8_t decodeHamming(uint8_t code) {
    bool p1 = (code >> 7) & 1; bool d1 = (code >> 6) & 1;
    bool p2 = (code >> 5) & 1; bool d2 = (code >> 4) & 1;
    bool p3 = (code >> 3) & 1; bool d3 = (code >> 2) & 1;
    bool p4 = (code >> 1) & 1; bool d4 = code & 1;

    bool A = p1 ^ d1 ^ d3 ^ d4;
    bool B = d1 ^ p2 ^ d2 ^ d4;
    bool C = d1 ^ d2 ^ p3 ^ d3;
    bool D = p1 ^ d1 ^ p2 ^ d2 ^ p3 ^ d3 ^ p4 ^ d4;

    if ((!A || !B || !C) && D)
        std::cout << "Двойная ошибка!!!\n";
    else if ((!A || !B || !C) && !D) {
        int x = !A + 2*!B + 4*!C + 8*!D;
        switch (x) {
            case 15:
                std::cout << "Ошибка в бите 2!!!\n";
                d1 ^= 1; break;
            case 14:
                std::cout << "Ошибка в бите 4!!!\n";
                d2 ^= 1; break;
            case 13:
                std::cout << "Ошибка в бите 6!!!\n";
                d3 ^= 1; break;
            case 11:
                std::cout << "Ошибка в бите 8!!!\n";
                d4 ^= 1; break;
            case 9:
                std::cout << "Ошибка в бите 1!!!\n";
                p1 ^= 1; break;
            case 10:
                std::cout << "Ошибка в бите 3!!!\n";
                p2 ^= 1; break;
            case 12:
                std::cout << "Ошибка в бите 5!!!\n";
                p3 ^= 1; break;
        }
    }
    else if ((A || B || C) && !D) {
        std::cout << "Ошибка в бите 7!!!\n";
        p4 ^= 1;
    }

    uint8_t result = 0;
    result |= (d1<<3) | (d2<<2) | (d3<<1) | d4;
    return result;
}