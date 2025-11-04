#include "models.h"

// ==================== РЕАЛИЗАЦИЯ КЛАССА TIME ====================

/// Конструктор времени - инициализирует компоненты и нормализует значения
Time::Time(int hours, int minutes, int days, int years) {
	setTime(hours, minutes, days, years);
}

/// Устанавливает время с автоматической нормализацией переполненных компонентов
void Time::setTime(int hours, int minutes, int days, int years) {
	time_.tm_hour = hours;
	time_.tm_min = minutes;
	time_.tm_mday = days + 1; // tm_mday начинается с 1, поэтому добавляем 1 к дням
	time_.tm_year = years;
	time_.tm_sec = 0;
	time_.tm_isdst = -1; // не учитывать летнее время автоматически

	// Выполняем нормализацию для корректного представления времени
	normalizeTime();
}

/// Нормализует компоненты времени: минуты -> часы, часы -> дни и т.д.
void Time::normalizeTime() {
	// Нормализация минут: преобразуем 60+ минут в часы
	if (time_.tm_min >= 60) {
		time_.tm_hour += time_.tm_min / 60;
		time_.tm_min = time_.tm_min % 60;
	}

	// Нормализация часов: преобразуем 24+ часов в дни
	if (time_.tm_hour >= 24) {
		time_.tm_mday += time_.tm_hour / 24;
		time_.tm_hour = time_.tm_hour % 24;
	}

	// Нормализация дней: предполагаем 30 дней в месяце для упрощения
	if (time_.tm_mday > 30) {
		time_.tm_year += (time_.tm_mday - 1) / 30;
		time_.tm_mday = ((time_.tm_mday - 1) % 30) + 1;
	}

	// Ограничение на очень большие значения для избежания переполнения
	if (time_.tm_year > 999) {
		time_.tm_year = 999;
		time_.tm_mday = 30;
		time_.tm_hour = 23;
		time_.tm_min = 59;
	}
}

/// Возвращает общее время в минутах для удобства сравнения и вычислений
long long Time::totalMinutes() const {
	return time_.tm_min +
		time_.tm_hour * 60 +
		(time_.tm_mday - 1) * 24 * 60 +
		time_.tm_year * 30 * 24 * 60;
}

/// Форматирует время в человеко-читаемую строку с автоматическим выбором компонентов
std::string Time::toString() const {
	std::stringstream ss;

	// Добавляем годы только если они есть
	if (time_.tm_year > 0) {
		ss << time_.tm_year << "г. ";
	}

	// Добавляем дни только если они есть или есть годы
	if (time_.tm_mday > 1 || time_.tm_year > 0) {
		ss << (time_.tm_mday - 1) << "д. ";
	}

	// Добавляем часы только если они есть или есть дни/годы
	if (time_.tm_hour > 0 || time_.tm_mday > 1 || time_.tm_year > 0) {
		ss << time_.tm_hour << "ч ";
	}

	// Минуты всегда присутствуют в выводе
	ss << time_.tm_min << "м";

	return ss.str();
}

/// Оператор сравнения для сортировки по времени (основан на общем количестве минут)
bool Time::operator<(const Time& other) const {
	return totalMinutes() < other.totalMinutes();
}

/// Оператор проверки на полное равенство всех компонентов времени
bool Time::operator==(const Time& other) const {
	return time_.tm_year == other.time_.tm_year &&
		time_.tm_mday == other.time_.tm_mday &&
		time_.tm_hour == other.time_.tm_hour &&
		time_.tm_min == other.time_.tm_min;
}

// ==================== РЕАЛИЗАЦИЯ КЛАССА DISH ====================

/// Конструктор блюда с объектом Time
Dish::Dish(const std::string& n, double p, const Time& t) : name(n), price(p), time(t) {}

/// Конструктор блюда с отдельными компонентами времени
Dish::Dish(const std::string& n, double p, int hours, int minutes, int days, int years)
	: name(n), price(p) {
	setTime(hours, minutes, days, years);
}

/// Устанавливает время приготовления блюда
void Dish::setTime(int hours, int minutes, int days, int years) {
	time.setTime(hours, minutes, days, years);
}

/// Возвращает общее время приготовления в минутах
long long Dish::totalMinutes() const {
	return time.totalMinutes();
}

/// Возвращает строковое представление времени приготовления
std::string Dish::getTimeString() const {
	return time.toString();
}

/// Выводит информацию о блюде в стандартный поток вывода
void Dish::print() const {
	std::cout << *this; // Используем перегруженный оператор вывода
}

/// Перегруженный оператор вывода для форматированного отображения блюда
std::ostream& operator<<(std::ostream& os, const Dish& dish) {
	std::stringstream ss;

	// Форматируем вывод с выравниванием столбцов
	ss << std::left << std::setw(25) << dish.name
		<< std::fixed << std::setprecision(2) << std::setw(10) << dish.price
		<< dish.getTimeString();

	os << ss.str();
	return os;
}