#include "app.h"
#include "file_parser.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cstdlib> // Для system()

// Конструктор с инъекцией зависимостей
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
	, invalidCount_(0) {
	initializeCommands();
}

/// Инициализирует карту команд приложения
void RestaurantMenuApp::initializeCommands() {
	// Регистрация команд без аргументов
	commandsMap_["help"] = [this](const std::string&) { cmdHelp(); };
	commandsMap_["print"] = [this](const std::string&) { cmdPrint(); };
	commandsMap_["clear"] = [this](const std::string&) { cmdClear(); };

	// Регистрация команд с аргументами
	commandsMap_["add"] = [this](const std::string& args) { cmdAdd(args); };
	commandsMap_["delete"] = [this](const std::string& args) { cmdDelete(args); };
	commandsMap_["save"] = [this](const std::string& args) { cmdSave(args); };
}

/// Обработчик команды help
void RestaurantMenuApp::cmdHelp(const std::string&) {
	showHelp();
}

/// Обработчик команды print
void RestaurantMenuApp::cmdPrint(const std::string&) {
	printMenu();
}

/// Обработчик команды clear
void RestaurantMenuApp::cmdClear(const std::string&) {
	clearConsole();
}

/// Обработчик команды add
void RestaurantMenuApp::cmdAdd(const std::string& args) {
	addDish(args);
}

/// Обработчик команды delete
void RestaurantMenuApp::cmdDelete(const std::string& args) {
	deleteDish(args);
}

/// Обработчик команды save
void RestaurantMenuApp::cmdSave(const std::string& args) {
	saveMenu(args);
}

// Загружает меню из указанного файла
void RestaurantMenuApp::loadMenu(const std::string& filename) {
	// Очищаем предыдущее меню перед загрузкой нового
	clearMenu();
	fileParser_->parseFile(filename, *storage_, invalidCount_);
	sorter_->sortAlphabetically(const_cast<std::vector<Dish>&>(storage_->getDishes()));
}

// Очищает текущее загруженное меню
void RestaurantMenuApp::clearMenu() {
	storage_->clear();
	invalidCount_ = 0;
}

// Выводит полное меню в консоль
void RestaurantMenuApp::printMenu() const {
	printer_->printAllDishes(storage_->getDishes(), invalidCount_);
}

// Запускает интерактивный режим управления меню
void RestaurantMenuApp::runInteractive() {
	std::string input;
	std::cout << std::endl << "=== ИНТЕРАКТИВНЫЙ РЕЖИМ УПРАВЛЕНИЯ ===" << std::endl;
	std::cout << "Для возврата к выбору файла введите 'exit'" << std::endl;
	showHelp();

	while (true) {
		std::cout << std::endl << "Введите команду: ";
		std::getline(std::cin, input);

		if (input == "exit") {
			std::cout << "Выход из интерактивного режима..." << std::endl;
			break;
		}

		processCommand(input);
	}
}

// Обрабатывает введенную пользователем команду
void RestaurantMenuApp::processCommand(const std::string& command) {
	if (command.empty()) return;

	std::istringstream iss(command);
	std::string cmd;
	iss >> cmd;

	// Извлекаем аргументы команды
	std::string args;
	std::getline(iss, args);

	// Удаляем ведущий пробел, если есть
	if (!args.empty() && args[0] == ' ') {
		args.erase(0, 1);
	}

	// Ищем команду в карте
	auto it = commandsMap_.find(cmd);
	if (it != commandsMap_.end()) {
		// Выполняем команду через std::function
		it->second(args);
		return;
	}

	// Если команда не найдена, пытаемся обработать как критерии фильтрации
	processUserInput(command);
}

