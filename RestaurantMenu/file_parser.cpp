#include "file_parser.h"
#include "utils.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

// Вспомогательная функция для поиска всех совпадений по регулярному выражению
static std::vector<std::string> findAllMatches(const std::string& str, const std::regex& regex, int group = 0) {
	std::vector<std::string> matches;
	std::sregex_iterator it(str.begin(), str.end(), regex);
	std::sregex_iterator end;
	for (; it != end; ++it) {
		matches.push_back((*it)[group].str());
	}
	return matches;
}

// Единая функция для поиска значения по регулярному выражению с удалением найденного
static bool findAndExtractValue(std::string& str, const std::regex& pattern, std::string& result, int group = 0) {
	std::smatch match;
	if (std::regex_search(str, match, pattern)) {
		result = match[group].str();
		// Удаляем найденное значение из строки
		str = std::regex_replace(str, pattern, "", std::regex_constants::format_first_only);
		return true;
	}
	return false;
}

// Конструктор парсера файлов
MenuFileParser::MenuFileParser()
	: numberParser_(std::make_unique<NumberParser>())
	, timeParser_(std::make_unique<TimeParser>())
	, name_pattern_("\"([^\"]+)\"")                    // Название в двойных кавычках
	, time_pattern_(R"(\b(\d+:(?:[0-5]?\d|59))\b)")   // Время: часы любые, минуты 0-59
	, price_pattern_(R"(\b(-?\d+[.,]\d+)\b)")         // Цена: строго дробное с точкой или запятой
{}

// Парсит файл с меню, заполняет хранилище и подсчитывает невалидные строки
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

// Парсит отдельную строку с информацией о блюде
bool MenuFileParser::parseDishLine(const std::string& line, IMenuStorage& storage) {
	std::string name;
	double price = 0.0;
	Time time;

	if (extractValuesWithRegex(line, name, price, time)) {
		// Проверяем только цену (должна быть положительной)
		if (price <= 0) {
			std::cout << "- Отсеяно: отрицательная или нулевая цена: " << price << std::endl;
			return false;
		}

		// Передаём полный объект Time
		storage.addDish(name, price, time);
		std::cout << "+ Добавлено блюдо: " << name << " цена: " << price << " время: " << time.toString() << std::endl;
		return true;
	}

	return false;
}

// Извлекает значения из строки с помощью регулярных выражений
bool MenuFileParser::extractValuesWithRegex(const std::string& line, std::string& name, double& price, Time& time) {
	std::string workingLine = StringUtils::trim(line);
	if (workingLine.empty()) return false;

	std::string nameResult, timeResult, priceResult;

	// Используем единую функцию для поиска и извлечения значений
	if (!findAndExtractValue(workingLine, name_pattern_, nameResult, 1)) {
		std::cout << "- Не найдено название в кавычках" << std::endl;
		return false;
	}
	name = nameResult;

	if (!findAndExtractValue(workingLine, time_pattern_, timeResult)) {
		std::cout << "- Не найдено время в формате чч:мм" << std::endl;
		return false;
	}

	if (!findAndExtractValue(workingLine, std::regex(price_pattern_), priceResult)) {
		std::cout << "- Не найдена цена" << std::endl;
		return false;
	}

	// Парсим время
	if (!timeParser_->parse(timeResult)) {
		std::cout << "- Ошибка парсинга времени: " << timeResult << std::endl;
		return false;
	}
	time = timeParser_->getResult();

	// Парсим цену
	if (!numberParser_->parse(priceResult)) {
		std::cout << "- Ошибка парсинга цены: " << priceResult << std::endl;
		return false;
	}
	price = numberParser_->getResult();

	// Проверяем, что в строке не осталось лишних данных
	workingLine = StringUtils::trim(workingLine);
	if (!workingLine.empty()) {
		std::cout << "- Обнаружены лишние данные в строке: '" << workingLine << "'" << std::endl;
		return false;
	}

	return true;
}