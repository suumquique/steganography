#include "main.h"

int main(void) {
	// ������������� ������� ��������� � �������
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	string filePath; // ���� � BMP-����� (���������������)
	DWORD dwRetStatus; // ������ ��������
	WORD packingDegree; // ������� �������� �����������: 1, 2, 4 ��� 8 ���.
	

	cout << "������� ���� � �����������, � ������� ����� �������� ������������� ���������: ";
	getline(cin, filePath);

	if (isFileValid(filePath) == FALSE) return INVALID_FILE_SIZE;

	cout << "������� ������� �������� �����������: 1 - �������� � ���� ���, 2 - �������� � ��� ���� � ��� ����� (�� ������ 8, ������ ������): ";
	cin >> packingDegree;
	
	dwRetStatus = encode(filePath, packingDegree);
	if (dwRetStatus != ERROR_SUCCESS) {
		cout << "������ ���������� � �������������� �� �������. ������ ��������� ��������� �� �������� " << dwRetStatus << endl;
		return dwRetStatus;
	}
	else cout << "\n\n" << "���������� ������� �������� � �������������� � ���������� � BMP-����." << endl;

	return ERROR_SUCCESS;
}