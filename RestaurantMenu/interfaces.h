#pragma once
#ifndef INTERFACES_H
#define INTERFACES_H

#include <vector>
#include <string>

// Предварительные объявления для уменьшения зависимости от заголовков
class Dish;
class Time;

// ==================== ИНТЕРФЕЙСЫ ====================

/// Базовый интерфейс для всех парсеров
class IParser {
public:
	virtual ~IParser() = default;

	/// Парсит входную строку и возвращает результат операции
	virtual bool parse(const std::string& input) = 0;

	/// Проверяет валидность результата последней операции парсинга
	virtual bool isValid() const = 0;

	/// Сбрасывает состояние парсера к начальному
	virtual void reset() = 0;
};

/// Интерфейс для хранения данных меню
class IMenuStorage {
public:
	virtual ~IMenuStorage() = default;

	/// Добавляет новое блюдо в хранилище
	virtual void addDish(const std::string& name, double price, const Time& time) = 0;

	/// Возвращает константную ссылку на вектор всех блюд
	virtual const std::vector<Dish>& getDishes() const = 0;

	/// Возвращает общее количество блюд в хранилище
	virtual size_t getDishesCount() const = 0;

	/// Полностью очищает хранилище
	virtual void clear() = 0;

	/// Удаляет конкретное блюдо по всем параметрам
	virtual bool removeDish(const std::string& name, double price, const Time& time) = 0;

	/// Сохраняет все блюда в указанный файл
	virtual void saveToFile(const std::string& filename) const = 0;
};

/// Интерфейс для сортировки блюд меню
class IMenuSorter {
public:
	virtual ~IMenuSorter() = default;

	/// Сортирует блюда в алфавитном порядке по названию
	virtual void sortAlphabetically(std::vector<Dish>& dishes) const = 0;

	/// Сортирует блюда по убыванию цены (от самой дорогой к самой дешевой)
	virtual void sortByPriceDesc(std::vector<Dish>& dishes) const = 0;

	/// Сортирует блюда по возрастанию времени приготовления
	virtual void sortByTimeAsc(std::vector<Dish>& dishes) const = 0;
};

/// Интерфейс для фильтрации блюд меню
class IMenuFilter {
public:
	virtual ~IMenuFilter() = default;

	/// Фильтрует блюда по максимальной цене (включительно)
	virtual std::vector<Dish> filterByPrice(const std::vector<Dish>& dishes, double maxPrice) const = 0;

	/// Фильтрует блюда по максимальному времени приготовления
	virtual std::vector<Dish> filterByTime(const std::vector<Dish>& dishes, const Time& maxTime) const = 0;

	/// Фильтрует блюда одновременно по цене и времени приготовления
	virtual std::vector<Dish> filterByPriceAndTime(const std::vector<Dish>& dishes, double maxPrice, const Time& maxTime) const = 0;
};

/// Интерфейс для вывода информации о меню
class IMenuPrinter {
public:
	virtual ~IMenuPrinter() = default;

	/// Выводит все блюда меню с статистикой невалидных записей
	virtual void printAllDishes(const std::vector<Dish>& dishes, int invalidCount) const = 0;

	/// Выводит отфильтрованные блюда с пользовательским заголовком
	virtual void printFilteredDishes(const std::vector<Dish>& dishes, const std::string& title) const = 0;
};

/// Интерфейс для парсинга файлов с меню
class IFileParser {
public:
	virtual ~IFileParser() = default;

	/// Парсит файл с меню и заполняет хранилище, подсчитывая невалидные строки
	virtual void parseFile(const std::string& filename, IMenuStorage& storage, int& invalidCount) = 0;
};

#endif // INTERFACES_H