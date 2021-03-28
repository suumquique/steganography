#include "main.h"

int main(void) {
	// ”станавливаем русскую кодировку в консоли
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	string filePath;
	cout << "¬ведите путь к изображению, в которое будет помещено зашифрованное сообщение: ";
	getline(cin, filePath);

	if (isFileValid(filePath) == FALSE) return INVALID_FILE_SIZE;

	WORD packingDegree;
	cout << "¬ведите степень упаковки содержимого: 1 - упаковка в один бит, 2 - упаковка в два бита и так далее (не больше 8): ";
	cin >> packingDegree;
	
	encode(filePath, packingDegree);

	return ERROR_SUCCESS;
}