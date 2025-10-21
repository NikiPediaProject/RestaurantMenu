#include "app.h"
#include "builder.h"
#include <iostream>
#include <locale>

/// Устанавливает русскую локаль для корректного вывода
void setRussianLocale() {
	setlocale(LC_ALL, "Russian");
}

/// Точка входа в приложение
int main() {
	setRussianLocale();

	try {
		// Используем билдер вместо фабрики
		auto app = RestaurantMenuAppBuilder::createDefault();

		// Альтернативный вариант с кастомными настройками:
		// auto app = RestaurantMenuAppBuilder()
		//     .setPrinter(std::make_unique<CustomMenuPrinter>()) // если бы был кастомный принтер
		//     .build();

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