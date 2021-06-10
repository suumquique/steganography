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
// ������������ ����� ���������� �����
#define MAX_EXTENSION_LENGTH 20
// ���������� ��������� ����� ����� � �����, ����� ���� ����� ���� ���� ���-�� ��������
#define MINIMUM_REQUIRED_FILE_LENGTH_IN_BYTES (INFORMATION_BYTES_COUNT + MESSAGE_LENGTH_INFORMATION_BITS_COUNT + sizeof(char) * BITS_IN_BYTE + MAX_EXTENSION_LENGTH * BITS_IN_BYTE)
// ������, ������������, ��� ������ BMP-����� ��� ������� ������ ������� ���
#define ERROR_SMALL_CONTAINER 999
// ������, ������������, ��� ��������� ������� �������� ���������� �� �������� ��������
#define ERROR_INVALID_PACKING_DEGREE 998

#define getFileExtension(filePath) filePath.substr(filePath.find_last_of('.') + 1)

size_t getFileLength(fstream& file);
unsigned long long getNextBytesNumericValue(fstream& file, size_t bytesCount);
BOOL isFileValid(string filePath);
int encode(string filePath, WORD packingDegree);
int decode(string filePath, WORD packingDegree);