#include "app.h"

RestaurantMenuApp::RestaurantMenuApp(
	std::unique_ptr<IMenuStorage> storage,
	std::unique_ptr<IMenuSorter> sorter,
	std::unique_ptr<IMenuFilter> filter,
	std::unique_ptr<IMenuPrinter> printer,
	std::unique_ptr<IFileParser> fileParser,
	std::unique_ptr<UserInputParser> userInputParser)
	: storage_(std::move(storage))
	, sorter_(std::move(sorter))
	, filter_(std::move(filter))
	, printer_(std::move(printer))
	, fileParser_(std::move(fileParser))
	, userInputParser_(std::move(userInputParser))
	, invalidCount_(0) {}

void RestaurantMenuApp::loadMenu(const std::string& filename) {
	fileParser_->parseFile(filename, *storage_, invalidCount_);
	sorter_->sortAlphabetically(const_cast<std::vector<Dish>&>(storage_->getDishes()));
}

void RestaurantMenuApp::printMenu() const {
	printer_->printAllDishes(storage_->getDishes(), invalidCount_);
}

void RestaurantMenuApp::runInteractive() {
	std::string input;
	std::cout << "\n=== ФИЛЬТРАЦИЯ МЕНЮ ===" << std::endl;
	std::cout << "Введите критерии фильтрации:" << std::endl;
	std::cout << "- Число (цена) - блюда дешевле указанной суммы" << std::endl;
	std::cout << "- Время (чч:мм) - блюда, готовящиеся быстрее" << std::endl;
	std::cout << "- Число и время - комбинированная фильтрация" << std::endl;
	std::cout << "Введите 'exit' для выхода" << std::endl;

	while (true) {
		std::cout << "\nВведите критерии: ";
		std::getline(std::cin, input);

		if (input == "exit") break;
		if (input.empty()) continue;

		processUserInput(input);
	}
}

void RestaurantMenuApp::processUserInput(const std::string& input) {
	userInputParser_->parse(input);

	if (userInputParser_->hasPrice() && userInputParser_->hasTime()) {
		auto filtered = filter_->filterByPriceAndTime(
			storage_->getDishes(),
			userInputParser_->getPrice(),
			userInputParser_->getTime()
		);
		std::stringstream priceStream;
		priceStream << std::fixed << std::setprecision(2) << userInputParser_->getPrice();
		printer_->printFilteredDishes(filtered,
			"Блюда дешевле " + priceStream.str() +
			" и готовящиеся быстрее " + userInputParser_->getTime().toString());
	}
	else if (userInputParser_->hasPrice()) {
		auto filtered = filter_->filterByPrice(storage_->getDishes(), userInputParser_->getPrice());
		std::stringstream priceStream;
		priceStream << std::fixed << std::setprecision(2) << userInputParser_->getPrice();
		printer_->printFilteredDishes(filtered, "Блюда дешевле " + priceStream.str());
	}
	else if (userInputParser_->hasTime()) {
		auto filtered = filter_->filterByTime(storage_->getDishes(), userInputParser_->getTime());
		printer_->printFilteredDishes(filtered,
			"Блюда, готовящиеся быстрее " + userInputParser_->getTime().toString());
	}
	else {
		std::cout << "Некорректный ввод!" << std::endl;
	}
}