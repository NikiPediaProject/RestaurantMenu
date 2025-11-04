#include "file_parser.h"
#include "utils.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

// ==================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ====================

/// Вспомогательная функция для поиска всех совпадений по регулярному выражению
static std::vector<std::string> findAllMatches(const std::string& str, const std::regex& regex, int group = 0) {
	std::vector<std::string> matches;
	std::sregex_iterator it(str.begin(), str.end(), regex);
	std::sregex_iterator end;

	// Собираем все совпадения в вектор
	for (; it != end; ++it) {
		matches.push_back((*it)[group].str());
	}
	return matches;
}

// ==================== РЕАЛИЗАЦИЯ MENU FILE PARSER ====================

/// Конструктор парсера файлов - инициализирует регулярные выражения и дочерние парсеры
MenuFileParser::MenuFileParser()
	: numberParser_(std::make_unique<NumberParser>())
	, timeParser_(std::make_unique<TimeParser>())
	, name_pattern_("\"([^\"]+)\"")                    // Название в двойных кавычках
	, time_pattern_(R"(\b(\d+:\d+)\b)")               // Время в формате чч:мм (любые числа)
	, price_pattern_(R"(\b(-?\d+(?:[.,]\d+)?)\b)")    // Цена (дробные числа с точкой или запятой)
{}

/// Парсит файл с меню, заполняет хранилище и подсчитывает невалидные строки
void MenuFileParser::parseFile(const std::string& filename, IMenuStorage& storage, int& invalidCount) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Ошибка открытия файла!");
	}

	std::string line;
	int lineNum = 0;

	// Читаем файл построчно
	while (std::getline(file, line)) {
		++lineNum;
		if (!line.empty()) {
			// Парсим каждую непустую строку
			if (!parseDishLine(line, storage)) {
				++invalidCount;
				std::cout << "Невалидная строка " << lineNum << ": " << line << std::endl;
			}
		}
	}
	file.close();
}

/// Парсит отдельную строку с информацией о блюде и добавляет в хранилище при успехе
bool MenuFileParser::parseDishLine(const std::string& line, IMenuStorage& storage) {
	std::string name;
	double price = 0.0;
	Time time;

	// Используем regex для извлечения значений из строки
	if (extractValuesWithRegex(line, name, price, time)) {
		// Проверяем только цену (должна быть положительной)
		if (price <= 0) {
			std::cout << "Отсеяно: отрицательная или нулевая цена: " << price << std::endl;
			return false;
		}

		// Передаём полный объект Time в хранилище
		storage.addDish(name, price, time);
		std::cout << "Добавлено блюдо: " << name << " цена: " << price << " время: " << time.toString() << std::endl;
		return true;
	}

	return false;
}

/// Извлекает значения названия, цены и времени из строки с помощью регулярных выражений
bool MenuFileParser::extractValuesWithRegex(const std::string& line, std::string& name, double& price, Time& time) {
	std::string workingLine = StringUtils::trim(line);
	if (workingLine.empty()) return false;

	// Сначала ищем название - оно должно быть в кавычках
	std::smatch name_match;
	if (!std::regex_search(workingLine, name_match, name_pattern_)) {
		std::cout << "Не найдено название в кавычках" << std::endl;
		return false;
	}
	name = name_match[1].str(); // Берем содержимое кавычек (первая группа)

	// Удаляем найденное название из строки для дальнейшего анализа
	std::string remaining = std::regex_replace(workingLine, name_pattern_, "", std::regex_constants::format_first_only);
	remaining = StringUtils::trim(remaining);

	// Теперь ищем время в оставшейся строке
	std::smatch time_match;
	if (!std::regex_search(remaining, time_match, time_pattern_)) {
		std::cout << "Не найдено время в формате чч:мм" << std::endl;
		return false;
	}

	// Парсим найденное время
	if (!timeParser_->parse(time_match[1].str())) {
		std::cout << "Ошибка парсинга времени: " << time_match[1].str() << std::endl;
		return false;
	}
	time = timeParser_->getResult();

	// Удаляем найденное время из строки
	remaining = std::regex_replace(remaining, time_pattern_, "", std::regex_constants::format_first_only);
	remaining = StringUtils::trim(remaining);

	// Ищем цену в оставшейся строке
	std::smatch price_match;
	if (!std::regex_search(remaining, price_match, std::regex(price_pattern_))) {
		std::cout << "Не найдена цена" << std::endl;
		return false;
	}

	// Парсим найденную цену
	if (!numberParser_->parse(price_match[1].str())) {
		std::cout << "Ошибка парсинга цены: " << price_match[1].str() << std::endl;
		return false;
	}
	price = numberParser_->getResult();

	// Удаляем найденную цену из строки
	remaining = std::regex_replace(remaining, std::regex(price_pattern_), "", std::regex_constants::format_first_only);
	remaining = StringUtils::trim(remaining);

	// Проверяем, что в строке не осталось лишних данных
	if (!remaining.empty()) {
		std::cout << "Обнаружены лишние данные в строке: '" << remaining << "'" << std::endl;
		return false;
	}

	return true;
}