// Обрабатывает пользовательский ввод для фильтрации меню
void RestaurantMenuApp::processUserInput(const std::string& input) {
	double price = 0.0;
	Time time;
	std::string errorMessage;

	if (userInputParser_->parse(input, price, time, errorMessage)) {
		if (price > 0 && time.totalMinutes() > 0) {
			// Комбинированная фильтрация по цене и времени
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
		std::cout << "ОШИБКА: " << errorMessage << std::endl;
		std::cout << "Неизвестная команда или некорректный ввод! Введите 'help' для справки." << std::endl;
	}
}

// Добавляет новое блюдо в меню
void RestaurantMenuApp::addDish(const std::string& dishData) {
	// Создаем временный парсер для извлечения значений
	MenuFileParser parser;
	std::string name;
	double price = 0.0;
	Time time;

	if (parser.extractValuesWithRegex(dishData, name, price, time)) {
		// Дополнительная проверка: название не должно быть пустым (двойная проверка для надежности)
		if (name.empty()) {
			std::cout << "ERROR: название блюда не может быть пустым!" << std::endl;
			return;
		}

		if (price <= 0) {
			std::cout << "ERROR: цена должна быть положительной!" << std::endl;
			return;
		}

		// Передаём полный объект Time
		storage_->addDish(name, price, time);
		std::cout << "Блюдо добавлено: " << name << " (цена: " << price << ", время: " << time.toString() << ")" << std::endl;

		// Пересортируем меню по алфавиту
		sorter_->sortAlphabetically(const_cast<std::vector<Dish>&>(storage_->getDishes()));
	}
	else {
		std::cout << "ERROR: неверный формат данных! Используйте: \"Название блюда\" цена время" << std::endl;
		std::cout << "Пример: add \"Пицца Маргарита\" 12.50 00:30" << std::endl;
	}
}

// Удаляет блюдо из меню
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
			storage_->addDish(dish.name, dish.price, dish.time);
		}

		size_t newCount = storage_->getDishesCount();
		if (newCount < removedCount) {
			std::cout << "Удалено блюд с названием \"" << name << "\": " << (removedCount - newCount) << std::endl;
		}
		else {
			std::cout << "WARNING: Блюда с названием \"" << name << "\" не найдены" << std::endl;
		}
	}
	else {
		// Если есть кавычки, используем полный парсинг
		MenuFileParser parser;
		std::string name;
		double price = 0.0;
		Time time;

		if (parser.extractValuesWithRegex(dishData, name, price, time)) {
			if (storage_->removeDish(name, price, time)) {
				std::cout << "Блюдо удалено: " << name << std::endl;
			}
			else {
				std::cout << "WARNING: Блюдо не найдено: " << name << std::endl;
			}
		}
		else {
			std::cout << "ERROR: неверный формат данных! Используйте: delete <Название блюда> или delete \"Название\" цена время" << std::endl;
		}
	}
}

// Сохраняет текущее меню в файл
void RestaurantMenuApp::saveMenu(const std::string& filename) const {
	if (filename == "menu.txt") {
		std::cout << "WARNING: вы пытаетесь сохранить в файл menu.txt. Это перезапишет исходный файл меню." << std::endl;
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
		std::cout << "ERROR: Ошибка при сохранении: " << e.what() << std::endl;
	}
}

// Очищает консоль
void RestaurantMenuApp::clearConsole() const {
	system("cls");
}

// Показывает справку по доступным командам
void RestaurantMenuApp::showHelp() const {
	std::cout << std::endl << "ДОСТУПНЫЕ КОМАНДЫ:" << std::endl;
	std::cout << "  help                         - показать эту справку" << std::endl;
	std::cout << "  print                        - вывести меню" << std::endl;
	std::cout << "  clear                        - очистить консоль" << std::endl;
	std::cout << "  add \"Название\" цена время    - добавить блюдо" << std::endl;
	std::cout << "  delete Название              - удалить все блюда с таким названием" << std::endl;
	std::cout << "  delete \"Название\" цена время - удалить конкретное блюдо" << std::endl;
	std::cout << "  save имя_файла               - сохранить меню в файл" << std::endl;
	std::cout << "  exit                         - выйти в меню выбора файла" << std::endl;
	std::cout << std::endl << "ФИЛЬТРАЦИЯ:" << std::endl;
	std::cout << "  цена                         - блюда дешевле указанной суммы" << std::endl;
	std::cout << "  время (чч:мм)                - блюда, готовящиеся быстрее" << std::endl;
	std::cout << "  цена время                   - комбинированная фильтрация" << std::endl;
}