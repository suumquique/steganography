#include "main.h"

void main(void) {
	// Устанавливаем русскую кодировку в консоли
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	string filePath;
	cout << "Введите путь к изображению, в которое будет помещено зашифрованное сообщение: ";
	getline(cin, filePath);

	if (!isFileValid(filePath)) return;
}

// Возвращает размер файла в байтах
size_t getFileLength(fstream& file) {

	// Сдвигаем указатель на конец файла
	file.seekg(0, ios_base::end);
	// Узнаем, на каком месте стоит указатель
	size_t length_in_bites = file.tellg();
	// Сдвигаем указатель назад в начало файла
	file.seekg(0, ios_base::beg);

	return length_in_bites;
}