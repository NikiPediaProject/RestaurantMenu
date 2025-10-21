#include "builder.h"

// Конструктор билдера
RestaurantMenuAppBuilder::RestaurantMenuAppBuilder() {
	// Инициализация компонентами по умолчанию
	storage_ = std::make_unique<MenuStorage>();
	sorter_ = std::make_unique<MenuSorter>();
	filter_ = std::make_unique<MenuFilter>(std::make_unique<MenuSorter>());
	printer_ = std::make_unique<MenuPrinter>();
	fileParser_ = std::make_unique<MenuFileParser>();
	userInputParser_ = std::make_unique<UserInputParser>();
}

// Методы установки компонентов
RestaurantMenuAppBuilder& RestaurantMenuAppBuilder::setStorage(std::unique_ptr<IMenuStorage> storage) {
	storage_ = std::move(storage);
	return *this;
}

RestaurantMenuAppBuilder& RestaurantMenuAppBuilder::setSorter(std::unique_ptr<IMenuSorter> sorter) {
	sorter_ = std::move(sorter);
	return *this;
}

RestaurantMenuAppBuilder& RestaurantMenuAppBuilder::setFilter(std::unique_ptr<IMenuFilter> filter) {
	filter_ = std::move(filter);
	return *this;
}

RestaurantMenuAppBuilder& RestaurantMenuAppBuilder::setPrinter(std::unique_ptr<IMenuPrinter> printer) {
	printer_ = std::move(printer);
	return *this;
}

RestaurantMenuAppBuilder& RestaurantMenuAppBuilder::setFileParser(std::unique_ptr<IFileParser> fileParser) {
	fileParser_ = std::move(fileParser);
	return *this;
}

RestaurantMenuAppBuilder& RestaurantMenuAppBuilder::setUserInputParser(std::unique_ptr<UserInputParser> userInputParser) {
	userInputParser_ = std::move(userInputParser);
	return *this;
}

// Создание приложения
std::unique_ptr<RestaurantMenuApp> RestaurantMenuAppBuilder::build() {
	return std::make_unique<RestaurantMenuApp>(
		std::move(storage_),
		std::move(sorter_),
		std::move(filter_),
		std::move(printer_),
		std::move(fileParser_),
		std::move(userInputParser_)
		);
}

// Создание приложения с настройками по умолчанию
std::unique_ptr<RestaurantMenuApp> RestaurantMenuAppBuilder::createDefault() {
	return RestaurantMenuAppBuilder().build();
}