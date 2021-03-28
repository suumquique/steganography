#include "main.h"

int main(void) {
	// ”станавливаем русскую кодировку в консоли
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	string filePath;
	cout << "¬ведите путь к изображению, в которое будет помещено зашифрованное сообщение: ";
	getline(cin, filePath);

	PackingDegree packingType = PackingDegree::PACKING_ONE_BIT;
	cout << "¬ведите степень упаковки содержимого: 1 - упаковка в один бит, 2 - упаковка в два бита: ";
	cin >> packingType;

	if (isFileValid(filePath) == FALSE) return INVALID_FILE_SIZE;
	
	encode(filePath, packingType);

	return ERROR_SUCCESS;
}