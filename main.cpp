#include "main.h"

// Почему-то не существовало оператора cin для enum, поэтому пришлось добавить перегрузку
istream& operator>>(istream& is, PackingDegree& i)
{
	int tmp;
	if (is >> tmp)
		i = static_cast<PackingDegree>(tmp);
	return is;
}

int main(void) {
	// Устанавливаем русскую кодировку в консоли
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	string filePath;
	cout << "Введите путь к изображению, в которое будет помещено зашифрованное сообщение: ";
	getline(cin, filePath);

	PackingDegree packingType = PackingDegree::PACKING_ONE_BIT;
	cout << "Введите степень упаковки содержимого: 1 - упаковка в один бит, 2 - упаковка в два бита: ";
	cin >> packingType;

	if (isFileValid(filePath) == FALSE) return INVALID_FILE_SIZE;
	
	encode(filePath, packingType);

	return ERROR_SUCCESS;
}