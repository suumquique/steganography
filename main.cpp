#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>

using namespace std;

// Количество байт в формате .bmp, в которые записывается информация о файле, а не цвета пикселей
#define INFORMATION_BYTES_COUNT 54

void main(void) {
	// Устанавливаем русскую кодировку в консоли
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	wstring filePath;
	cout << "Введите путь к изображению, в которое будет помещено зашифрованное сообщение: ";
	getline(wcin, filePath);
	wstring neededFileExtension = L".bmp";
	
	if (filePath.compare(filePath.length() - neededFileExtension.length(), neededFileExtension.length(), neededFileExtension)) {
		cout << "Неверное расширение файла. Требуется файл формата .bmp" << endl;
		return;
	}


}