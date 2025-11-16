#include "models.h"
#include <iomanip>

// ==================== РЕАЛИЗАЦИЯ МЕТОДОВ TIME ====================

// Конструктор с инициализацией компонентов времени
Time::Time(int hours, int minutes, int days, int years) {
	setTime(hours, minutes, days, years);
}

// Устанавливает время с автоматической нормализацией
void Time::setTime(int hours, int minutes, int days, int years) {
	time_.tm_hour = hours;
	time_.tm_min = minutes;
	time_.tm_mday = days + 1; // tm_mday начинается с 1
	time_.tm_year = years;
	time_.tm_sec = 0;
	time_.tm_isdst = -1; // не учитывать летнее время

	normalizeTime();
}

// Нормализует компоненты времени
void Time::normalizeTime() {
	// Нормализация минут
	if (time_.tm_min >= 60) {
		time_.tm_hour += time_.tm_min / 60;
		time_.tm_min = time_.tm_min % 60;
	}

	// Нормализация часов
	if (time_.tm_hour >= 24) {
		time_.tm_mday += time_.tm_hour / 24;
		time_.tm_hour = time_.tm_hour % 24;
	}

	// Нормализация дней (предполагаем 31 день в месяце для простоты)
	if (time_.tm_mday > 31) {
		time_.tm_year += (time_.tm_mday - 1) / 31;
		time_.tm_mday = ((time_.tm_mday - 1) % 31) + 1;
	}

	// Ограничение на очень большие значения
	if (time_.tm_year > 999) {
		time_.tm_year = 999;
		time_.tm_mday = 31;
		time_.tm_hour = 23;
		time_.tm_min = 59;
	}
}

// Возвращает общее время в минутах
long long Time::totalMinutes() const {
	return time_.tm_min +
		time_.tm_hour * 60 +
		(time_.tm_mday - 1) * 24 * 60 +
		time_.tm_year * 31 * 24 * 60;
}

// Возвращает строковое представление времени
std::string Time::toString() const {
	std::stringstream ss;

	if (time_.tm_year > 0) {
		ss << time_.tm_year << "г. ";
	}
	if (time_.tm_mday > 1 || time_.tm_year > 0) {
		ss << (time_.tm_mday - 1) << "д. ";
	}
	if (time_.tm_hour > 0 || time_.tm_mday > 1 || time_.tm_year > 0) {
		ss << time_.tm_hour << "ч ";
	}
	ss << time_.tm_min << "м";

	return ss.str();
}

// Оператор сравнения "меньше" для сортировки
bool Time::operator<(const Time& other) const {
	return totalMinutes() < other.totalMinutes();
}

// Оператор проверки на равенство временных интервалов
bool Time::operator==(const Time& other) const {
	return time_.tm_year == other.time_.tm_year &&
		time_.tm_mday == other.time_.tm_mday &&
		time_.tm_hour == other.time_.tm_hour &&
		time_.tm_min == other.time_.tm_min;
}

// ==================== РЕАЛИЗАЦИЯ МЕТОДОВ DISH ====================

// Конструктор с объектом Time
Dish::Dish(const std::string& n, double p, const Time& t) : name(n), price(p), time(t) {}

// Конструктор с отдельными компонентами времени
Dish::Dish(const std::string& n, double p, int hours, int minutes, int days, int years)
	: name(n), price(p) {
	setTime(hours, minutes, days, years);
}

// Устанавливает время приготовления
void Dish::setTime(int hours, int minutes, int days, int years) {
	time.setTime(hours, minutes, days, years);
}

// Возвращает общее время приготовления в минутах
long long Dish::totalMinutes() const {
	return time.totalMinutes();
}

// Возвращает строковое представление времени приготовления
std::string Dish::getTimeString() const {
	return time.toString();
}

// Выводит информацию о блюде
void Dish::print() const {
	std::cout << *this; // Используем оператор вывода
}

// Оператор вывода для Dish
std::ostream& operator<<(std::ostream& os, const Dish& dish) {
	std::stringstream ss;
	ss << std::left << std::setw(25) << dish.name
		<< std::fixed << std::setprecision(2) << std::setw(10) << dish.price
		<< dish.getTimeString();

	os << ss.str();
	return os;
}