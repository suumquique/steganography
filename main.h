#pragma once

#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <bitset>

using namespace std;

#define INFORMATION_BYTES_COUNT 54
// ���������� ��� � �����
#define BITS_IN_BYTE 8
/* ���������� ���, ���������� �� ���������� � ����� �������� ��������� (� �����).
* ��������� ��, ��� �� 1 ��� ���������� ��������� 1 ���� ��� �� ������� (��� �� 2 ���� ���� ����) */
#define MESSAGE_LENGTH_INFORMATION_BITS_COUNT 32
// ���������� ��������� ����� ����� � �����, ����� ���� ����� ���� ���� ���-�� ��������
#define MINIMUM_REQUIRED_FILE_LENGTH_IN_BYTES (INFORMATION_BYTES_COUNT + MESSAGE_LENGTH_INFORMATION_BITS_COUNT + sizeof(char) * 8)

size_t getFileLength(ifstream& file);

/*��������� �������:
1. ����� ��������� ����� (�� ���������);
2. ���������� ������, ������� ���������� �������, ������� � ������� ������� � ����� (ios_base::cur). �� ������ ������ ����.

���������� ������������� �����, ���������� �� ��������� ������, ���� ������ ������ ������ ���������.
��������, ��� ���� ����������� � ����� �� ������� ���� 01001101 � 01000010 ������ �������� 0100001001001101,
�� ���� 16973 � ���������� ������� ���������.*/
static unsigned getNexBytesNumericValue(ifstream& file, size_t bytesCount){
    byte* bytes = new byte[bytesCount];
    unsigned long long result = 0;

    // ������ ����� ������
    for (size_t i = 0; i < bytesCount; i++) {
        file.read((char*)&bytes[i], sizeof(bytes[i]));
    }

    // ���������� ����� � �������� ����� � �������� �������, ������ ��� ������� �� 8 - ���������� ��� � �����
    for (int i = bytesCount - 1; i >= 0; i--) {
        result = result << BITS_IN_BYTE;
        result |= bytes[i];
    }

    delete[] bytes;
    return result;
}
