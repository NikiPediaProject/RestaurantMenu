#include "file_parser.h"
#include "utils.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

MenuFileParser::MenuFileParser()
	: numberParser_(std::make_unique<NumberParser>())
	, timeParser_(std::make_unique<TimeParser>())
	, name_pattern_("\"([^\"]*)\"")                    // Название в двойных кавычках
	, time_pattern_(R"((\d{1,5}:\d{1,5}))")           // Время в формате чч:мм (больший диапазон)
	, price_pattern_(R"(-?\d+(?:[.,]\d+)?)")          // Цена (дробное, может быть отрицательным)
{}

void MenuFileParser::parseFile(const std::string& filename, IMenuStorage& storage, int& invalidCount) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Ошибка открытия файла!");
	}

	std::string line;
	int lineNum = 0;
	while (std::getline(file, line)) {
		lineNum++;
		if (!line.empty()) {
			if (!parseDishLine(line, storage)) {
				invalidCount++;
				std::cout << "Невалидная строка " << lineNum << ": " << line << std::endl;
			}
		}
	}
	file.close();
}

bool MenuFileParser::parseDishLine(const std::string& line, IMenuStorage& storage) {
	std::string name;
	double price = 0.0;
	Time time;

	if (extractValuesWithRegex(line, name, price, time)) {
		// Проверяем только цену (должна быть положительной)
		if (price <= 0) {
			std::cout << "Отсеяно: отрицательная или нулевая цена: " << price << std::endl;
			return false;
		}

		// Используем setTime для установки времени
		Time normalizedTime;
		normalizedTime.setTime(time.hours, time.minutes);

		storage.addDish(name, price, normalizedTime.hours, normalizedTime.minutes);
		std::cout << "Добавлено блюдо: " << name << " цена: " << price << " время: " << normalizedTime.toString() << std::endl;
		return true;
	}

	return false;
}

bool MenuFileParser::extractValuesWithRegex(const std::string& line, std::string& name, double& price, Time& time) {
	std::string workingLine = StringUtils::trim(line);
	if (workingLine.empty()) return false;

	// Ищем название в кавычках
	std::smatch name_match;
	if (!std::regex_search(workingLine, name_match, name_pattern_)) {
		std::cout << "Не найдено название в кавычках в строке: " << workingLine << std::endl;
		return false;
	}
	name = name_match[1].str();
	if (name.empty()) {
		std::cout << "Пустое название" << std::endl;
		return false;
	}

	// Удаляем найденное название из строки для поиска остальных значений
	std::string remaining = std::regex_replace(workingLine, name_pattern_, "", std::regex_constants::format_first_only);

	// Ищем время
	std::smatch time_match;
	if (!std::regex_search(remaining, time_match, time_pattern_)) {
		std::cout << "Не найдено время в формате чч:мм в строке: " << remaining << std::endl;
		return false;
	}

	// Парсим время
	if (!timeParser_->parse(time_match[0].str())) {
		std::cout << "Ошибка парсинга времени: " << time_match[0].str() << std::endl;
		return false;
	}
	time = timeParser_->getResult();

	// Удаляем найденное время из строки
	remaining = std::regex_replace(remaining, time_pattern_, "", std::regex_constants::format_first_only);

	// Ищем цену в оставшейся строке
	std::smatch price_match;
	if (!std::regex_search(remaining, price_match, std::regex(price_pattern_))) {
		std::cout << "Не найдена цена в строке: " << remaining << std::endl;
		return false;
	}

	// Парсим цену
	if (!numberParser_->parse(price_match[0].str())) {
		std::cout << "Ошибка парсинга цены: " << price_match[0].str() << std::endl;
		return false;
	}
	price = numberParser_->getResult();

	// Проверяем, что в строке не осталось лишних данных
	remaining = std::regex_replace(remaining, std::regex(price_pattern_), "", std::regex_constants::format_first_only);
	remaining = StringUtils::trim(remaining);

	if (!remaining.empty()) {
		std::cout << "Обнаружены лишние данные в строке: '" << remaining << "'" << std::endl;
		return false;
	}

	return true;
}