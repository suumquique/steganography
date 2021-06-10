#include "main.hpp"

int decode(string filePath, WORD packingDegree) {
	if (!isFileValid(filePath)) {
		cout << "���������� ������������, �������������� ����������." << endl;
		return INVALID_FILE_SIZE;
	}

	fstream binaryFile(filePath, ios::binary | ios::in); // ���� ��������������� � ������� BMP, ��������� � �������� �������
	fstream outputFile; // ���� ��� ������, ���� �� ��������������, ��� ��� �� ����� ���������� � ����
	string outputFileName; // ���� � ����� ��� ������ ����������
	size_t fileLength = getFileLength(binaryFile); // ����� BMP-����� � ������ (��� �������� ���������������)
	size_t encodedMessageLength; // ����� ��������������� ��������� � ������
	bitset<32> encodedMessageLengthBitset; // ����� ��������������� ��������� � ������, �������������� � �������� ������� (������)
	byte temp; // ��������� ���������� ��� ���������� ������ �� ��������� ������ � �� ������ � ����
	bitset<8> currentBitset; // ��������� ���������� ��� �������������� �������� ������ � ������
	bitset<8> tempBitset; // ������ ��� ������ � ��������� ������� ��� ���������� �� ���������������

	// ���������� ������ 54 ����� � �����, ��� ��� � ��� ���������� ��������� ���������� BMP
	binaryFile.seekg(INFORMATION_BYTES_COUNT, ios_base::beg);

	/* �������� ����� �������������� ��������� � ������ �� ������ 4 ������������� ������. ��� �����
	* ������������� ������� ���� ���������������, ������� �����, ����� �������� 4 ����� ����������, � ���
	* ���������� ��� ���������� (MESSAGE_LENGTH_INFORMATION_BITS_COUNT) ������ �� ������� ������ (packingDegree). */
	for (size_t i = 0, j = MESSAGE_LENGTH_INFORMATION_BITS_COUNT - 1; i < MESSAGE_LENGTH_INFORMATION_BITS_COUNT / packingDegree; i++) {
		binaryFile.read((char*)&temp, sizeof(temp));
		currentBitset = bitset<8>(temp);
		/* �������� �� ������� ����� ��������������� ����������� ���������� ��������� ����� � ������ �� � ������. ��� ��� �����
		* �������� �����, ������ � ������ ����� �� ������� ����� � ������� */
		for (size_t k = 0; k < packingDegree; k++) {
			encodedMessageLengthBitset[j--] = currentBitset[k];
		}
	}
	// ������������ ������������ �������� ����� �� ������� � unsigned int (size_t)
	encodedMessageLength = encodedMessageLengthBitset.to_ulong();

	// ��������� ��� ����� (��� ����������), ���� ����� �������� ����������� �� ��������������� ������
	cout << "������� ���� � �����, ���� ����� ����������� ����� ���������� (���������� ��������� �� �����): ";
	cin.ignore(32767, '\n');
	getline(cin, outputFileName);
	outputFileName += '.';

	/* ��������� ���������� ����� �� 16 ��������� �������������� ����.
	* ������� ����� 16 �������������� ����, ������ ���� - 8  ���, ��� ������� �������� "1" ������ ��� ������� ������ �����
	* ���������������, ��� ������� �������� "2" - 2 ���� �� ����, ������� ������ �� ������������ ����� ���������� * 8 / ������� �������� */
	for (int i = 0, j = BITS_IN_BYTE - 1; i < MAX_EXTENSION_LENGTH * BITS_IN_BYTE / packingDegree; i++) {
		// ��������� ������� ���� �� ��������������� � ����������� ��� � ������
		binaryFile.read((char*)&temp, sizeof(temp));
		currentBitset = bitset<8>(temp);

		/* ������ ��������� ���� (� ����������� �� ������� ��������) �� �������� ����� (�������) �
		* ���������� �� � ������ �������� �������, ������� �� ������� �������� (�����) (��������� ����� � ���������,
		���� ������� �� ������� �����)*/
		for (size_t k = 0; k < packingDegree; k++, j--) {
			tempBitset[j] = currentBitset[k];
		}

		/* ����� ��������� ������� ������, ����������� ��� � ������, ��������� � ������ � ����� � �����, �����
		* ������� ������ � �������� ������� (������, ���������� � ���������� �������� �������� - 7) */
		if (j <= 0) {
			temp = static_cast<byte>(tempBitset.to_ulong());
			// ���� ������ ������, �� �� ��� �� �����
			if (temp != '\0') outputFileName += temp;
			tempBitset.reset();
			j = BITS_IN_BYTE - 1;
		}
	}

	// ��������� ����, ���� ����� ����������� �������������� ����������, �� ������
	outputFile.open(outputFileName, ios::binary | ios::out | ios::trunc);
	// ���������, ��������� �� �� ��������
	if (!outputFile.is_open()) {
		cout << "������: ���������� ������� ��������� ���� �� ������. �������� ���� ��� ������ � �������� ��������." << endl;
		return ERROR_FILE_INVALID;
	}

	/* ������ ��������� �� ��������������� ���� ������������� ������ (��� ��� � ��� ��� ���� �� ����� � ������).
	� ��������, ��� ���������� ����������� �����, ������ �������� ������� ����� ���������� � ����*/
	for (int i = 0, j = BITS_IN_BYTE - 1; i < encodedMessageLength * BITS_IN_BYTE / packingDegree; i++) {
		binaryFile.read((char*)&temp, sizeof(temp));
		currentBitset = bitset<8>(temp);

		for (size_t k = 0; k < packingDegree; k++, j--) {
			tempBitset[j] = currentBitset[k];
		}

		/* ����� ��������� ������� ������, ����������� ��� � ������, ��������� � ������ � ����� � �����, �����
		* ������� ������ � �������� ������� (������, ���������� � ���������� �������� �������� - 7) */
		if (j <= 0) {
			outputFile << static_cast<byte>(tempBitset.to_ulong());
			tempBitset.reset();
			j = BITS_IN_BYTE - 1;
		}
	}

	// ��������� ���� �� ������
	outputFile.close();

	// ���������, ��������� �� ������ ����� � ��������� ��������
	outputFile.open(outputFileName, ios::binary | ios::in);
	if (getFileLength(outputFile) != encodedMessageLength) {
		cout << "������: � �������� ���� ���� �������� �� ��� �������������� ���������� �� ���������������." << endl;
		return INVALID_FILE_SIZE;
	}
	outputFile.close();
	
	return ERROR_SUCCESS;
}