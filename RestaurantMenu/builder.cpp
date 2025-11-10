#include "builder.h"
#include "storage.h"
#include "parsers.h"
#include "printer.h"
#include "file_parser.h"
#include <sstream>
#include <iostream>

// ==================== РЕАЛИЗАЦИЯ RESTAURANT MENU APP BUILDER ====================

/// Конструктор билдера - инициализирует все компоненты значениями по умолчанию
RestaurantMenuAppBuilder::RestaurantMenuAppBuilder() {
	std::stringstream log;
	log << "Builder: Инициализация компонентов по умолчанию..." << std::endl;
	std::cout << log.str();

	// Инициализация всеми компонентами по умолчанию для быстрого старта
	storage_ = std::make_unique<MenuStorage>();
	log.str(""); log << "   MenuStorage создан" << std::endl; std::cout << log.str();

	sorter_ = std::make_unique<MenuSorter>();
	log.str(""); log << "   MenuSorter создан" << std::endl; std::cout << log.str();

	filter_ = std::make_unique<MenuFilter>(std::make_unique<MenuSorter>());
	log.str(""); log << "   MenuFilter создан" << std::endl; std::cout << log.str();

	printer_ = std::make_unique<MenuPrinter>();
	log.str(""); log << "   MenuPrinter создан" << std::endl; std::cout << log.str();

	fileParser_ = std::make_unique<MenuFileParser>();
	log.str(""); log << "   MenuFileParser создан" << std::endl; std::cout << log.str();

	userInputParser_ = std::make_unique<UserInputParser>();
	log.str(""); log << "   UserInputParser создан" << std::endl; std::cout << log.str();

	log.str(""); log << "Все компоненты успешно инициализированы!" << std::endl; std::cout << log.str();
}

/// Устанавливает пользовательскую реализацию хранилища меню
RestaurantMenuAppBuilder& RestaurantMenuAppBuilder::setStorage(std::unique_ptr<IMenuStorage> storage) {
	std::stringstream log;
	log << "Builder: Установка пользовательского хранилища" << std::endl;
	std::cout << log.str();

	storage_ = std::move(storage);
	return *this;
}

/// Устанавливает пользовательскую реализацию сортировщика меню
RestaurantMenuAppBuilder& RestaurantMenuAppBuilder::setSorter(std::unique_ptr<IMenuSorter> sorter) {
	std::stringstream log;
	log << "Builder: Установка пользовательского сортировщика" << std::endl;
	std::cout << log.str();

	sorter_ = std::move(sorter);
	return *this;
}

/// Устанавливает пользовательскую реализацию фильтра меню
RestaurantMenuAppBuilder& RestaurantMenuAppBuilder::setFilter(std::unique_ptr<IMenuFilter> filter) {
	std::stringstream log;
	log << "Builder: Установка пользовательского фильтра" << std::endl;
	std::cout << log.str();

	filter_ = std::move(filter);
	return *this;
}

/// Устанавливает пользовательскую реализацию принтера меню
RestaurantMenuAppBuilder& RestaurantMenuAppBuilder::setPrinter(std::unique_ptr<IMenuPrinter> printer) {
	std::stringstream log;
	log << "Builder: Установка пользовательского принтера" << std::endl;
	std::cout << log.str();

	printer_ = std::move(printer);
	return *this;
}

/// Устанавливает пользовательскую реализацию парсера файлов
RestaurantMenuAppBuilder& RestaurantMenuAppBuilder::setFileParser(std::unique_ptr<IFileParser> fileParser) {
	std::stringstream log;
	log << "Builder: Установка пользовательского парсера файлов" << std::endl;
	std::cout << log.str();

	fileParser_ = std::move(fileParser);
	return *this;
}

/// Устанавливает пользовательскую реализацию парсера пользовательского ввода
RestaurantMenuAppBuilder& RestaurantMenuAppBuilder::setUserInputParser(std::unique_ptr<UserInputParser> userInputParser) {
	std::stringstream log;
	log << "Builder: Установка пользовательского парсера ввода" << std::endl;
	std::cout << log.str();

	userInputParser_ = std::move(userInputParser);
	return *this;
}

/// Финальный метод сборки - создает экземпляр приложения с текущей конфигурацией
std::unique_ptr<RestaurantMenuApp> RestaurantMenuAppBuilder::build() {
	std::stringstream log;
	log << "Builder: Создание приложения RestaurantMenuApp..." << std::endl;
	std::cout << log.str();

	// Создаем приложение, передавая владение всеми компонентами
	auto app = std::make_unique<RestaurantMenuApp>(
		std::move(storage_),
		std::move(sorter_),
		std::move(filter_),
		std::move(printer_),
		std::move(fileParser_),
		std::move(userInputParser_)
		);

	log.str(""); log << "Приложение RestaurantMenuApp успешно создано!" << std::endl; std::cout << log.str();
	return app;
}

/// Статический метод для быстрого создания приложения с настройками по умолчанию
std::unique_ptr<RestaurantMenuApp> RestaurantMenuAppBuilder::createDefault() {
	std::stringstream log;
	log << "Builder: Создание приложения с настройками по умолчанию" << std::endl;
	std::cout << log.str();

	return RestaurantMenuAppBuilder().build();
}