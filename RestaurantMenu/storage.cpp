#include "storage.h"

// Реализации MenuStorage

void MenuStorage::addDish(const std::string& name, double price, int hours, int minutes) {
	dishes_.emplace_back(name, price, hours, minutes);
}

const std::vector<Dish>& MenuStorage::getDishes() const { return dishes_; }
size_t MenuStorage::getDishesCount() const { return dishes_.size(); }
void MenuStorage::clear() { dishes_.clear(); }

// Реализации MenuSorter

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

// Реализации MenuFilter

MenuFilter::MenuFilter(std::unique_ptr<IMenuSorter> sorter) : sorter_(std::move(sorter)) {}

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