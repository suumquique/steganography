#include "main.h"

void main(void) {
	// ”станавливаем русскую кодировку в консоли
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	string filePath;
	cout << "¬ведите путь к изображению, в которое будет помещено зашифрованное сообщение: ";
	getline(cin, filePath);

	if (isFileValid(filePath) == FALSE) return;
	
	encode(filePath);
}