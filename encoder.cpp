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

	fstream binaryFileIn(filePath, ios::binary | ios::in); // Файл BMP-контейнера, откуда будем считывать информацию

	cout << "Введите путь к файлу, содержимое которого требуется скрыть в BMP-изображении: ";
	cin >> fileToEncodePath;
	fstream textFile(fileToEncodePath, ios::binary | ios::in);
	if (!textFile.is_open()) return ERROR_FILE_INVALID;
	fileToEncodeExtension = getFileExtension(fileToEncodePath);

	containerLengthInBytes = getFileLength(binaryFileIn);
	fileToEncodeLength = getFileLength(textFile);
	// Если контейнер слишком мал (не сможет вместить зашифрованный файл) кидаем ошибку
	neededBytesToFitEncodedMessage = (fileToEncodeLength * (BITS_IN_BYTE / packingDegree + 1)) + MINIMUM_REQUIRED_FILE_LENGTH_IN_BYTES;
	if (neededBytesToFitEncodedMessage >= containerLengthInBytes) return ERROR_SMALL_CONTAINER;
	
	// Создаем массив наборов из восьми бит, причем сам массив имеет длину, равную количеству байт в контейнере
	stegocontainer = new bitset<8>[containerLengthInBytes], stegocontainerPtr = stegocontainer;
	// Считываем данные из двоичного файла побайтово и преобразуем байты в bitset из 8 битов
	for (size_t i = 0; i < containerLengthInBytes; i++) {
		binaryFileIn.read((char*)&temp, sizeof(temp));
		stegocontainer[i] = bitset<8>(temp);
	}
	binaryFileIn.close();
	
	// Пропускаем 54 бита, которые в BMP-файле предназначены под служебную информацию
	stegocontainerPtr += INFORMATION_BYTES_COUNT;

	/* Динамически создаем массив битсетов для всей информации в бинарном формате, которую будем записывать в стегоконтейнер.
	В нее входит 4 байта информации о длине сообщения, 16 байтов информации о расширении кодируемого файла и количество
	символов в самом кодируемом файле (то есть, используя char, количество байтов)*/
	binaryInfoToEncodeLength = MESSAGE_LENGTH_INFORMATION_BITS_COUNT / BITS_IN_BYTE + fileToEncodeLength + MAX_EXTENSION_LENGTH;
	binaryInfoToEncode = new bitset<8>[binaryInfoToEncodeLength + 1];
	binaryInfoToEncodePtr = binaryInfoToEncode;

	// Делаем копию, поскольку будем двигать биты и поломаем длину файла
	size_t fileToEncodeLengthCopy = fileToEncodeLength;
	/*Идем по числу из 32 бит (size_t), беря по 8 бит за один раз, от младших бит к старшим, однако
	записываем их в массив в обратном порядке. Получается, в итоге сначала идут старшие биты, а затем младшие*/
	for(int i = MESSAGE_LENGTH_INFORMATION_BITS_COUNT / BITS_IN_BYTE - 1; i > 0 ; i--) {
		// Берем 8 младших бит и кидаем их в битсет, а битсет переносим в массив по индексу, причем так, что индексы старших будут раньше
		binaryInfoToEncode[i] = bitset<8>(static_cast<uint8_t>(fileToEncodeLengthCopy));
		// Удаляем 8 младших бит
		fileToEncodeLengthCopy >>= 8;
	}
	// Сдвигаем указатель, так как дальше будем идти по нему, на количество заполненных элементов
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

	// Закрываем файл, закончив работу с ним
	textFile.close();


	// Вносим результат в младшие биты байтов стегоконтейнера
	for (size_t i = 0; i < binaryInfoToEncodeLength; i++) {
		// Идем по битам в каждом битсете на 8 битов из binaryInfoToEncode
		for (int j = BITS_IN_BYTE - 1; j >= 0; ) {
			// Заменяем в каждом байте стегоконтейнера количество младших бит, равное степени упаковки, на биты из текста для кодирования
			for (size_t k = 0; k < packingDegree; k++) {
				/* Вставляем в текущий элемент стегоконтейнера (bitset<8>) на место k (считается с младших бит)
				бит под номером j из битсета (тоже bitset<8>) под номером i в тексте для кодирования. Затем уменьшаем j, то есть берем
				более младший бит из того же битсета информации для кодирования, и так делаем, пока не дойдем до нулевого бита в
				binaryInfoToEncode[i]. Тогда увеличиваем i на единицу и продолжаем. */
				(*stegocontainerPtr).set(k, binaryInfoToEncode[i].test(j--));
			}
			/* После того, как мы заменили k младших бит в текущем битсете (bitset<8>) стегоконтейнера, переходим
			* к следующему битсету, увеличивая указатель (то есть, на следующей итерации будем работать уже с новым битсетом) */
			stegocontainerPtr++;
		}
	}

	// Создаем поток на вывод, очищаем файл
	ofstream binaryFileOut(filePath, ios::binary | ios::trunc);
	if (!binaryFileOut.is_open()) {
		cout << "Невозможно открыть файл на вывод информации." << endl;
		return ERROR_FILE_INVALID;
	}

	// Идем циклом по всему стегоконтейнеру
	for (size_t i = 0; i < containerLengthInBytes; i++) {
		// Выводим побитово измененные данные из стегоконтейнера в файл
		binaryFileOut << (byte) stegocontainer[i].to_ulong();
	}
	// Закрываем файл
	binaryFileOut.close();

	// Проверяем, получили ли мы валидную картинку после записи скрытого сообщения
	if (!isFileValid(filePath)) {
		cout << "Измененный стегоконтейнер не является валидным BMP-файлом." << endl;
		return INVALID_FILE_SIZE;
	}

	return ERROR_SUCCESS;
}