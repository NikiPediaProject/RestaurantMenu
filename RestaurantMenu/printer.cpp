#include "printer.h"

// ==================== РЕАЛИЗАЦИЯ MENU PRINTER ====================

/// Выводит все блюда меню в табличном формате с заголовком и статистикой
void MenuPrinter::printAllDishes(const std::vector<Dish>& dishes, int invalidCount) const {
	std::cout << std::endl << "=== МЕНЮ РЕСТОРАНА ===" << std::endl;
	printTableHeader();

	// Выводим все блюда с нумерацией
	for (size_t i = 0; i < dishes.size(); ++i) {
		std::cout << i + 1 << ". " << dishes[i] << std::endl;
	}

	// Выводим статистику по невалидным записям
	std::cout << std::endl << "Отсеяно невалидных блюд: " << invalidCount << std::endl;
	std::cout << "Валидных блюд: " << dishes.size() << std::endl;
}

/// Выводит отфильтрованный список блюд с пользовательским заголовком
void MenuPrinter::printFilteredDishes(const std::vector<Dish>& dishes, const std::string& title) const {
	if (dishes.empty()) {
		std::cout << std::endl << "Нет блюд, соответствующих критериям." << std::endl;
		return;
	}

	std::cout << std::endl << title << std::endl;
	printTableHeader();

	// Выводим отфильтрованные блюда с нумерацией
	for (size_t i = 0; i < dishes.size(); ++i) {
		std::cout << i + 1 << ". " << dishes[i] << std::endl;
	}
}

/// Выводит заголовок таблицы с выравниванием столбцов
void MenuPrinter::printTableHeader() const {
	// Форматируем заголовок таблицы с фиксированной шириной столбцов
	std::cout << std::left << std::setw(3) << "№"
		<< std::setw(25) << "Название блюда"
		<< std::setw(10) << "Цена"
		<< "Время приготовления" << std::endl;

	// Разделительная линия под заголовком
	std::cout << std::string(60, '-') << std::endl;
}