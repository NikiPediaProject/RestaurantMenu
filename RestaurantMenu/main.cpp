#include "app.h"
#include "builder.h"
#include <iostream>
#include <locale>
#include <windows.h>

/// Устанавливает русскую локаль для корректного отображения кириллицы
void setRussianLocale() {
	// Устанавливаем локаль для всех категорий на русскую
	setlocale(LC_ALL, "Russian");
	// Устанавливаем кодовую страницу ввода консоли на Windows-1251
	SetConsoleCP(1251);
	// Устанавливаем кодовую страницу вывода консоли на Windows-1251
	SetConsoleOutputCP(1251);
}

/// Очищает консоль с помощью системной команды
void clearConsole() {
	system("cls");
}

/// Показывает справку по командам уровня выбора файла
void showFileLevelHelp() {
	std::cout << std::endl << "ДОСТУПНЫЕ КОМАНДЫ НА УРОВНЕ ВЫБОРА ФАЙЛА:" << std::endl;
	std::cout << "  /help             - показать эту справку" << std::endl;
	std::cout << "  /clear            - очистить консоль" << std::endl;
	std::cout << "  <имя_файла>       - загрузить указанный файл с меню" << std::endl;
	std::cout << "  /stop             - завершить работу программы" << std::endl;
	std::cout << std::endl;
}

/// Основной цикл работы с файлами меню
void runFileLoop() {
	// Создаем экземпляр приложения с настройками по умолчанию
	auto app = RestaurantMenuAppBuilder::createDefault();
	// Переменная для хранения пользовательского ввода
	std::string input;

	std::cout << "=== РЕСТОРАННОЕ МЕНЮ - СИСТЕМА УПРАВЛЕНИЯ ===" << std::endl;
	// Показываем справку при старте
	showFileLevelHelp();

	// Основной бесконечный цикл обработки команд
	while (true) {
		std::cout << "Введите команду или имя файла: ";
		// Считываем всю строку ввода
		std::getline(std::cin, input);

		// Пропускаем пустой ввод
		if (input.empty()) {
			continue;
		}

		// Обработка команд, начинающихся с /
		if (input[0] == '/') {
			if (input == "/help") {
				// Показываем справку
				showFileLevelHelp();
			}
			else if (input == "/clear") {
				// Очищаем консоль
				clearConsole();
			}
			else if (input == "/stop") {
				std::cout << "Завершение работы программы." << std::endl;
				// Выходим из цикла и завершаем программу
				break;
			}
			else {
				std::cout << "Неизвестная команда. Введите /help для справки." << std::endl;
			}
			// Переходим к следующей итерации цикла
			continue;
		}

		// Если ввод не команда, то считаем его именем файла
		try {
			std::cout << std::endl << "Загрузка файла: " << input << std::endl;
			// Загружаем меню из указанного файла
			app->loadMenu(input);
			// Выводим загруженное меню
			app->printMenu();
			// Запускаем интерактивный режим работы с меню
			app->runInteractive();

			// Очищаем меню после выхода из интерактивного режима
			app->clearMenu();
			std::cout << std::endl << "Файл закрыт. Можно загрузить новый файл." << std::endl;
			std::cout << std::string(50, '=') << std::endl;
		}
		catch (const std::exception& e) {
			// Обрабатываем ошибки при загрузке файла
			std::cout << std::endl << "ОШИБКА: Не удалось обработать файл '" << input << "'" << std::endl;
			std::cout << "Причина: " << e.what() << std::endl;
			std::cout << "Попробуйте другой файл." << std::endl;
		}
	}
}

/// Главная функция - точка входа в программу
int main() {
	// Устанавливаем русскую локаль для корректного отображения текста
	setRussianLocale();

	try {
		// Запускаем основной цикл работы программы
		runFileLoop();
	}
	catch (const std::exception& e) {
		// Обрабатываем критические ошибки
		std::cout << std::endl << "КРИТИЧЕСКАЯ ОШИБКА: " << e.what() << std::endl;
		// Возвращаем код ошибки
		return 1;
	}

	std::cout << std::endl << "Программа завершена. ББ ГГ ВП" << std::endl;
	// Успешное завершение программы
	return 0;
}