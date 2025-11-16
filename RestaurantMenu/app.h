#pragma once
#ifndef APP_H
#define APP_H

#include "interfaces.h"
#include "parsers.h"
#include <memory>
#include <string>

// Предварительные объявления
class IMenuStorage;
class IMenuSorter;
class IMenuFilter;
class IMenuPrinter;
class IFileParser;

/// Главный класс приложения для управления ресторанным меню
class RestaurantMenuApp {
private:
	std::unique_ptr<IMenuStorage> storage_;          ///< Хранилище блюд меню
	std::unique_ptr<IMenuSorter> sorter_;            ///< Сортировщик блюд
	std::unique_ptr<IMenuFilter> filter_;            ///< Фильтр блюд
	std::unique_ptr<IMenuPrinter> printer_;          ///< Принтер для вывода
	std::unique_ptr<IFileParser> fileParser_;        ///< Парсер файлов
	std::unique_ptr<UserInputParser> userInputParser_; ///< Парсер пользовательского ввода
	int invalidCount_;                               ///< Счетчик невалидных записей

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

	/// Очищает текущее меню
	void clearMenu();

	/// Выводит все меню
	void printMenu() const;

	/// Запускает интерактивный режим
	void runInteractive();

	/// Обрабатывает команды управления
	void processCommand(const std::string& command);

private:
	/// Обрабатывает ввод для фильтрации
	void processUserInput(const std::string& input);

	/// Добавляет блюдо
	void addDish(const std::string& dishData);

	/// Удаляет блюдо
	void deleteDish(const std::string& dishData);

	/// Сохраняет меню в файл
	void saveMenu(const std::string& filename) const;

	/// Очищает консоль
	void clearConsole() const;

	/// Показывает справку по командам
	void showHelp() const;
};

#endif // APP_H