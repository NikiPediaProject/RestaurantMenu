#include "app.h"
#include "file_parser.h"
#include <iostream>
#include <sstream>
#include <algorithm>

// Конструктор
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
	std::cout << "\n=== РЕЖИМ УПРАВЛЕНИЯ МЕНЮ ===" << std::endl;
	showHelp();

	while (true) {
		std::cout << "\nВведите команду: ";
		std::getline(std::cin, input);

		if (input == "exit") break;

		processCommand(input);
	}
}

void RestaurantMenuApp::processCommand(const std::string& command) {
	if (command.empty()) return;

	// Проверяем специальные команды
	if (command == "help") {
		showHelp();
	}
	else if (command == "print") {
		printMenu();
	}
	else if (command.substr(0, 4) == "add ") {
		addDish(command.substr(4));
	}
	else if (command.substr(0, 7) == "delete ") {
		deleteDish(command.substr(7));
	}
	else if (command.substr(0, 5) == "save ") {
		saveMenu(command.substr(5));
	}
	else {
		// Если это не команда, пытаемся обработать как критерии фильтрации
		processUserInput(command);
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
		std::cout << "Неизвестная команда или некорректный ввод! Введите 'help' для справки." << std::endl;
	}
}

void RestaurantMenuApp::addDish(const std::string& dishData) {
	// Создаем временный парсер для извлечения значений
	MenuFileParser parser;
	std::string name;
	double price = 0.0;
	Time time;

	if (parser.extractValuesWithRegex(dishData, name, price, time)) {
		if (price <= 0) {
			std::cout << "Ошибка: цена должна быть положительной!" << std::endl;
			return;
		}

		Time normalizedTime;
		normalizedTime.setTime(time.hours, time.minutes);

		storage_->addDish(name, price, normalizedTime.hours, normalizedTime.minutes);
		std::cout << "Блюдо добавлено: " << name << " (цена: " << price << ", время: " << normalizedTime.toString() << ")" << std::endl;

		// Пересортируем меню
		sorter_->sortAlphabetically(const_cast<std::vector<Dish>&>(storage_->getDishes()));
	}
	else {
		std::cout << "Ошибка: неверный формат данных! Используйте: \"Название блюда\" цена время" << std::endl;
		std::cout << "Пример: add \"Пицца Маргарита\" 12.50 00:30" << std::endl;
	}
}

void RestaurantMenuApp::deleteDish(const std::string& dishData) {
	std::string trimmedData = dishData;

	// Проверяем, есть ли кавычки в данных
	if (dishData.find('"') == std::string::npos) {
		// Если кавычек нет, удаляем только по названию
		std::string name = dishData;
		size_t removedCount = storage_->getDishesCount();

		// Создаем временное хранилище для блюд, которые не нужно удалять
		std::vector<Dish> remainingDishes;
		for (const auto& dish : storage_->getDishes()) {
			if (dish.name != name) {
				remainingDishes.push_back(dish);
			}
		}

		// Очищаем основное хранилище и добавляем обратно только нужные блюда
		storage_->clear();
		for (const auto& dish : remainingDishes) {
			storage_->addDish(dish.name, dish.price, dish.time.hours, dish.time.minutes);
		}

		size_t newCount = storage_->getDishesCount();
		if (newCount < removedCount) {
			std::cout << "Удалено блюд с названием \"" << name << "\": " << (removedCount - newCount) << std::endl;
		}
		else {
			std::cout << "Блюда с названием \"" << name << "\" не найдены" << std::endl;
		}
	}
	else {
		// Если есть кавычки, используем полный парсинг
		MenuFileParser parser;
		std::string name;
		double price = 0.0;
		Time time;

		if (parser.extractValuesWithRegex(dishData, name, price, time)) {
			Time normalizedTime;
			normalizedTime.setTime(time.hours, time.minutes);

			if (storage_->removeDish(name, price, normalizedTime.hours, normalizedTime.minutes)) {
				std::cout << "Блюдо удалено: " << name << std::endl;
			}
			else {
				std::cout << "Блюдо не найдено: " << name << std::endl;
			}
		}
		else {
			std::cout << "Ошибка: неверный формат данных! Используйте: delete \"Название блюда\" или delete \"Название\" цена время" << std::endl;
		}
	}
}

void RestaurantMenuApp::saveMenu(const std::string& filename) const {
	if (filename == "menu.txt") {
		std::cout << "Внимание: вы пытаетесь сохранить в файл menu.txt. Это перезапишет исходный файл меню." << std::endl;
		std::cout << "Продолжить? (y/n): ";

		std::string answer;
		std::getline(std::cin, answer);

		if (answer != "y" && answer != "Y") {
			std::cout << "Сохранение отменено." << std::endl;
			return;
		}
	}

	try {
		storage_->saveToFile(filename);
		std::cout << "Меню успешно сохранено в файл: " << filename << std::endl;
	}
	catch (const std::exception& e) {
		std::cout << "Ошибка при сохранении: " << e.what() << std::endl;
	}
}

void RestaurantMenuApp::showHelp() const {
	std::cout << "Доступные команды:" << std::endl;
	std::cout << "  add \"Название\" цена время    - добавить блюдо" << std::endl;
	std::cout << "  delete Название                - удалить все блюда с таким названием" << std::endl;
	std::cout << "  delete \"Название\" цена время - удалить конкретное блюдо" << std::endl;
	std::cout << "  save имя_файла               - сохранить меню в файл" << std::endl;
	std::cout << "  print                        - вывести меню" << std::endl;
	std::cout << "  help                         - показать эту справку" << std::endl;
	std::cout << "  exit                         - выйти из программы" << std::endl;
	std::cout << "  цена / время                 - фильтрация по цене и/или времени" << std::endl;
}