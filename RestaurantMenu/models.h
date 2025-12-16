#pragma once
#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <iostream>
#include <sstream>
#include <ctime>

/// Класс для представления времени с поддержкой часов, минут, дней и лет
class Time {
private:
	std::tm time_{};              ///< Внутреннее представление времени в структуре tm + Инициализация по умолчанию
	void normalizeTime();       ///< Метод для нормализации компонентов времени

public:
	/// Конструктор с инициализацией компонентов времени
	Time(int hours = 0, int minutes = 0, int days = 0, int years = 0);

	/// Устанавливает время с автоматической нормализацией
	void setTime(int hours, int minutes, int days = 0, int years = 0);

	/// Возвращает общее время в минутах для удобства сравнения
	long long totalMinutes() const;

	/// Возвращает человеко-читаемое строковое представление времени
	std::string toString() const;

	/// Оператор сравнения "меньше" для сортировки
	bool operator<(const Time& other) const;

	/// Оператор проверки на равенство временных интервалов
	bool operator==(const Time& other) const;

	// Геттеры для доступа к компонентам времени
	int getHours() const { return time_.tm_hour; }
	int getMinutes() const { return time_.tm_min; }
	int getDays() const { return time_.tm_mday - 1; }
	int getYears() const { return time_.tm_year; }
};

/// Класс, представляющий блюдо в меню ресторана
class Dish {
public:
	std::string name;   ///< Название блюда
	double price;       ///< Цена блюда в валюте
	Time time;          ///< Время приготовления блюда

	/// Конструктор с объектом Time
	Dish(const std::string& n, double p, const Time& t);

	/// Конструктор с отдельными компонентами времени
	Dish(const std::string& n, double p, int hours, int minutes, int days = 0, int years = 0);

	/// Устанавливает время приготовления
	void setTime(int hours, int minutes, int days = 0, int years = 0);

	/// Возвращает общее время приготовления в минутах
	long long totalMinutes() const;

	/// Возвращает строковое представление времени приготовления
	std::string getTimeString() const;

	/// Выводит информацию о блюде в стандартный поток вывода
	void print() const;

	/// Оператор вывода для потоков
	friend std::ostream& operator<<(std::ostream& os, const Dish& dish);
};

#endif // MODELS_H