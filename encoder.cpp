#include "main.h"

int encode(string filePath, WORD packingDegree) {
	string fileToEncodePath; // Путь к файлу, содержимо которого требуется зашифровать (спрятать)
	string fileToEncodeExtension; // Расширение файла, содержимое которого требуется закодировать (спрятать в стегоконтейнер)
	size_t fileToEncodeLength; // Размер файла, содержимое которого требуется закодировать, в байтах
	size_t containerLengthInBytes; // Длина стегоконтейнера (файла BMP) в байтах
	size_t neededBytesToFitEncodedMessage; // Количество байтов, нужное, чтобы вместить зашифрованный файл (сообщение) и служебную информацию контейнера
	size_t binaryInfoToEncodeLength; // Общая длина информации для кодирования
	bitset<BITS_IN_BYTE>* binaryInfoToEncode, *binaryInfoToEncodePtr; // Массив c информацией из файла и служебной информацией (расширение) для кодирования в двоичном виде
	bitset<BITS_IN_BYTE>* stegocontainer, *stegocontainerPtr; // Массив с байтами из BMP-файла (стегоконтейнера) в двоичном виде
	bitset<BITS_IN_BYTE * 4> messageToEncodeLength; // Длина кодируемого сообщения в битовом формате
	byte temp; // Временнная переменная для побайтового считывания

	cout << "Введите путь к файлу, содержимое которого требуется скрыть в BMP-изображении: ";
	cin >> fileToEncodePath;
	fstream textFile(fileToEncodePath, ios::binary | ios::in);
	if (!textFile.is_open()) return ERROR_FILE_INVALID;
	fileToEncodeExtension = getFileExtension(fileToEncodePath);

	fstream binaryFile(filePath, ios::binary | ios::in);
	containerLengthInBytes = getFileLength(binaryFile);
	fileToEncodeLength = getFileLength(textFile);
	// Если контейнер слишком мал (не сможет вместить зашифрованный файл) кидаем ошибку
	neededBytesToFitEncodedMessage = (fileToEncodeLength * (BITS_IN_BYTE / packingDegree + 1)) + MINIMUM_REQUIRED_FILE_LENGTH_IN_BYTES;
	if (neededBytesToFitEncodedMessage >= containerLengthInBytes) return ERROR_SMALL_CONTAINER;
	
	// Создаем массив наборов из восьми бит, причем сам массив имеет длину, равную количеству байт в контейнере
	stegocontainer = new bitset<8>[containerLengthInBytes], stegocontainerPtr = stegocontainer;
	// Считываем данные из двоичного файла побайтово и преобразуем байты в bitset из 8 битов
	for (size_t i = 0; i < containerLengthInBytes; i++) {
		binaryFile.read((char*)&temp, sizeof(temp));
		stegocontainer[i] = bitset<8>(temp);
	}
	binaryFile.close();
	
	// Пропускаем 54 бита, которые в BMP-файле предназначены под служебную информацию
	stegocontainerPtr += INFORMATION_BYTES_COUNT;

	/* Динамически создаем массив битсетов для всей информации в бинарном формате, которую будем записывать в стегоконтейнер.
	В нее входит 4 байта информации о длине сообщения, 16 байтов информации о расширении кодируемого файла и количество
	символов в самом кодируемом файле (то есть, используя char, количество байтов)*/
	binaryInfoToEncodeLength = MESSAGE_LENGTH_INFORMATION_BITS_COUNT / BITS_IN_BYTE + fileToEncodeLength + MAX_EXTENSION_LENGTH;
	binaryInfoToEncode = new bitset<8>[binaryInfoToEncodeLength + 1];
	binaryInfoToEncodePtr = binaryInfoToEncode;

	/*Превращаем size_t значение длины (количества символов) в файле в bitset из 32 бит и перекидываем его в основной массив
	информации для кодирования */
	messageToEncodeLength = bitset<32>(fileToEncodeLength);
	for (size_t i = 0; i < MESSAGE_LENGTH_INFORMATION_BITS_COUNT; i++) {
		binaryInfoToEncode[i / 8][i % 8] = messageToEncodeLength[i];
	}
	// Так как будем пользоваться арифметикой указателей, показываем, что 4 ячейки уже заняли
	binaryInfoToEncodePtr += MESSAGE_LENGTH_INFORMATION_BITS_COUNT / BITS_IN_BYTE;

	/*Теперь записываем расширение исходного файла, который прячем (кодируем). Расширение будет писаться в начало 
	выделяемых на это 16 бит, в конце пойдут нули, которые в итоге будут преобразовываться в символы конца строки*/
	for (size_t i = 0; i < MAX_EXTENSION_LENGTH; i++) {
		if (i < fileToEncodeExtension.length()) (*binaryInfoToEncodePtr++) = bitset<8>(fileToEncodeExtension[i]);
		else (*binaryInfoToEncodePtr++) = bitset<8>((unsigned char) 0);
	}

	// И наконец записываем сам текст, читая по одной букве (байту) и преобразуя каждый байт в bitset
	for (size_t i = 0; i < fileToEncodeLength; i++) {
		textFile.read((char*)&temp, sizeof(temp));
		*binaryInfoToEncodePtr++ = bitset<8>(temp);
	}

	for (size_t i = 0; i < binaryInfoToEncodeLength;) {

	}

	return ERROR_SUCCESS;
}