#pragma once
#ifndef PRINTER_H
#define PRINTER_H

#include "interfaces.h"
#include "models.h"
#include <iomanip>
#include <iostream>

/// Реализация вывода меню в консоль
class MenuPrinter : public IMenuPrinter {
public:
	void printAllDishes(const std::vector<Dish>& dishes, int invalidCount) const override;
	void printFilteredDishes(const std::vector<Dish>& dishes, const std::string& title) const override;

private:
	void printTableHeader() const;
};

#endif // PRINTER_H