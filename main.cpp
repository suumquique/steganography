#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>

using namespace std;

// ���������� ���� � ������� .bmp, � ������� ������������ ���������� � �����, � �� ����� ��������
#define INFORMATION_BYTES_COUNT 54

void main(void) {
	// ������������� ������� ��������� � �������
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	wstring filePath;
	cout << "������� ���� � �����������, � ������� ����� �������� ������������� ���������: ";
	getline(wcin, filePath);
	wstring neededFileExtension = L".bmp";
	
	if (filePath.compare(filePath.length() - neededFileExtension.length(), neededFileExtension.length(), neededFileExtension)) {
		cout << "�������� ���������� �����. ��������� ���� ������� .bmp" << endl;
		return;
	}


}