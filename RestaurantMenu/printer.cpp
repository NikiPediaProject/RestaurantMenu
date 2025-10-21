#include "printer.h"

void MenuPrinter::printAllDishes(const std::vector<Dish>& dishes, int invalidCount) const {
	std::cout << "\n=== МЕНЮ РЕСТОРАНА ===" << std::endl;
	printTableHeader();

	for (const auto& dish : dishes) {
		dish.print();
	}

	std::cout << "\nОтсеяно невалидных блюд: " << invalidCount << std::endl;
	std::cout << "Валидных блюд: " << dishes.size() << std::endl;
}

void MenuPrinter::printFilteredDishes(const std::vector<Dish>& dishes, const std::string& title) const {
	if (dishes.empty()) {
		std::cout << "\nНет блюд, соответствующих критериям." << std::endl;
		return;
	}

	std::cout << "\n" << title << std::endl;
	printTableHeader();

	for (const auto& dish : dishes) {
		dish.print();
	}
}

void MenuPrinter::printTableHeader() const {
	std::cout << std::left << std::setw(25) << "Название блюда"
		<< std::setw(10) << "Цена"
		<< "Время приготовления" << std::endl;
	std::cout << std::string(60, '-') << std::endl;
}