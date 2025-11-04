#include "app.h"
#include "builder.h"
#include <iostream>
#include <locale>
#include <windows.h>

/// Устанавливает русскую локаль для корректного вывода
void setRussianLocale() {
	// Устанавливаем локаль для консоли
	setlocale(LC_ALL, "Russian");

	// Для Windows устанавливаем кодовую страницу консоли
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
}

/// Точка входа в приложение
int main() {
	setRussianLocale();

	try {
		// Используем билдер вместо фабрики
		auto app = RestaurantMenuAppBuilder::createDefault();

		app->loadMenu("menu.txt");
		app->printMenu();
		app->runInteractive();
	}
	catch (const std::exception& e) {
		std::cout << "Ошибка: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}