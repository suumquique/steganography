#include "main.h"

void main(void) {
	// ������������� ������� ��������� � �������
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	string filePath;
	cout << "������� ���� � �����������, � ������� ����� �������� ������������� ���������: ";
	getline(cin, filePath);

	if (!isFileValid(filePath)) return;
}

// ���������� ������ ����� � ������
size_t getFileLength(fstream& file) {

	// �������� ��������� �� ����� �����
	file.seekg(0, ios_base::end);
	// ������, �� ����� ����� ����� ���������
	size_t length_in_bites = file.tellg();
	// �������� ��������� ����� � ������ �����
	file.seekg(0, ios_base::beg);

	return length_in_bites;
}