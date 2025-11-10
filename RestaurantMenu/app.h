#pragma once
#ifndef APP_H
#define APP_H

#include "interfaces.h"

// Предварительные объявления для уменьшения зависимостей компиляции
class IMenuStorage;
class IMenuSorter;
class IMenuFilter;
class IMenuPrinter;
class IFileParser;
class UserInputParser;

/// Главный класс приложения для управления меню ресторана
/// Реализует паттерн Dependency Injection для гибкости архитектуры
class RestaurantMenuApp {
private:
	// Компоненты приложения, внедряемые через конструктор
	std::unique_ptr<IMenuStorage> storage_;             ///< Хранилище данных меню
	std::unique_ptr<IMenuSorter> sorter_;               ///< Сортировщик блюд
	std::unique_ptr<IMenuFilter> filter_;               ///< Фильтр блюд
	std::unique_ptr<IMenuPrinter> printer_;             ///< Вывод информации
	std::unique_ptr<IFileParser> fileParser_;           ///< Парсер файлов
	std::unique_ptr<UserInputParser> userInputParser_;  ///< Парсер пользовательского ввода

	int invalidCount_;  ///< Счетчик невалидных записей при загрузке

public:
	/// Конструктор с инъекцией зависимостей (Dependency Injection)
	RestaurantMenuApp(
		std::unique_ptr<IMenuStorage> storage,
		std::unique_ptr<IMenuSorter> sorter,
		std::unique_ptr<IMenuFilter> filter,
		std::unique_ptr<IMenuPrinter> printer,
		std::unique_ptr<IFileParser> fileParser,
		std::unique_ptr<UserInputParser> userInputParser);

	/// Загружает меню из файла и автоматически сортирует по алфавиту
	void loadMenu(const std::string& filename);

	/// Выводит полное меню в консоль
	void printMenu() const;

	/// Запускает интерактивный режим управления меню
	void runInteractive();

	/// Обрабатывает отдельную команду пользователя
	void processCommand(const std::string& command);

private:
	/// Обрабатывает пользовательский ввод как критерии фильтрации
	void processUserInput(const std::string& input);

	/// Добавляет новое блюдо в меню
	void addDish(const std::string& dishData);

	/// Удаляет блюдо из меню
	void deleteDish(const std::string& dishData);

	/// Сохраняет текущее меню в файл
	void saveMenu(const std::string& filename) const;

	/// Выводит справку по доступным командам
	void showHelp() const;
};

#endif // APP_H
