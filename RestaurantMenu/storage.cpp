#include "storage.h"
#include <algorithm>
#include <cmath>

// ==================== MENU SORTER ====================

void MenuSorter::sortAlphabetically(std::vector<Dish>& dishes) const {
	std::sort(dishes.begin(), dishes.end(),
		[](const Dish& a, const Dish& b) { return a.name < b.name; });
}

void MenuSorter::sortByPriceDesc(std::vector<Dish>& dishes) const {
	std::sort(dishes.begin(), dishes.end(),
		[](const Dish& a, const Dish& b) { return a.price > b.price; });
}

void MenuSorter::sortByTimeAsc(std::vector<Dish>& dishes) const {
	std::sort(dishes.begin(), dishes.end(),
		[](const Dish& a, const Dish& b) { return a.time < b.time; });
}

// ==================== MENU FILTER ====================

MenuFilter::MenuFilter(std::unique_ptr<IMenuSorter> sorter)
	: sorter_(std::move(sorter)) {}

std::vector<Dish> MenuFilter::filterByPrice(const std::vector<Dish>& dishes, double maxPrice) const {
	std::vector<Dish> result;
	for (const auto& dish : dishes) {
		if (dish.price < maxPrice) result.push_back(dish);
	}
	sorter_->sortByPriceDesc(result);
	return result;
}

std::vector<Dish> MenuFilter::filterByTime(const std::vector<Dish>& dishes, const Time& maxTime) const {
	std::vector<Dish> result;
	int maxTotalMinutes = maxTime.totalMinutes();
	for (const auto& dish : dishes) {
		if (dish.totalMinutes() < maxTotalMinutes) result.push_back(dish);
	}
	sorter_->sortByTimeAsc(result);
	return result;
}

std::vector<Dish> MenuFilter::filterByPriceAndTime(const std::vector<Dish>& dishes, double maxPrice, const Time& maxTime) const {
	std::vector<Dish> result;
	int maxTotalMinutes = maxTime.totalMinutes();
	for (const auto& dish : dishes) {
		if (dish.price < maxPrice && dish.totalMinutes() < maxTotalMinutes) {
			result.push_back(dish);
		}
	}
	sorter_->sortAlphabetically(result);
	return result;
}

// ==================== MENU STORAGE ====================

void MenuStorage::addDish(const std::string& name, double price, int hours, int minutes) {
	dishes_.emplace_back(name, price, hours, minutes);
}

const std::vector<Dish>& MenuStorage::getDishes() const {
	return dishes_;
}

size_t MenuStorage::getDishesCount() const {
	return dishes_.size();
}

void MenuStorage::clear() {
	dishes_.clear();
}

bool MenuStorage::removeDish(const std::string& name, double price, int hours, int minutes) {
	const double epsilon = 0.001;

	size_t initialSize = dishes_.size();

	dishes_.erase(
		std::remove_if(dishes_.begin(), dishes_.end(),
			[&](const Dish& dish) {
		// Если цена и время не указаны (равны 0), удаляем только по названию
		if (price == 0 && hours == 0 && minutes == 0) {
			return dish.name == name;
		}
		// Если указана только цена, удаляем по названию и цене
		else if (hours == 0 && minutes == 0) {
			return dish.name == name && std::fabs(dish.price - price) < epsilon;
		}
		// Если указаны все параметры, удаляем по полному совпадению
		else {
			return dish.name == name &&
				std::fabs(dish.price - price) < epsilon &&
				dish.time.hours == hours &&
				dish.time.minutes == minutes;
		}
	}),
		dishes_.end()
		);

	return dishes_.size() < initialSize;
}

void MenuStorage::saveToFile(const std::string& filename) const {
	std::ofstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Не удалось открыть файл для сохранения: " + filename);
	}

	for (const auto& dish : dishes_) {
		file << "\"" << dish.name << "\" "
			<< std::fixed << std::setprecision(2) << dish.price << " "
			<< dish.getTimeString() << std::endl;
	}

	file.close();
}