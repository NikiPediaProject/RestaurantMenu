#pragma once
#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <iomanip>
#include <iostream>

/// Представляет время в часах и минутах
class Time {
public:
	int hours;
	int minutes;

	Time(int h = 0, int m = 0);

	/// Устанавливает время с нормализацией
	void setTime(int h, int m);

	/// Возвращает общее время в минутах
	int totalMinutes() const;

	/// Возвращает строковое представление времени
	std::string toString() const;

	/// Оператор сравнения для сортировки
	bool operator<(const Time& other) const;
};

/// Представляет блюдо в меню
class Dish {
public:
	std::string name;
	double price;
	Time time;

	Dish(const std::string& n, double p, const Time& t);
	Dish(const std::string& n, double p, int h, int m);

	/// Устанавливает время приготовления
	void setTime(int h, int m);

	/// Возвращает время приготовления в минутах
	int totalMinutes() const;

	/// Возвращает строковое представление времени
	std::string getTimeString() const;

	/// Выводит информацию о блюде
	void print() const;
};

#endif // MODELS_H