#pragma once
#ifndef STORAGE_H
#define STORAGE_H

#include "interfaces.h"
#include "models.h"
#include <vector>
#include <algorithm>

/// Хранилище для блюд меню
class MenuStorage : public IMenuStorage {
private:
	std::vector<Dish> dishes_;

public:
	void addDish(const std::string& name, double price, int hours, int minutes) override;
	const std::vector<Dish>& getDishes() const override;
	size_t getDishesCount() const override;
	void clear() override;
};

/// Реализация сортировки меню
class MenuSorter : public IMenuSorter {
public:
	void sortAlphabetically(std::vector<Dish>& dishes) const override;
	void sortByPriceDesc(std::vector<Dish>& dishes) const override;
	void sortByTimeAsc(std::vector<Dish>& dishes) const override;
};

/// Реализация фильтрации меню
class MenuFilter : public IMenuFilter {
private:
	std::unique_ptr<IMenuSorter> sorter_;

public:
	MenuFilter(std::unique_ptr<IMenuSorter> sorter);

	std::vector<Dish> filterByPrice(const std::vector<Dish>& dishes, double maxPrice) const override;
	std::vector<Dish> filterByTime(const std::vector<Dish>& dishes, const Time& maxTime) const override;
	std::vector<Dish> filterByPriceAndTime(const std::vector<Dish>& dishes, double maxPrice, const Time& maxTime) const override;
};

#endif // STORAGE_H