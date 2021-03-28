#include "main.h"

int main(void) {
	// ������������� ������� ��������� � �������
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	string filePath;
	cout << "������� ���� � �����������, � ������� ����� �������� ������������� ���������: ";
	getline(cin, filePath);

	if (isFileValid(filePath) == FALSE) return INVALID_FILE_SIZE;

	WORD packingDegree;
	cout << "������� ������� �������� �����������: 1 - �������� � ���� ���, 2 - �������� � ��� ���� � ��� ����� (�� ������ 8): ";
	cin >> packingDegree;
	
	encode(filePath, packingDegree);

	return ERROR_SUCCESS;
}