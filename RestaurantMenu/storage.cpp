#include "storage.h"
#include <algorithm>
#include <cmath>

// ==================== РЕАЛИЗАЦИЯ MENU SORTER ====================

/// Сортирует блюда в алфавитном порядке по названию (A-Z)
void MenuSorter::sortAlphabetically(std::vector<Dish>& dishes) const {
	std::sort(dishes.begin(), dishes.end(),
		[](const Dish& a, const Dish& b) { return a.name < b.name; });
}

/// Сортирует блюда по убыванию цены (от самой дорогой к самой дешевой)
void MenuSorter::sortByPriceDesc(std::vector<Dish>& dishes) const {
	std::sort(dishes.begin(), dishes.end(),
		[](const Dish& a, const Dish& b) { return a.price > b.price; });
}

/// Сортирует блюда по возрастанию времени приготовления
void MenuSorter::sortByTimeAsc(std::vector<Dish>& dishes) const {
	std::sort(dishes.begin(), dishes.end(),
		[](const Dish& a, const Dish& b) { return a.time < b.time; });
}

// ==================== РЕАЛИЗАЦИЯ MENU FILTER ====================

/// Конструктор фильтра с инъекцией зависимости сортировщика
MenuFilter::MenuFilter(std::unique_ptr<IMenuSorter> sorter)
	: sorter_(std::move(sorter)) {}

/// Фильтрует блюда по максимальной цене и сортирует по убыванию цены
std::vector<Dish> MenuFilter::filterByPrice(const std::vector<Dish>& dishes, double maxPrice) const {
	std::vector<Dish> result;

	// Проходим по всем блюдам и отбираем те, что дешевле максимальной цены
	for (const auto& dish : dishes) {
		if (dish.price < maxPrice) result.push_back(dish);
	}

	// Сортируем результаты по убыванию цены
	sorter_->sortByPriceDesc(result);
	return result;
}

/// Фильтрует блюда по максимальному времени и сортирует по возрастанию времени
std::vector<Dish> MenuFilter::filterByTime(const std::vector<Dish>& dishes, const Time& maxTime) const {
	std::vector<Dish> result;
	long long maxTotalMinutes = maxTime.totalMinutes();

	// Проходим по всем блюдам и отбираем те, что готовятся быстрее
	for (const auto& dish : dishes) {
		if (dish.totalMinutes() < maxTotalMinutes) result.push_back(dish);
	}

	// Сортируем результаты по возрастанию времени приготовления
	sorter_->sortByTimeAsc(result);
	return result;
}

/// Фильтрует блюда одновременно по цене и времени, сортирует по алфавиту
std::vector<Dish> MenuFilter::filterByPriceAndTime(const std::vector<Dish>& dishes, double maxPrice, const Time& maxTime) const {
	std::vector<Dish> result;
	long long maxTotalMinutes = maxTime.totalMinutes();

	// Проходим по всем блюдам и отбираем по двум критериям одновременно
	for (const auto& dish : dishes) {
		if (dish.price < maxPrice && dish.totalMinutes() < maxTotalMinutes) {
			result.push_back(dish);
		}
	}

	// Сортируем результаты в алфавитном порядке
	sorter_->sortAlphabetically(result);
	return result;
}

// ==================== РЕАЛИЗАЦИЯ MENU STORAGE ====================

/// Добавляет новое блюдо в конец хранилища
void MenuStorage::addDish(const std::string& name, double price, const Time& time) {
	dishes_.emplace_back(name, price, time);
}

/// Возвращает константную ссылку на вектор всех блюд
const std::vector<Dish>& MenuStorage::getDishes() const {
	return dishes_;
}

/// Возвращает текущее количество блюд в хранилище
size_t MenuStorage::getDishesCount() const {
	return dishes_.size();
}

/// Полностью очищает хранилище, удаляя все блюда
void MenuStorage::clear() {
	dishes_.clear();
}

/// Удаляет конкретное блюдо по точному совпадению всех параметров
bool MenuStorage::removeDish(const std::string& name, double price, const Time& time) {
	const double epsilon = 0.001; // Погрешность для сравнения дробных чисел

	size_t initialSize = dishes_.size();

	// Используем алгоритм remove_if для удаления элементов по предикату
	dishes_.erase(
		std::remove_if(dishes_.begin(), dishes_.end(),
			[&](const Dish& dish) {
		// Сравниваем время целиком через оператор ==, а не отдельные компоненты
		return dish.name == name &&
			std::fabs(dish.price - price) < epsilon &&
			dish.time == time;
	}),
		dishes_.end()
		);

	// Возвращаем true если что-то было удалено
	return dishes_.size() < initialSize;
}

/// Сохраняет все блюда в файл в формате, пригодном для последующей загрузки
void MenuStorage::saveToFile(const std::string& filename) const {
	std::ofstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Не удалось открыть файл для сохранения: " + filename);
	}

	// Записываем каждое блюдо в отдельной строке
	for (const auto& dish : dishes_) {
		file << "\"" << dish.name << "\" "
			<< std::fixed << std::setprecision(2) << dish.price << " "
			<< dish.getTimeString() << std::endl;
	}

	file.close();
}