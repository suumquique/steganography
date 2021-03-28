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

size_t getFileLength(ifstream& file);

/*Параметры функции:
1. Поток читаемого файла (по указателю);
2. Количество байтов, которые необходимо считать, начиная с текущей позиции в файле (ios_base::cur). Не больше восьми байт.

Возвращает положительное число, полученное из считанных байтов, если читать справа налево побайтово.
Например, для двух находящихся в файле по порядку байт 01001101 и 01000010 вернет значение 0100001001001101,
то есть 16973 в десятичной системе счисления.*/
static unsigned getNexBytesNumericValue(ifstream& file, size_t bytesCount){
    byte* bytes = new byte[bytesCount];
    unsigned long long result = 0;

    // Читаем байты подряд
    for (size_t i = 0; i < bytesCount; i++) {
        file.read((char*)&bytes[i], sizeof(bytes[i]));
    }

    // Записываем байты в итоговое число в обратном порядке, каждый раз сдвигая на 8 - количество бит в байте
    for (int i = bytesCount - 1; i >= 0; i--) {
        result = result << BITS_IN_BYTE;
        result |= bytes[i];
    }

    delete[] bytes;
    return result;
}
