#pragma once
#ifndef PRINTER_H
#define PRINTER_H

#include "interfaces.h"
#include "models.h"
#include <iostream>
#include <iomanip>

/// Реализация вывода меню в табличном формате в консоль
class MenuPrinter : public IMenuPrinter {
public:
	/// Выводит полное меню с заголовком и статистикой
	void printAllDishes(const std::vector<Dish>& dishes, int invalidCount) const override;

	/// Выводит отфильтрованные блюда с пользовательским заголовком
	void printFilteredDishes(const std::vector<Dish>& dishes, const std::string& title) const override;

private:
	/// Вспомогательный метод для вывода заголовка таблицы
	void printTableHeader() const;
};

#endif // PRINTER_H