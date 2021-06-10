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
// Максимальная длина расширения файла
#define MAX_EXTENSION_LENGTH 20
// Минимально возможная длина файла в битах, чтобы туда можно было хоть что-то записать
#define MINIMUM_REQUIRED_FILE_LENGTH_IN_BYTES (INFORMATION_BYTES_COUNT + MESSAGE_LENGTH_INFORMATION_BITS_COUNT + sizeof(char) * BITS_IN_BYTE + MAX_EXTENSION_LENGTH * BITS_IN_BYTE)
// Ошибка, показывающая, что размер BMP-файла для скрытия текста слишком мал
#define ERROR_SMALL_CONTAINER 999
// Ошибка, показывающая, что введенная степень упаковки информации не является валидной
#define ERROR_INVALID_PACKING_DEGREE 998

#define getFileExtension(filePath) filePath.substr(filePath.find_last_of('.') + 1)

size_t getFileLength(fstream& file);
unsigned long long getNextBytesNumericValue(fstream& file, size_t bytesCount);
BOOL isFileValid(string filePath);
int encode(string filePath, WORD packingDegree);
int decode(string filePath, WORD packingDegree);