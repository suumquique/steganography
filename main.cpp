#include "main.h"

// ������-�� �� ������������ ��������� cin ��� enum, ������� �������� �������� ����������
istream& operator>>(istream& is, PackingDegree& i)
{
	int tmp;
	if (is >> tmp)
		i = static_cast<PackingDegree>(tmp);
	return is;
}

int main(void) {
	// ������������� ������� ��������� � �������
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	string filePath;
	cout << "������� ���� � �����������, � ������� ����� �������� ������������� ���������: ";
	getline(cin, filePath);

	PackingDegree packingType = PackingDegree::PACKING_ONE_BIT;
	cout << "������� ������� �������� �����������: 1 - �������� � ���� ���, 2 - �������� � ��� ����: ";
	cin >> packingType;

	if (isFileValid(filePath) == FALSE) return INVALID_FILE_SIZE;
	
	encode(filePath, packingType);

	return ERROR_SUCCESS;
}