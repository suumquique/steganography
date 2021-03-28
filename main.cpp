#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>

using namespace std;

// ���������� ���� � ������� .bmp, � ������� ������������ ���������� � �����, � �� ����� ��������
#define INFORMATION_BYTES_COUNT 54
// ���������� ��� � �����
#define BITS_IN_BYTE 8
/* ���������� ���, ���������� �� ���������� � ����� �������� ��������� (� �����).
* ��������� ��, ��� �� 1 ��� ���������� ��������� 1 ���� ��� �� ������� (��� �� 2 ���� ���� ����) */
#define MESSAGE_LENGTH_INFORMATION_BITS_COUNT 32

size_t getFileLength(ifstream& file) {

	// �������� ��������� �� ����� �����
	file.seekg(0, ios_base::end);
	// ������, �� ����� ����� ����� ���������
	size_t length_in_bites = file.tellg();
	// �������� ��������� ����� � ������ �����
	file.seekg(0, ios_base::beg);

	return length_in_bites;
}

void main(void) {
	// ������������� ������� ��������� � �������
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	wstring filePath;
	wstring neededFileExtension = L".bmp";
	cout << "������� ���� � �����������, � ������� ����� �������� ������������� ���������: ";
	getline(wcin, filePath);


	// ���������, ������������� �� ������, ���������� ���� � �����, �� ��� ���������� - .bmp
	if (filePath.compare(filePath.length() - neededFileExtension.length(), neededFileExtension.length(), neededFileExtension)) {
		cout << "�������� ���������� �����. ��������� ���� ������� .bmp" << endl;
		return;
	}

	ifstream binaryFile;
	binaryFile.open(filePath, ios::binary);
	if (!binaryFile.is_open()) {
		cout << "������: ���� �� ��������: �������� ���� � �����, ���� ������ � ���� ���������." << endl;
		return;
	}

	if (getFileLength(binaryFile) <= INFORMATION_BYTES_COUNT * BITS_IN_BYTE + MESSAGE_LENGTH_INFORMATION_BITS_COUNT * BITS_IN_BYTE) {
		cout << "������: ���� ������ ����� ����, ��� ����������� ��� ��� ���������� �������. ����������� ������ BMP-���� ��� ������ �������� ���������" << endl;
		return;
	}
	
}