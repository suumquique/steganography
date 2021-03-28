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

/*Параметры функции:
1. Поток читаемого файла (по указателю);
2. Количество байтов, которые необходимо считать, начиная с текущей позиции в файле (ios_base::cur). Не больше восьми байт.

Возвращает положительное число, полученное из считанных байтов, если читать справа налево побайтово.
Например, для двух находящихся в файле по порядку байт 01001101 и 01000010 вернет значение 0100001001001101,
то есть 16973 в десятичной системе счисления.*/
unsigned long long getNexBytesNumericValue(fstream& file, size_t bytesCount) {
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

BOOL isFileValid(string filePath) {
    string neededFileExtension = ".bmp";

	// Проверяем, заканчивается ли строка, содержащая путь к файлу, на его расширение - .bmp
	if (filePath.compare(filePath.length() - neededFileExtension.length(), neededFileExtension.length(), neededFileExtension)) {
		cout << "Неверное расширение файла. Требуется файл формата .bmp" << endl;
		return FALSE;
	}

	fstream binaryFile(filePath, ios::binary | ios::in);
	if (!binaryFile.is_open()) {
		cout << "Ошибка: файл не открылся: неверный путь к файлу, либо доступ к нему ограничен." << endl;
		return FALSE;
	}

	/* Проверяем длину файла: если она меньше требуемых для информации о файле BMP 54 байт + 32 байт для информации о длине скрытого
	стеганографически сообщения, то значит, что в этот файл точно ничего записать не получится. */
	if (getFileLength(binaryFile) <= MINIMUM_REQUIRED_FILE_LENGTH_IN_BYTES) {
		cout << "Ошибка: файл скорее всего пуст, или практически все его содержимое удалено. Используйте другой BMP-файл для записи скрытого сообщения" << endl;
		return FALSE;
	}

	return TRUE;
}