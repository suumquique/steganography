#include "main.h"

int main(void) {
	// Устанавливаем русскую кодировку в консоли
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	string filePath; // Путь к BMP-файлу (стегоконтейнеру)
	DWORD dwRetStatus; // Статус возврата
	WORD packingDegree; // Степень упаковки содержимого: 1, 2, 4 или 8 бит.
	

	cout << "Введите путь к изображению, в которое будет помещено зашифрованное сообщение: ";
	getline(cin, filePath);

	if (isFileValid(filePath) == FALSE) return INVALID_FILE_SIZE;

	cout << "Введите степень упаковки содержимого: 1 - упаковка в один бит, 2 - упаковка в два бита и так далее (не больше 8, только четные): ";
	cin >> packingDegree;
	if (packingDegree < 1 && packingDegree > 8 && packingDegree % 2 != 0) {
		cout << "Введена некорректная степень упаковки информации." << endl;
		return ERROR_INVALID_PACKING_DEGREE;
	}
	

	dwRetStatus = encode(filePath, packingDegree);
	if (dwRetStatus != ERROR_SUCCESS) {
		cout << "Запись информации в стегоконтейнер не удалась. Работа программы завершена со статусом " << dwRetStatus << endl;
		return dwRetStatus;
	}
	else cout << "\n" << "Информация успешно записана в стегоконтейнер и перенесена в BMP-файл." << endl << endl;


	dwRetStatus = decode(filePath, packingDegree);
	if (dwRetStatus != ERROR_SUCCESS) {
		cout << "Корректно считать информацию из стегоконтейнера не удалось. Работа программы завершена со статусом " << dwRetStatus << endl;
		return dwRetStatus;
	}
	else cout << "\n" << "Декодированная информация успешно записана в указанный вами файл вывода." << endl << endl;

	return ERROR_SUCCESS;
}