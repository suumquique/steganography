#pragma once

#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <bitset>

using namespace std;

#define INFORMATION_BYTES_COUNT 54
//  оличество бит в байте
#define BITS_IN_BYTE 8
/*  оличество бит, выделенных на информацию о длине скрытого сообщени€ (в битах).
* ”читываем то, что на 1 бит информации требуетс€ 1 байт дл€ ее скрыти€ (или на 2 бита один байт) */
#define MESSAGE_LENGTH_INFORMATION_BITS_COUNT 32
// ћаксимальна€ длина расширени€ файла
#define MAX_EXTENSION_LENGTH 20
// ћинимально возможна€ длина файла в битах, чтобы туда можно было хоть что-то записать
#define MINIMUM_REQUIRED_FILE_LENGTH_IN_BYTES (INFORMATION_BYTES_COUNT + MESSAGE_LENGTH_INFORMATION_BITS_COUNT + sizeof(char) * BITS_IN_BYTE + MAX_EXTENSION_LENGTH * BITS_IN_BYTE)

// —тепень упаковки скрытого сообщени€: измен€етс€ либо один последний бит каждого байта палитры, либо два последних бита
enum class PackingDegree {PACKING_ONE_BIT = 1, BACKING_TWO_BITS};

// ѕочему-то не существовало оператора cin дл€ enum, поэтому пришлось добавить перегрузку
istream& operator>>(std::istream& is, PackingDegree& i)
{
    int tmp;
    if (is >> tmp)
        i = static_cast<PackingDegree>(tmp);
    return is;
}

size_t getFileLength(fstream& file);
unsigned long long getNexBytesNumericValue(fstream& file, size_t bytesCount);
BOOL isFileValid(string filePath);
int encode(string filePath, PackingDegree packingDegree);
int decode(string filePath, PackingDegree packingDegree);