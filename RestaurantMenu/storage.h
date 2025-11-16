#pragma once
#ifndef STORAGE_H
#define STORAGE_H

#include "interfaces.h"
#include "models.h"
#include <memory>
#include <vector>

/// Реализация алгоритмов сортировки меню
class MenuSorter : public IMenuSorter {
public:
	/// Сортирует блюда по алфавиту (A-Z)
	void sortAlphabetically(std::vector<Dish>& dishes) const override;

	/// Сортирует блюда по убыванию цены
	void sortByPriceDesc(std::vector<Dish>& dishes) const override;

	/// Сортирует блюда по возрастанию времени приготовления
	void sortByTimeAsc(std::vector<Dish>& dishes) const override;
};

/// Реализация фильтрации меню с автоматической сортировкой результатов
class MenuFilter : public IMenuFilter {
private:
	std::unique_ptr<IMenuSorter> sorter_;   ///< Сортировщик для упорядочивания результатов

public:
	/// Конструктор с инъекцией зависимости сортировщика
	MenuFilter(std::unique_ptr<IMenuSorter> sorter);

	/// Фильтрует блюда по цене и сортирует по убыванию цены
	std::vector<Dish> filterByPrice(const std::vector<Dish>& dishes, double maxPrice) const override;

	/// Фильтрует блюда по времени и сортирует по возрастанию времени
	std::vector<Dish> filterByTime(const std::vector<Dish>& dishes, const Time& maxTime) const override;

	/// Фильтрует блюда по цене и времени, сортирует по алфавиту
	std::vector<Dish> filterByPriceAndTime(const std::vector<Dish>& dishes, double maxPrice, const Time& maxTime) const override;
};

/// Контейнер для хранения блюд меню с возможностью сохранения в файл
class MenuStorage : public IMenuStorage {
private:
	std::vector<Dish> dishes_;  ///< Вектор для хранения блюд

public:
	/// Добавляет блюдо в контейнер
	void addDish(const std::string& name, double price, const Time& time) override;

	/// Возвращает константную ссылку на вектор блюд
	const std::vector<Dish>& getDishes() const override;

	/// Возвращает количество блюд в хранилище
	size_t getDishesCount() const override;

	/// Полностью очищает хранилище
	void clear() override;

	/// Удаляет конкретное блюдо по точному совпадению всех параметров
	bool removeDish(const std::string& name, double price, const Time& time) override;

	/// Сохраняет все блюда в файл в формате "Название" цена время
	void saveToFile(const std::string& filename) const override;
};

#endif // STORAGE_H