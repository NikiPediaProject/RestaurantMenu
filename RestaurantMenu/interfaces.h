#pragma once
#ifndef INTERFACES_H
#define INTERFACES_H

#include <vector>
#include <string>

// Предварительное объявление класса Dish
class Dish;

// ==================== ИНТЕРФЕЙСЫ ====================

/// Базовый интерфейс для всех парсеров
class IParser {
public:
	virtual ~IParser() = default;
	virtual bool parse(const std::string& input) = 0;  ///< Парсит входную строку
	virtual bool isValid() const = 0;                  ///< Проверяет валидность результата
	virtual void reset() = 0;                          ///< Сбрасывает состояние парсера
};

/// Интерфейс для хранения меню
class IMenuStorage {
public:
	virtual ~IMenuStorage() = default;
	virtual void addDish(const std::string& name, double price, int hours, int minutes) = 0; ///< Добавляет блюдо
	virtual const std::vector<Dish>& getDishes() const = 0;                           ///< Возвращает все блюда
	virtual size_t getDishesCount() const = 0;                                              ///< Возвращает количество блюд
	virtual void clear() = 0;                                                               ///< Очищает хранилище
	virtual bool removeDish(const std::string& name, double price, int hours, int minutes) = 0; ///< Удаляет блюдо
	virtual void saveToFile(const std::string& filename) const = 0;                       ///< Сохраняет в файл
};

/// Интерфейс для сортировки меню
class IMenuSorter {
public:
	virtual ~IMenuSorter() = default;
	virtual void sortAlphabetically(std::vector<Dish>& dishes) const = 0;   ///< Сортирует по алфавиту
	virtual void sortByPriceDesc(std::vector<Dish>& dishes) const = 0;      ///< Сортирует по убыванию цены
	virtual void sortByTimeAsc(std::vector<Dish>& dishes) const = 0;        ///< Сортирует по возрастанию времени
};

/// Интерфейс для фильтрации меню
class IMenuFilter {
public:
	virtual ~IMenuFilter() = default;
	virtual std::vector<Dish> filterByPrice(const std::vector<Dish>& dishes, double maxPrice) const = 0; ///< Фильтрует по цене
	virtual std::vector<Dish> filterByTime(const std::vector<Dish>& dishes, const class Time& maxTime) const = 0; ///< Фильтрует по времени
	virtual std::vector<Dish> filterByPriceAndTime(const std::vector<Dish>& dishes, double maxPrice, const class Time& maxTime) const = 0; ///< Фильтрует по цене и времени
};

/// Интерфейс для вывода меню
class IMenuPrinter {
public:
	virtual ~IMenuPrinter() = default;
	virtual void printAllDishes(const std::vector<Dish>& dishes, int invalidCount) const = 0;     ///< Выводит все блюда
	virtual void printFilteredDishes(const std::vector<Dish>& dishes, const std::string& title) const = 0; ///< Выводит отфильтрованные блюда
};

/// Интерфейс для парсинга файлов
class IFileParser {
public:
	virtual ~IFileParser() = default;
	virtual void parseFile(const std::string& filename, IMenuStorage& storage, int& invalidCount) = 0; ///< Парсит файл с меню
};

#endif // INTERFACES_H