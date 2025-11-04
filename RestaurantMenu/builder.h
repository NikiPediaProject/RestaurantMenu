#pragma once
#ifndef BUILDER_H
#define BUILDER_H

#include "app.h"
#include "interfaces.h"
#include "storage.h"
#include "parsers.h"
#include "printer.h"
#include "file_parser.h"
#include <memory>

// Предварительные объявления
class MenuStorage;
class MenuSorter;
class MenuFilter;
class MenuPrinter;
class MenuFileParser;
class UserInputParser;

/// Билдер для пошагового создания приложения RestaurantMenuApp
class RestaurantMenuAppBuilder {
private:
	std::unique_ptr<IMenuStorage> storage_;
	std::unique_ptr<IMenuSorter> sorter_;
	std::unique_ptr<IMenuFilter> filter_;
	std::unique_ptr<IMenuPrinter> printer_;
	std::unique_ptr<IFileParser> fileParser_;
	std::unique_ptr<UserInputParser> userInputParser_;

public:
	RestaurantMenuAppBuilder();

	/// Устанавливает хранилище меню
	RestaurantMenuAppBuilder& setStorage(std::unique_ptr<IMenuStorage> storage);

	/// Устанавливает сортировщик меню
	RestaurantMenuAppBuilder& setSorter(std::unique_ptr<IMenuSorter> sorter);

	/// Устанавливает фильтр меню
	RestaurantMenuAppBuilder& setFilter(std::unique_ptr<IMenuFilter> filter);

	/// Устанавливает принтер меню
	RestaurantMenuAppBuilder& setPrinter(std::unique_ptr<IMenuPrinter> printer);

	/// Устанавливает парсер файлов
	RestaurantMenuAppBuilder& setFileParser(std::unique_ptr<IFileParser> fileParser);

	/// Устанавливает парсер пользовательского ввода
	RestaurantMenuAppBuilder& setUserInputParser(std::unique_ptr<UserInputParser> userInputParser);

	/// Создает и возвращает экземпляр приложения
	std::unique_ptr<RestaurantMenuApp> build();

	/// Создает приложение с настройками по умолчанию
	static std::unique_ptr<RestaurantMenuApp> createDefault();
};

#endif // BUILDER_H