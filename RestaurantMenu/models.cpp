#include "models.h"

// Реализации методов Time

Time::Time(int h, int m) {
	setTime(h, m);
}

void Time::setTime(int h, int m) {
	hours = h;
	minutes = m;

	// Нормализация времени
	if (minutes >= 60) {
		hours += minutes / 60;
		minutes = minutes % 60;
	}

	// Если часы превышают 99, устанавливаем максимальное значение
	if (hours > 99) {
		hours = 99;
		minutes = 59;
	}
}

int Time::totalMinutes() const {
	return hours * 60 + minutes;
}

std::string Time::toString() const {
	if (hours > 99) return "+99:59";

	int displayHours = hours;
	int displayMinutes = minutes;

	// Нормализуем минуты для отображения
	if (displayMinutes >= 60) {
		displayHours += displayMinutes / 60;
		displayMinutes = displayMinutes % 60;
	}

	if (displayHours > 99) return "+99:59";

	return (displayHours < 10 ? "0" : "") + std::to_string(displayHours) + ":" +
		(displayMinutes < 10 ? "0" : "") + std::to_string(displayMinutes);
}

bool Time::operator<(const Time& other) const {
	return totalMinutes() < other.totalMinutes();
}

// Реализации методов Dish

Dish::Dish(const std::string& n, double p, const Time& t) : name(n), price(p), time(t) {}

Dish::Dish(const std::string& n, double p, int h, int m) : name(n), price(p) {
	setTime(h, m);
}

void Dish::setTime(int h, int m) {
	time.setTime(h, m);
}

int Dish::totalMinutes() const {
	return time.totalMinutes();
}

std::string Dish::getTimeString() const {
	return time.toString();
}

void Dish::print() const {
	std::cout << std::left << std::setw(25) << name
		<< std::fixed << std::setprecision(2) << std::setw(10) << price
		<< getTimeString() << std::endl;
}