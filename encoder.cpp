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

	fstream binaryFileIn(filePath, ios::binary | ios::in); // ���� BMP-����������, ������ ����� ��������� ����������

	cout << "������� ���� � �����, ���������� �������� ��������� ������ � BMP-�����������: ";
	cin >> fileToEncodePath;
	fstream textFile(fileToEncodePath, ios::binary | ios::in);
	if (!textFile.is_open()) return ERROR_FILE_INVALID;
	fileToEncodeExtension = getFileExtension(fileToEncodePath);

	containerLengthInBytes = getFileLength(binaryFileIn);
	fileToEncodeLength = getFileLength(textFile);
	// ���� ��������� ������� ��� (�� ������ �������� ������������� ����) ������ ������
	neededBytesToFitEncodedMessage = (fileToEncodeLength * (BITS_IN_BYTE / packingDegree + 1)) + MINIMUM_REQUIRED_FILE_LENGTH_IN_BYTES;
	if (neededBytesToFitEncodedMessage >= containerLengthInBytes) return ERROR_SMALL_CONTAINER;
	
	// ������� ������ ������� �� ������ ���, ������ ��� ������ ����� �����, ������ ���������� ���� � ����������
	stegocontainer = new bitset<8>[containerLengthInBytes], stegocontainerPtr = stegocontainer;
	// ��������� ������ �� ��������� ����� ��������� � ����������� ����� � bitset �� 8 �����
	for (size_t i = 0; i < containerLengthInBytes; i++) {
		binaryFileIn.read((char*)&temp, sizeof(temp));
		stegocontainer[i] = bitset<8>(temp);
	}
	binaryFileIn.close();
	
	// ���������� 54 ����, ������� � BMP-����� ������������� ��� ��������� ����������
	stegocontainerPtr += INFORMATION_BYTES_COUNT;

	/* ����������� ������� ������ �������� ��� ���� ���������� � �������� �������, ������� ����� ���������� � ��������������.
	� ��� ������ 4 ����� ���������� � ����� ���������, 16 ������ ���������� � ���������� ����������� ����� � ����������
	�������� � ����� ���������� ����� (�� ����, ��������� char, ���������� ������)*/
	binaryInfoToEncodeLength = MESSAGE_LENGTH_INFORMATION_BITS_COUNT / BITS_IN_BYTE + fileToEncodeLength + MAX_EXTENSION_LENGTH;
	binaryInfoToEncode = new bitset<8>[binaryInfoToEncodeLength + 1];
	binaryInfoToEncodePtr = binaryInfoToEncode;

	// ������ �����, ��������� ����� ������� ���� � �������� ����� �����
	size_t fileToEncodeLengthCopy = fileToEncodeLength;
	/*���� �� ����� �� 32 ��� (size_t), ���� �� 8 ��� �� ���� ���, �� ������� ��� � �������, ������
	���������� �� � ������ � �������� �������. ����������, � ����� ������� ���� ������� ����, � ����� �������*/
	for(int i = MESSAGE_LENGTH_INFORMATION_BITS_COUNT / BITS_IN_BYTE - 1; i > 0 ; i--) {
		// ����� 8 ������� ��� � ������ �� � ������, � ������ ��������� � ������ �� �������, ������ ���, ��� ������� ������� ����� ������
		binaryInfoToEncode[i] = bitset<8>(static_cast<uint8_t>(fileToEncodeLengthCopy));
		// ������� 8 ������� ���
		fileToEncodeLengthCopy >>= 8;
	}
	// �������� ���������, ��� ��� ������ ����� ���� �� ����, �� ���������� ����������� ���������
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

	// ��������� ����, �������� ������ � ���
	textFile.close();


	// ������ ��������� � ������� ���� ������ ���������������
	for (size_t i = 0; i < binaryInfoToEncodeLength; i++) {
		// ���� �� ����� � ������ ������� �� 8 ����� �� binaryInfoToEncode
		for (int j = BITS_IN_BYTE - 1; j >= 0; ) {
			// �������� � ������ ����� ��������������� ���������� ������� ���, ������ ������� ��������, �� ���� �� ������ ��� �����������
			for (size_t k = 0; k < packingDegree; k++) {
				/* ��������� � ������� ������� ��������������� (bitset<8>) �� ����� k (��������� � ������� ���)
				��� ��� ������� j �� ������� (���� bitset<8>) ��� ������� i � ������ ��� �����������. ����� ��������� j, �� ���� �����
				����� ������� ��� �� ���� �� ������� ���������� ��� �����������, � ��� ������, ���� �� ������ �� �������� ���� �
				binaryInfoToEncode[i]. ����� ����������� i �� ������� � ����������. */
				(*stegocontainerPtr).set(k, binaryInfoToEncode[i].test(j--));
			}
			/* ����� ����, ��� �� �������� k ������� ��� � ������� ������� (bitset<8>) ���������������, ���������
			* � ���������� �������, ���������� ��������� (�� ����, �� ��������� �������� ����� �������� ��� � ����� ��������) */
			stegocontainerPtr++;
		}
	}

	// ������� ����� �� �����, ������� ����
	ofstream binaryFileOut(filePath, ios::binary | ios::trunc);
	if (!binaryFileOut.is_open()) {
		cout << "���������� ������� ���� �� ����� ����������." << endl;
		return ERROR_FILE_INVALID;
	}

	// ���� ������ �� ����� ���������������
	for (size_t i = 0; i < containerLengthInBytes; i++) {
		// ������� �������� ���������� ������ �� ��������������� � ����
		binaryFileOut << (byte) stegocontainer[i].to_ulong();
	}
	// ��������� ����
	binaryFileOut.close();

	// ���������, �������� �� �� �������� �������� ����� ������ �������� ���������
	if (!isFileValid(filePath)) {
		cout << "���������� �������������� �� �������� �������� BMP-������." << endl;
		return INVALID_FILE_SIZE;
	}

	return ERROR_SUCCESS;
}