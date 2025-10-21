#pragma once
#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include "interfaces.h"
#include "parsers.h"
#include "utils.h"
#include <fstream>
#include <stdexcept>

/// Парсер для файлов с меню
class MenuFileParser : public IFileParser {
private:
	std::unique_ptr<NumberParser> numberParser_;
	std::unique_ptr<TimeParser> timeParser_;

public:
	MenuFileParser();

	void parseFile(const std::string& filename, IMenuStorage& storage, int& invalidCount) override;

private:
	/// Парсит строку с информацией о блюде
	bool parseDishLine(const std::string& line, IMenuStorage& storage);

	/// Извлекает название блюда из кавычек
	bool extractQuotedName(const std::string& line, std::string& name, std::string& remaining);
};

#endif // FILE_PARSER_H