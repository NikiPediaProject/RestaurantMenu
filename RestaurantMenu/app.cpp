#include "app.h"
#include "file_parser.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <memory>
#include <string>

// ==================== РЕАЛИЗАЦИЯ RESTAURANT MENU APP ====================

/// Конструктор с инъекцией зависимостей (Dependency Injection pattern)
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

/// Загружает меню из файла, парсит данные и автоматически сортирует по алфавиту
void RestaurantMenuApp::loadMenu(const std::string& filename) {
	fileParser_->parseFile(filename, *storage_, invalidCount_);
	sorter_->sortAlphabetically(const_cast<std::vector<Dish>&>(storage_->getDishes()));
}

/// Выводит полное меню ресторана в консоль в табличном формате
void RestaurantMenuApp::printMenu() const {
	printer_->printAllDishes(storage_->getDishes(), invalidCount_);
}

/// Запускает основной интерактивный цикл управления меню
void RestaurantMenuApp::runInteractive() {
	std::string input;
	std::cout << std::endl << "=== РЕЖИМ УПРАВЛЕНИЯ МЕНЮ ===" << std::endl;
	showHelp();

	// Основной цикл обработки команд
	while (true) {
		std::cout << std::endl << "Введите команду: ";
		std::getline(std::cin, input);

		if (input == "exit") break;

		processCommand(input);
	}
}

/// Обрабатывает одну команду пользователя (маршрутизатор команд)
void RestaurantMenuApp::processCommand(const std::string& command) {
	if (command.empty()) return;

	// Проверяем специальные команды управления
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
		// Если это не команда управления, пытаемся обработать как критерии фильтрации
		processUserInput(command);
	}
}

/// Обрабатывает пользовательский ввод как критерии фильтрации меню
void RestaurantMenuApp::processUserInput(const std::string& input) {
	double price = 0.0;
	Time time;

	if (userInputParser_->parse(input, price, time)) {
		if (price > 0 && time.totalMinutes() > 0) {
			// Фильтрация одновременно по цене и времени
			auto filtered = filter_->filterByPriceAndTime(storage_->getDishes(), price, time);
			std::stringstream priceStream;
			priceStream << std::fixed << std::setprecision(2) << price;
			printer_->printFilteredDishes(filtered,
				"Блюда дешевле " + priceStream.str() +
				" и готовящиеся быстрее " + time.toString());
		}
		else if (price > 0) {
			// Фильтрация только по цене
			auto filtered = filter_->filterByPrice(storage_->getDishes(), price);
			std::stringstream priceStream;
			priceStream << std::fixed << std::setprecision(2) << price;
			printer_->printFilteredDishes(filtered, "Блюда дешевле " + priceStream.str());
		}
		else if (time.totalMinutes() > 0) {
			// Фильтрация только по времени
			auto filtered = filter_->filterByTime(storage_->getDishes(), time);
			printer_->printFilteredDishes(filtered, "Блюда, готовящиеся быстрее " + time.toString());
		}
		else {
			std::cout << "Некорректные критерии фильтрации!" << std::endl;
		}
	}
	else {
		std::cout << "Неизвестная команда или некорректный ввод! Введите 'help' для справки." << std::endl;
	}
}

/// Добавляет новое блюдо в меню с валидацией данных
void RestaurantMenuApp::addDish(const std::string& dishData) {
	// Создаем временный парсер для извлечения значений из строки
	MenuFileParser parser;
	std::string name;
	double price = 0.0;
	Time time;

	if (parser.extractValuesWithRegex(dishData, name, price, time)) {
		if (price <= 0) {
			std::cout << "Ошибка: цена должна быть положительной!" << std::endl;
			return;
		}

		// Передаём полный объект Time в хранилище
		storage_->addDish(name, price, time);
		std::cout << "Блюдо добавлено: " << name << " (цена: " << price << ", время: " << time.toString() << ")" << std::endl;

		// Пересортируем меню для поддержания порядка
		sorter_->sortAlphabetically(const_cast<std::vector<Dish>&>(storage_->getDishes()));
	}
	else {
		std::cout << "Ошибка: неверный формат данных! Используйте: \"Название блюда\" цена время" << std::endl;
		std::cout << "Пример: add \"Пицца Маргарита\" 12.50 00:30" << std::endl;
	}
}

/// Удаляет блюдо из меню (по названию или полному совпадению)
void RestaurantMenuApp::deleteDish(const std::string& dishData) {
	std::string trimmedData = dishData;

	// Проверяем, есть ли кавычки в данных для определения стратегии удаления
	if (dishData.find('"') == std::string::npos) {
		// Если кавычек нет - удаляем все блюда с таким названием
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
			storage_->addDish(dish.name, dish.price, dish.time);
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
		// Если есть кавычки - используем точное удаление по всем параметрам
		MenuFileParser parser;
		std::string name;
		double price = 0.0;
		Time time;

		if (parser.extractValuesWithRegex(dishData, name, price, time)) {
			if (storage_->removeDish(name, price, time)) {
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

/// Сохраняет текущее меню в файл с подтверждением перезаписи
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

/// Выводит справочную информацию по доступным командам
void RestaurantMenuApp::showHelp() const {
	std::cout << "Доступные команды:" << std::endl;
	std::cout << "  add \"Название\" цена время    - добавить блюдо" << std::endl;
	std::cout << "  delete Название              - удалить все блюда с таким названием" << std::endl;
	std::cout << "  delete \"Название\" цена время - удалить конкретное блюдо" << std::endl;
	std::cout << "  save имя_файла               - сохранить меню в файл" << std::endl;
	std::cout << "  print                        - вывести меню" << std::endl;
	std::cout << "  help                         - показать эту справку" << std::endl;
	std::cout << "  exit                         - выйти из программы" << std::endl;
	std::cout << "  цена [время]                 - фильтрация по цене и/или времени" << std::endl;
}
