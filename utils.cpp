#include "main.h"

/*��������� �������:
1. ����� ��������� ����� (�� ���������);
2. ���������� ������, ������� ���������� �������, ������� � ������� ������� � ����� (ios_base::cur). �� ������ ������ ����.

���������� ������������� �����, ���������� �� ��������� ������, ���� ������ ������ ������ ���������.
��������, ��� ���� ����������� � ����� �� ������� ���� 01001101 � 01000010 ������ �������� 0100001001001101,
�� ���� 16973 � ���������� ������� ���������.*/
unsigned long long getNexBytesNumericValue(fstream& file, size_t bytesCount) {
	byte* bytes = new byte[bytesCount];
	unsigned long long result = 0;

	// ������ ����� ������
	for (size_t i = 0; i < bytesCount; i++) {
		file.read((char*)&bytes[i], sizeof(bytes[i]));
	}

	// ���������� ����� � �������� ����� � �������� �������, ������ ��� ������� �� 8 - ���������� ��� � �����
	for (int i = bytesCount - 1; i >= 0; i--) {
		result = result << BITS_IN_BYTE;
		result |= bytes[i];
	}

	delete[] bytes;
	return result;
}

BOOL isFileValid(string filePath) {
	string neededFileExtension = ".bmp";

	// ���������, ������������� �� ������, ���������� ���� � �����, �� ��� ���������� - .bmp
	if (filePath.compare(filePath.length() - neededFileExtension.length(), neededFileExtension.length(), neededFileExtension)) {
		cout << "�������� ���������� �����. ��������� ���� ������� .bmp" << endl;
		return FALSE;
	}

	fstream binaryFile(filePath, ios::binary | ios::in);
	if (!binaryFile.is_open()) {
		cout << "������: ���� �� ��������: �������� ���� � �����, ���� ������ � ���� ���������." << endl;
		return FALSE;
	}

	/* ��������� ����� �����: ���� ��� ������ ��������� ��� ���������� � ����� BMP 54 ���� + 32 ���� ��� ���������� � ����� ��������
	����������������� ���������, �� ������, ��� � ���� ���� ����� ������ �������� �� ���������. */
	if (getFileLength(binaryFile) <= MINIMUM_REQUIRED_FILE_LENGTH_IN_BYTES) {
		cout << "������: ���� ������ ����� ����, ��� ����������� ��� ��� ���������� �������. ����������� ������ BMP-���� ��� ������ �������� ���������" << endl;
		return FALSE;
	}

	return TRUE;
}

// ���������� ������ ����� � ������
size_t getFileLength(fstream& file) {

	// �������� ��������� �� ����� �����
	file.seekg(0, ios_base::end);
	// ������, �� ����� ����� ����� ���������
	size_t length_in_bytes = file.tellg();
	// �������� ��������� ����� � ������ �����
	file.seekg(0, ios_base::beg);

	return length_in_bytes;
}