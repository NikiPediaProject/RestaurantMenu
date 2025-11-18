#pragma once
#ifndef BUILDER_H
#define BUILDER_H

#include "app.h"
#include <memory>

/// Строитель для пошагового создания приложения RestaurantMenuApp
class RestaurantMenuAppBuilder {
private:
	// Компоненты для сборки приложения
	std::unique_ptr<IMenuStorage> storage_;             ///< Хранилище данных
	std::unique_ptr<IMenuSorter> sorter_;               ///< Сортировщик
	std::unique_ptr<IMenuFilter> filter_;               ///< Фильтр
	std::unique_ptr<IMenuPrinter> printer_;             ///< Принтер
	std::unique_ptr<IFileParser> fileParser_;           ///< Парсер файлов
	std::unique_ptr<UserInputParser> userInputParser_;  ///< Парсер пользовательского ввода

public:
	RestaurantMenuAppBuilder();

	/// Устанавливает пользовательское хранилище меню
	RestaurantMenuAppBuilder& setStorage(std::unique_ptr<IMenuStorage> storage);

	/// Устанавливает пользовательский сортировщик меню
	RestaurantMenuAppBuilder& setSorter(std::unique_ptr<IMenuSorter> sorter);

	/// Устанавливает пользовательский фильтр меню
	RestaurantMenuAppBuilder& setFilter(std::unique_ptr<IMenuFilter> filter);

	/// Устанавливает пользовательский принтер меню
	RestaurantMenuAppBuilder& setPrinter(std::unique_ptr<IMenuPrinter> printer);

	/// Устанавливает пользовательский парсер файлов
	RestaurantMenuAppBuilder& setFileParser(std::unique_ptr<IFileParser> fileParser);

	/// Устанавливает пользовательский парсер ввода
	RestaurantMenuAppBuilder& setUserInputParser(std::unique_ptr<UserInputParser> userInputParser);

	/// Создает и возвращает полностью сконфигурированный экземпляр приложения
	std::unique_ptr<RestaurantMenuApp> build();

	/// Статический метод для создания приложения с настройками по умолчанию
	static std::unique_ptr<RestaurantMenuApp> createDefault();
};

#endif // BUILDER_H