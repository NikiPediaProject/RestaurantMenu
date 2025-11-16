#pragma once
#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include "interfaces.h"
#include "parsers.h"
#include <memory>
#include <regex>
#include <string>

/// Парсер файлов меню с использованием регулярных выражений
class MenuFileParser : public IFileParser {
private:
	std::unique_ptr<NumberParser> numberParser_;    ///< Парсер для цен
	std::unique_ptr<TimeParser> timeParser_;        ///< Парсер для времени

	// Регулярные выражения для различных компонентов строки меню
	std::regex name_pattern_;   ///< Шаблон для извлечения названия в кавычках
	std::regex time_pattern_;   ///< Шаблон для извлечения времени в формате чч:мм
	std::regex price_pattern_;  ///< Шаблон для извлечения числовых цен

public:
	MenuFileParser();

	/// Парсит файл с меню, заполняет хранилище и подсчитывает невалидные строки
	void parseFile(const std::string& filename, IMenuStorage& storage, int& invalidCount) override;

	/// Публичный метод для извлечения значений из строки с помощью regex
	bool extractValuesWithRegex(const std::string& line, std::string& name, double& price, Time& time);

private:
	/// Внутренний метод для парсинга отдельной строки с информацией о блюде
	bool parseDishLine(const std::string& line, IMenuStorage& storage);
};

#endif // FILE_PARSER_H