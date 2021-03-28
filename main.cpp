#include "main.h"

void main(void) {
	// ������������� ������� ��������� � �������
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	string filePath;
	string neededFileExtension = ".bmp";
	cout << "������� ���� � �����������, � ������� ����� �������� ������������� ���������: ";
	getline(cin, filePath);


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

	/* ��������� ����� �����: ���� ��� ������ ��������� ��� ���������� � ����� BMP 54 ���� + 32 ���� ��� ���������� � ����� ��������
	����������������� ���������, �� ������, ��� � ���� ���� ����� ������ �������� �� ���������. */
	if (getFileLength(binaryFile) <= MINIMUM_REQUIRED_FILE_LENGTH_IN_BYTES) {
		cout << "������: ���� ������ ����� ����, ��� ����������� ��� ��� ���������� �������. ����������� ������ BMP-���� ��� ������ �������� ���������" << endl;
		return;
	}
	
	/* �������� ��������� � �����, ��������� ��� �����, ���������� ���������� � ����� (54 ������),
	* ����� ����� ������ ���������� � ��������� ������, ������������ ����� �������� */
	binaryFile.seekg(INFORMATION_BYTES_COUNT, ios_base::beg);
	
}

// ���������� ������ ����� � ������
size_t getFileLength(ifstream& file) {

	// �������� ��������� �� ����� �����
	file.seekg(0, ios_base::end);
	// ������, �� ����� ����� ����� ���������
	size_t length_in_bites = file.tellg();
	// �������� ��������� ����� � ������ �����
	file.seekg(0, ios_base::beg);

	return length_in_bites;
}