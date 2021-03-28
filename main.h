#pragma once

#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>

using namespace std;

#define INFORMATION_BYTES_COUNT 54
// Количество бит в байте
#define BITS_IN_BYTE 8
/* Количество бит, выделенных на информацию о длине скрытого сообщения (в битах).
* Учитываем то, что на 1 бит информации требуется 1 байт для ее скрытия (или на 2 бита один байт) */
#define MESSAGE_LENGTH_INFORMATION_BITS_COUNT 32
// Минимально возможная длина файла в битах, чтобы туда можно было хоть что-то записать
#define MINIMUM_REQUIRED_FILE_LENGTH_IN_BITS (INFORMATION_BYTES_COUNT * BITS_IN_BYTE + MESSAGE_LENGTH_INFORMATION_BITS_COUNT * BITS_IN_BYTE + sizeof(char) * 8 * BITS_IN_BYTE)

size_t getFileLength(ifstream& file);
