#include "main.h"

void main(void) {
	// ������������� ������� ��������� � �������
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	string filePath;
	cout << "������� ���� � �����������, � ������� ����� �������� ������������� ���������: ";
	getline(cin, filePath);

	if (isFileValid(filePath) == FALSE) return;
	
	encode(filePath);
}