#include "main.hpp"

int decode(string filePath, WORD packingDegree) {
	if (!isFileValid(filePath)) {
		cout << "Невозможно декодировать, стегоконтейнер невалидный." << endl;
		return INVALID_FILE_SIZE;
	}

	fstream binaryFile(filePath, ios::binary | ios::in); // Файл стегоконтейнера в формате BMP, открываем в двоичном формате
	fstream outputFile; // Файл для вывода, пока не инициализируем, так как не знаем расширение и путь
	string outputFileName; // Путь к файлу для вывода информации
	size_t fileLength = getFileLength(binaryFile); // Длина BMP-файла в байтах (для создания стегоконтейнера)
	size_t encodedMessageLength; // Длина закодированного сообщения в байтах
	bitset<32> encodedMessageLengthBitset; // Длина закодированного сообщения в байтах, представленная в двоичном формате (битсет)
	byte temp; // Временная переменная для считывания байтов из двоичного потока и их записи в файл
	bitset<8> currentBitset; // Временная переменная для преобразования входящих байтов в битсет
	bitset<8> tempBitset; // Битсет для работы с байтовыми данными при считывании из стегоконтейнера

	// Пропускаем первые 54 байта в файле, так как в них содержится служебная информация BMP
	binaryFile.seekg(INFORMATION_BYTES_COUNT, ios_base::beg);

	/* Получаем длину зашифрованного сообщения в байтах из первых 4 зашифрованных байтов. Для этого
	* просматриваем столько байт стегоконтейнера, сколько нужно, чтобы вместить 4 байта информации, а это
	* количество бит информации (MESSAGE_LENGTH_INFORMATION_BITS_COUNT) делить на степень сжатия (packingDegree). */
	for (size_t i = 0, j = MESSAGE_LENGTH_INFORMATION_BITS_COUNT - 1; i < MESSAGE_LENGTH_INFORMATION_BITS_COUNT / packingDegree; i++) {
		binaryFile.read((char*)&temp, sizeof(temp));
		currentBitset = bitset<8>(temp);
		/* Вынимаем из каждого байта стегоконтейнера требующееся количество последних битов и кладем их в битсет. Так как число
		* записано прямо, класть в битсет нужно от больших битов к меньшим */
		for (size_t k = 0; k < packingDegree; k++) {
			encodedMessageLengthBitset[j--] = currentBitset[k];
		}
	}
	// Конвертируем получившееся двоичное число из битсета в unsigned int (size_t)
	encodedMessageLength = encodedMessageLengthBitset.to_ulong();

	// Считываем имя файла (без расширения), куда будут записаны извлеченные из стегоконтейнера данные
	cout << "Введите путь к файлу, куда будет осуществлен вывод информации (расширение указывать не нужно): ";
	cin.ignore(32767, '\n');
	getline(cin, outputFileName);
	outputFileName += '.';

	/* Считываем расширение файла из 16 следующих закодированных байт.
	* Считать нужно 16 закодированных байт, каждый байт - 8  бит, при степени упаковки "1" каждый бит требует одного байта
	* стегоконтейнера, при степени упаковки "2" - 2 бита на байт, поэтому читаем до максимальной длины расширения * 8 / степень упаковки */
	for (int i = 0, j = BITS_IN_BYTE - 1; i < MAX_EXTENSION_LENGTH * BITS_IN_BYTE / packingDegree; i++) {
		// Считываем текущий байт из стегоконтейнера и преобразуем его в битсет
		binaryFile.read((char*)&temp, sizeof(temp));
		currentBitset = bitset<8>(temp);

		/* Читаем последние биты (в зависимости от степени упаковки) из текущего байта (битсета) и
		* записываем их в битсет текущего символа, начиная со старших степеней (битов) (поскольку клали в контейнер,
		тоже начиная со старших битов)*/
		for (size_t k = 0; k < packingDegree; k++, j--) {
			tempBitset[j] = currentBitset[k];
		}

		/* Когда заполнили текущий битсет, преобразуем его в символ, добавляем в строку с путем к файлу, затем
		* очищаем битсет и обнуляем счетчик (точнее, возвращаем к начальному старшему значению - 7) */
		if (j <= 0) {
			temp = static_cast<byte>(tempBitset.to_ulong());
			// Если символ пустой, то он нам не нужен
			if (temp != '\0') outputFileName += temp;
			tempBitset.reset();
			j = BITS_IN_BYTE - 1;
		}
	}

	// Открываем файл, куда будет загружаться расшифрованная информация, на запись
	outputFile.open(outputFileName, ios::binary | ios::out | ios::trunc);
	// Проверяем, корректно ли он открылся
	if (!outputFile.is_open()) {
		cout << "Ошибка: невозможно открыть указанный файл на запись. Неверный путь или доступ к каталогу запрещен." << endl;
		return ERROR_FILE_INVALID;
	}

	/* Теперь считываем из стегоконтейнера сами зашифрованные данные (так как у нас уже есть их длина в байтах).
	В принципе, все аналогично предыдущему циклу, только итоговые символы будем записывать в файл*/
	for (int i = 0, j = BITS_IN_BYTE - 1; i < encodedMessageLength * BITS_IN_BYTE / packingDegree; i++) {
		binaryFile.read((char*)&temp, sizeof(temp));
		currentBitset = bitset<8>(temp);

		for (size_t k = 0; k < packingDegree; k++, j--) {
			tempBitset[j] = currentBitset[k];
		}

		/* Когда заполнили текущий битсет, преобразуем его в символ, добавляем в строку с путем к файлу, затем
		* очищаем битсет и обнуляем счетчик (точнее, возвращаем к начальному старшему значению - 7) */
		if (j <= 0) {
			outputFile << static_cast<byte>(tempBitset.to_ulong());
			tempBitset.reset();
			j = BITS_IN_BYTE - 1;
		}
	}

	// Закрываем файл на запись
	outputFile.close();

	// Проверяем, совпадает ли размер файла с ожидаемым размером
	outputFile.open(outputFileName, ios::binary | ios::in);
	if (getFileLength(outputFile) != encodedMessageLength) {
		cout << "Ошибка: в выходной файл была записана не вся расшифрованная информация из стегоконтейнера." << endl;
		return INVALID_FILE_SIZE;
	}
	outputFile.close();
	
	return ERROR_SUCCESS;
}