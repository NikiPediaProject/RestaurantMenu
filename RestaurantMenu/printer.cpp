#include "printer.h"

// Выводит все блюда меню с статистикой
void MenuPrinter::printAllDishes(const std::vector<Dish>& dishes, int invalidCount) const {
	std::cout << std::endl << "=== МЕНЮ РЕСТОРАНА ===" << std::endl;
	printTableHeader();

	// Вывод нумерованного списка блюд
	for (size_t i = 0; i < dishes.size(); ++i) {
		std::cout << std::setw(5) << std::right << (i + 1) << ". " << dishes[i] << std::endl;
	}

	// Вывод статистики
	std::cout << std::endl << "СТАТИСТИКА:" << std::endl;
	std::cout << "  Отсеяно невалидных блюд: " << invalidCount << std::endl;
	std::cout << "  Валидных блюд: " << dishes.size() << std::endl;
}

// Выводит отфильтрованный список блюд
void MenuPrinter::printFilteredDishes(const std::vector<Dish>& dishes, const std::string& title) const {
	if (dishes.empty()) {
		std::cout << std::endl << "Нет блюд, соответствующих критериям." << std::endl;
		return;
	}

	std::cout << std::endl << title << std::endl;
	printTableHeader();

	// Вывод нумерованного списка отфильтрованных блюд
	for (size_t i = 0; i < dishes.size(); ++i) {
		std::cout << std::setw(5) << std::right << (i + 1) << ". " << dishes[i] << std::endl;
	}
}

// Выводит заголовок таблицы меню
void MenuPrinter::printTableHeader() const {
	// Заголовок таблицы с выравниванием
	std::cout << std::setw(5) << std::right << "№"
		<< " "
		<< std::setw(25) << std::left << "Название блюда"
		<< std::setw(10) << std::left << "Цена"
		<< "Время приготовления" << std::endl;

	// Разделительная линия
	std::cout << std::string(60, '-') << std::endl;
}