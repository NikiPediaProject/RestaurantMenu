#pragma once
#ifndef APP_H
#define APP_H

#include "interfaces.h"
#include "storage.h"
#include "parsers.h"
#include "printer.h"
#include "file_parser.h"
#include <memory>
#include <string>

// Предварительные объявления
class IMenuStorage;
class IMenuSorter;
class IMenuFilter;
class IMenuPrinter;
class IFileParser;
class UserInputParser;

/// Главный класс приложения ресторанного меню
class RestaurantMenuApp {
private:
	std::unique_ptr<IMenuStorage> storage_;
	std::unique_ptr<IMenuSorter> sorter_;
	std::unique_ptr<IMenuFilter> filter_;
	std::unique_ptr<IMenuPrinter> printer_;
	std::unique_ptr<IFileParser> fileParser_;
	std::unique_ptr<UserInputParser> userInputParser_;
	int invalidCount_;

public:
	/// Конструктор с инъекцией зависимостей
	RestaurantMenuApp(
		std::unique_ptr<IMenuStorage> storage,
		std::unique_ptr<IMenuSorter> sorter,
		std::unique_ptr<IMenuFilter> filter,
		std::unique_ptr<IMenuPrinter> printer,
		std::unique_ptr<IFileParser> fileParser,
		std::unique_ptr<UserInputParser> userInputParser);

	/// Загружает меню из файла
	void loadMenu(const std::string& filename);

	/// Выводит все меню
	void printMenu() const;

	/// Запускает интерактивный режим управления
	void runInteractive();

	/// Обрабатывает команды управления
	void processCommand(const std::string& command);

private:
	/// Обрабатывает пользовательский ввод для фильтрации
	void processUserInput(const std::string& input);

	/// Добавляет блюдо
	void addDish(const std::string& dishData);

	/// Удаляет блюдо
	void deleteDish(const std::string& dishData);

	/// Сохраняет меню в файл
	void saveMenu(const std::string& filename) const;

	/// Показывает справку по командам
	void showHelp() const;
};

#endif // APP_H