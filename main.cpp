#include "main.h"

// Количество байт в формате .bmp, в которые записывается информация о файле, а не цвета пикселей
#define INFORMATION_BYTES_COUNT 54
// Количество бит в байте
#define BITS_IN_BYTE 8
/* Количество бит, выделенных на информацию о длине скрытого сообщения (в битах).
* Учитываем то, что на 1 бит информации требуется 1 байт для ее скрытия (или на 2 бита один байт) */
#define MESSAGE_LENGTH_INFORMATION_BITS_COUNT 32

void main(void) {
	// Устанавливаем русскую кодировку в консоли
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	wstring filePath;
	wstring neededFileExtension = L".bmp";
	cout << "Введите путь к изображению, в которое будет помещено зашифрованное сообщение: ";
	getline(wcin, filePath);


	// Проверяем, заканчивается ли строка, содержащая путь к файлу, на его расширение - .bmp
	if (filePath.compare(filePath.length() - neededFileExtension.length(), neededFileExtension.length(), neededFileExtension)) {
		cout << "Неверное расширение файла. Требуется файл формата .bmp" << endl;
		return;
	}

	ifstream binaryFile;
	binaryFile.open(filePath, ios::binary);
	if (!binaryFile.is_open()) {
		cout << "Ошибка: файл не открылся: неверный путь к файлу, либо доступ к нему ограничен." << endl;
		return;
	}

	/* Проверяем длину файла: если она меньше требуемых для информации о файле BMP 54 байт + 32 байт для информации о длине скрытого
	стеганографически сообщения, то значит, что в этот файл точно ничего записать не получится. */
	if (getFileLength(binaryFile) <= INFORMATION_BYTES_COUNT * BITS_IN_BYTE + MESSAGE_LENGTH_INFORMATION_BITS_COUNT * BITS_IN_BYTE) {
		cout << "Ошибка: файл скорее всего пуст, или практически все его содержимое удалено. Используйте другой BMP-файл для записи скрытого сообщения" << endl;
		return;
	}
	
	/* Сдвигаем указатель в файле, пропуская все биты, содержащие информацию о файле (54 первых),
	* чтобы сразу начать считывание и изменение байтов, определяющих цвета пикселей */
	binaryFile.seekg(INFORMATION_BYTES_COUNT * BITS_IN_BYTE, ios_base::beg);
	
}

size_t getFileLength(ifstream& file) {

	// Сдвигаем указатель на конец файла
	file.seekg(0, ios_base::end);
	// Узнаем, на каком месте стоит указатель
	size_t length_in_bites = file.tellg();
	// Сдвигаем указатель назад в начало файла
	file.seekg(0, ios_base::beg);

	return length_in_bites;
}