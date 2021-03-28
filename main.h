#pragma once

#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <bitset>

using namespace std;

#define INFORMATION_BYTES_COUNT 54
// Количество бит в байте
#define BITS_IN_BYTE 8
/* Количество бит, выделенных на информацию о длине скрытого сообщения (в битах).
* Учитываем то, что на 1 бит информации требуется 1 байт для ее скрытия (или на 2 бита один байт) */
#define MESSAGE_LENGTH_INFORMATION_BITS_COUNT 32
// Минимально возможная длина файла в битах, чтобы туда можно было хоть что-то записать
#define MINIMUM_REQUIRED_FILE_LENGTH_IN_BYTES (INFORMATION_BYTES_COUNT + MESSAGE_LENGTH_INFORMATION_BITS_COUNT + sizeof(char) * 8)

size_t getFileLength(fstream& file);
unsigned long long getNexBytesNumericValue(fstream& file, size_t bytesCount);
BOOL isFileValid(string filePath);
int encode(string filePath);
void decode(string filePath);