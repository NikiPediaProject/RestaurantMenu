#include "app.h"
#include "builder.h"
#include <iostream>
#include <locale>
#include <windows.h>

/// Устанавливает русскую локаль и кодовые страницы для корректного вывода кириллицы
void setRussianLocale() {
	// Устанавливаем локаль для консоли для поддержки русского языка
	setlocale(LC_ALL, "Russian");

	// Для Windows дополнительно устанавливаем кодовые страницы консоли
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
}

/// Точка входа в приложение - основной управляющий метод
int main() {
	// Настраиваем локаль для поддержки русского языка
	setRussianLocale();

	try {
		// Используем билдер для создания приложения с компонентами по умолчанию
		auto app = RestaurantMenuAppBuilder::createDefault();

		// Загружаем начальное меню из файла
		app->loadMenu("menu.txt");

		// Выводим загруженное меню для проверки
		app->printMenu();

		// Запускаем основной интерактивный цикл приложения
		app->runInteractive();
	}
	catch (const std::exception& e) {
		// Обработка критических ошибок с выводом сообщения
		std::cout << "Критическая ошибка при работе приложения: " << e.what() << std::endl;
		return 1;
	}

	// Успешное завершение программы
	return 0;
}