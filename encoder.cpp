#include "main.h"

int encode(string filePath, WORD packingDegree) {
	string fileToEncodePath; // ���� � �����, ��������� �������� ��������� ����������� (��������)
	string fileToEncodeExtension; // ���������� �����, ���������� �������� ��������� ������������ (�������� � ��������������)
	size_t fileToEncodeLength; // ������ �����, ���������� �������� ��������� ������������, � ������
	size_t containerLengthInBytes; // ����� ��������������� (����� BMP) � ������
	size_t neededBytesToFitEncodedMessage; // ���������� ������, ������, ����� �������� ������������� ���� (���������) � ��������� ���������� ����������
	size_t binaryInfoToEncodeLength; // ����� ����� ���������� ��� �����������
	bitset<BITS_IN_BYTE>* binaryInfoToEncode, *binaryInfoToEncodePtr; // ������ c ����������� �� ����� � ��������� ����������� (����������) ��� ����������� � �������� ����
	bitset<BITS_IN_BYTE>* stegocontainer, *stegocontainerPtr; // ������ � ������� �� BMP-����� (���������������) � �������� ����
	bitset<BITS_IN_BYTE * 4> messageToEncodeLength; // ����� ����������� ��������� � ������� �������
	byte temp; // ���������� ���������� ��� ����������� ����������

	cout << "������� ���� � �����, ���������� �������� ��������� ������ � BMP-�����������: ";
	cin >> fileToEncodePath;
	fstream textFile(fileToEncodePath, ios::binary | ios::in);
	if (!textFile.is_open()) return ERROR_FILE_INVALID;
	fileToEncodeExtension = getFileExtension(fileToEncodePath);

	fstream binaryFile(filePath, ios::binary | ios::in);
	containerLengthInBytes = getFileLength(binaryFile);
	fileToEncodeLength = getFileLength(textFile);
	// ���� ��������� ������� ��� (�� ������ �������� ������������� ����) ������ ������
	neededBytesToFitEncodedMessage = (fileToEncodeLength * (BITS_IN_BYTE / packingDegree + 1)) + MINIMUM_REQUIRED_FILE_LENGTH_IN_BYTES;
	if (neededBytesToFitEncodedMessage >= containerLengthInBytes) return ERROR_SMALL_CONTAINER;
	
	// ������� ������ ������� �� ������ ���, ������ ��� ������ ����� �����, ������ ���������� ���� � ����������
	stegocontainer = new bitset<8>[containerLengthInBytes], stegocontainerPtr = stegocontainer;
	// ��������� ������ �� ��������� ����� ��������� � ����������� ����� � bitset �� 8 �����
	for (size_t i = 0; i < containerLengthInBytes; i++) {
		binaryFile.read((char*)&temp, sizeof(temp));
		stegocontainer[i] = bitset<8>(temp);
	}
	binaryFile.close();
	
	// ���������� 54 ����, ������� � BMP-����� ������������� ��� ��������� ����������
	stegocontainerPtr += INFORMATION_BYTES_COUNT;

	/* ����������� ������� ������ �������� ��� ���� ���������� � �������� �������, ������� ����� ���������� � ��������������.
	� ��� ������ 4 ����� ���������� � ����� ���������, 16 ������ ���������� � ���������� ����������� ����� � ����������
	�������� � ����� ���������� ����� (�� ����, ��������� char, ���������� ������)*/
	binaryInfoToEncodeLength = MESSAGE_LENGTH_INFORMATION_BITS_COUNT / BITS_IN_BYTE + fileToEncodeLength + MAX_EXTENSION_LENGTH;
	binaryInfoToEncode = new bitset<8>[binaryInfoToEncodeLength + 1];
	binaryInfoToEncodePtr = binaryInfoToEncode;

	/*���������� size_t �������� ����� (���������� ��������) � ����� � bitset �� 32 ��� � ������������ ��� � �������� ������
	���������� ��� ����������� */
	messageToEncodeLength = bitset<32>(fileToEncodeLength);
	for (size_t i = 0; i < MESSAGE_LENGTH_INFORMATION_BITS_COUNT; i++) {
		binaryInfoToEncode[i / 8][i % 8] = messageToEncodeLength[i];
	}
	// ��� ��� ����� ������������ ����������� ����������, ����������, ��� 4 ������ ��� ������
	binaryInfoToEncodePtr += MESSAGE_LENGTH_INFORMATION_BITS_COUNT / BITS_IN_BYTE;

	/*������ ���������� ���������� ��������� �����, ������� ������ (��������). ���������� ����� �������� � ������ 
	���������� �� ��� 16 ���, � ����� ������ ����, ������� � ����� ����� ����������������� � ������� ����� ������*/
	for (size_t i = 0; i < MAX_EXTENSION_LENGTH; i++) {
		if (i < fileToEncodeExtension.length()) (*binaryInfoToEncodePtr++) = bitset<8>(fileToEncodeExtension[i]);
		else (*binaryInfoToEncodePtr++) = bitset<8>((unsigned char) 0);
	}

	// � ������� ���������� ��� �����, ����� �� ����� ����� (�����) � ���������� ������ ���� � bitset
	for (size_t i = 0; i < fileToEncodeLength; i++) {
		textFile.read((char*)&temp, sizeof(temp));
		*binaryInfoToEncodePtr++ = bitset<8>(temp);
	}

	for (size_t i = 0; i < binaryInfoToEncodeLength;) {

	}

	return ERROR_SUCCESS;
}