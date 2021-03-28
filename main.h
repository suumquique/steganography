#pragma once

#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>

using namespace std;

#define INFORMATION_BYTES_COUNT 54
// ���������� ��� � �����
#define BITS_IN_BYTE 8
/* ���������� ���, ���������� �� ���������� � ����� �������� ��������� (� �����).
* ��������� ��, ��� �� 1 ��� ���������� ��������� 1 ���� ��� �� ������� (��� �� 2 ���� ���� ����) */
#define MESSAGE_LENGTH_INFORMATION_BITS_COUNT 32
// ���������� ��������� ����� ����� � �����, ����� ���� ����� ���� ���� ���-�� ��������
#define MINIMUM_REQUIRED_FILE_LENGTH_IN_BITS (INFORMATION_BYTES_COUNT * BITS_IN_BYTE + MESSAGE_LENGTH_INFORMATION_BITS_COUNT * BITS_IN_BYTE + sizeof(char) * 8 * BITS_IN_BYTE)

size_t getFileLength(ifstream& file);
