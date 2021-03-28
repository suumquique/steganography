#include "main.h"

/*Параметры функции:
1. Поток читаемого файла (по указателю);
2. Количество байтов, которые необходимо считать, начиная с текущей позиции в файле (ios_base::cur). Не больше восьми байт.

Возвращает положительное число, полученное из считанных байтов, если читать справа налево побайтово.
Например, для двух находящихся в файле по порядку байт 01001101 и 01000010 вернет значение 0100001001001101,
то есть 16973 в десятичной системе счисления.*/
unsigned long long getNextBytesNumericValue(fstream& file, size_t bytesCount) {
	byte* bytes = new byte[bytesCount];
	unsigned long long result = 0;

	// Читаем байты подряд
	for (size_t i = 0; i < bytesCount; i++) {
		file.read((char*)&bytes[i], sizeof(bytes[i]));
	}

	// Записываем байты в итоговое число в обратном порядке, каждый раз сдвигая на 8 - количество бит в байте
	for (int i = bytesCount - 1; i >= 0; i--) {
		result = result << BITS_IN_BYTE;
		result |= bytes[i];
	}

	// Возвращаем указатель назад на нужное количество байтов
	file.seekg(- (int) bytesCount);

	delete[] bytes;
	return result;
}

BOOL isFileValid(string filePath) {
	string neededFileExtension = ".bmp";
	size_t fileLength;
	

	// Проверяем, заканчивается ли строка, содержащая путь к файлу, на его расширение - .bmp
	if (getFileExtension(filePath) == neededFileExtension) {
		cout << "Неверное расширение файла. Требуется файл формата .bmp" << endl;
		return FALSE;
	}

	fstream binaryFile(filePath, ios::binary | ios::in);
	if (!binaryFile.is_open()) {
		cout << "Ошибка: файл не открылся: неверный путь к файлу, либо доступ к нему ограничен." << endl;
		return FALSE;
	}
	fileLength = getFileLength(binaryFile);


	/* Проверяем длину файла: если она меньше требуемых для информации о файле BMP 54 байт + 32 байт для информации о длине скрытого
	стеганографически сообщения, то значит, что в этот файл точно ничего записать не получится. */
	if (fileLength <= MINIMUM_REQUIRED_FILE_LENGTH_IN_BYTES) {
		cout << "Ошибка: файл скорее всего пуст, или практически все его содержимое удалено. Используйте другой BMP-файл для записи скрытого сообщения" << endl;
		binaryFile.close();
		return FALSE;
	}

	// Пропукаем первые два байта, переходя к тем, где лежит информация о размере файла
	binaryFile.seekg(2, ios_base::beg);

	// Если реальная длина файла не равна длине, указаной в служебной информации, файл bmp поврежден и с ним работать не стоит
	if (getNextBytesNumericValue(binaryFile, 4) != fileLength) {
		cout << "Реальный размер файла не равен длине, указанной в служебной информации. BMP-файл невалиден." << endl;
		binaryFile.close();
		return FALSE;
	}

	binaryFile.close();

	return TRUE;
}

// Возвращает размер файла в байтах
size_t getFileLength(fstream& file) {

	// Сдвигаем указатель на конец файла
	file.seekg(0, ios_base::end);
	// Узнаем, на каком месте стоит указатель
	size_t length_in_bytes = static_cast<size_t>(file.tellg());
	// Сдвигаем указатель назад в начало файла
	file.seekg(0, ios_base::beg);

	return length_in_bytes;
}