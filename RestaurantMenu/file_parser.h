#pragma once
#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include "interfaces.h"
#include "parsers.h"
#include <memory>
#include <regex>
#include <string>

//Функция ручного ввода(без валидации, тип функции void)
void addMenuItem(IMenuStorage& storage, const std::string& name, double price, const Time& time);

/// Парсер для файлов с меню с использованием регулярных выражений
class MenuFileParser : public IFileParser {
private:
	std::unique_ptr<NumberParser> numberParser_;
	std::unique_ptr<TimeParser> timeParser_;

	// Регулярные выражения для проверки форматов
	std::regex name_pattern_;
	std::regex time_pattern_;
	std::regex price_pattern_;

public:
	MenuFileParser();

	/// Парсит файл с меню и заполняет хранилище
	void parseFile(const std::string& filename, IMenuStorage& storage, int& invalidCount) override;

	/// Извлекает значения из строки с помощью regex (публичный метод)
	bool extractValuesWithRegex(const std::string& line, std::string& name, double& price, Time& time);

private:
	/// Парсит строку с информацией о блюде с использованием regex
	bool parseDishLine(const std::string& line, IMenuStorage& storage);
};

#endif // FILE_PARSER_H
