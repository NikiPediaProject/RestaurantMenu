#include "storage.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>

// ==================== MENU SORTER ====================

// Сортирует блюда по алфавиту (A-Z)
void MenuSorter::sortAlphabetically(std::vector<Dish>& dishes) const {
	std::sort(dishes.begin(), dishes.end(),
		[](const Dish& a, const Dish& b) { return a.name < b.name; });
}

// Сортирует блюда по убыванию цены
void MenuSorter::sortByPriceDesc(std::vector<Dish>& dishes) const {
	std::sort(dishes.begin(), dishes.end(),
		[](const Dish& a, const Dish& b) { return a.price > b.price; });
}

// Сортирует блюда по возрастанию времени приготовления
void MenuSorter::sortByTimeAsc(std::vector<Dish>& dishes) const {
	std::sort(dishes.begin(), dishes.end(),
		[](const Dish& a, const Dish& b) { return a.time < b.time; });
}

// ==================== MENU FILTER ====================

// Конструктор с инъекцией зависимости сортировщика
MenuFilter::MenuFilter(std::unique_ptr<IMenuSorter> sorter)
	: sorter_(std::move(sorter)) {}

// Фильтрует блюда по цене и сортирует по убыванию цены
std::vector<Dish> MenuFilter::filterByPrice(const std::vector<Dish>& dishes, double maxPrice) const {
	std::vector<Dish> result;
	for (const auto& dish : dishes) {
		if (dish.price < maxPrice) result.push_back(dish);
	}
	sorter_->sortByPriceDesc(result);
	return result;
}

// Фильтрует блюда по времени и сортирует по возрастанию времени
std::vector<Dish> MenuFilter::filterByTime(const std::vector<Dish>& dishes, const Time& maxTime) const {
	std::vector<Dish> result;
	long long maxTotalMinutes = maxTime.totalMinutes();
	for (const auto& dish : dishes) {
		if (dish.totalMinutes() < maxTotalMinutes) result.push_back(dish);
	}
	sorter_->sortByTimeAsc(result);
	return result;
}

// Фильтрует блюда по цене и времени, сортирует по алфавиту
std::vector<Dish> MenuFilter::filterByPriceAndTime(const std::vector<Dish>& dishes, double maxPrice, const Time& maxTime) const {
	std::vector<Dish> result;
	long long maxTotalMinutes = maxTime.totalMinutes();
	for (const auto& dish : dishes) {
		if (dish.price < maxPrice && dish.totalMinutes() < maxTotalMinutes) {
			result.push_back(dish);
		}
	}
	sorter_->sortAlphabetically(result);
	return result;
}

// ==================== MENU STORAGE ====================

// Добавляет блюдо в контейнер
void MenuStorage::addDish(const std::string& name, double price, const Time& time) {
	dishes_.emplace_back(name, price, time);
}

// Возвращает константную ссылку на вектор блюд
const std::vector<Dish>& MenuStorage::getDishes() const {
	return dishes_;
}

// Возвращает количество блюд в хранилище
size_t MenuStorage::getDishesCount() const {
	return dishes_.size();
}

// Полностью очищает хранилище
void MenuStorage::clear() {
	dishes_.clear();
}

// Удаляет конкретное блюдо по точному совпадению всех параметров
bool MenuStorage::removeDish(const std::string& name, double price, const Time& time) {
	const double epsilon = 0.001;

	size_t initialSize = dishes_.size();

	// Используем публичные методы для сравнения
	dishes_.erase(
		std::remove_if(dishes_.begin(), dishes_.end(),
			[&](const Dish& dish) {
		// Сравниваем по имени, цене и полному времени
		return dish.name == name &&
			std::fabs(dish.price - price) < epsilon &&
			dish.time == time; // Используем оператор == для Time
	}),
		dishes_.end()
		);

	return dishes_.size() < initialSize;
}

// Сохраняет все блюда в файл
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