#pragma once
#ifndef PARSERS_H
#define PARSERS_H

#include "interfaces.h"
#include "models.h"
#include "utils.h"
#include <memory>
#include <sstream>
#include <vector>
#include <algorithm>

/// Парсер для числовых значений
class NumberParser : public IParser {
private:
	double result_;
	bool isValid_;

public:
	NumberParser();
	bool parse(const std::string& str) override;
	bool isValid() const override;
	double getResult() const;
	void reset() override;
};

/// Парсер для временных значений формата "чч:мм"
class TimeParser : public IParser {
private:
	Time result_;
	bool isValid_;

public:
	TimeParser();
	static bool isValidTime(const Time& time);
	bool parse(const std::string& token) override;
	bool isValid() const override;
	Time getResult() const;
	void reset() override;
};

/// Парсер пользовательского ввода для фильтрации
class UserInputParser {
private:
	std::vector<std::unique_ptr<IParser>> parsers_;
	bool hasPrice_;
	bool hasTime_;
	double price_;
	Time time_;

public:
	UserInputParser();

	/// Добавляет новый парсер в цепочку
	void addParser(std::unique_ptr<IParser> parser);

	/// Парсит пользовательский ввод для извлечения критериев фильтрации
	void parse(const std::string& input);

	bool hasPrice() const;
	bool hasTime() const;
	double getPrice() const;
	Time getTime() const;

private:
	/// Сбрасывает состояние парсера
	void reset();

	/// Обрабатывает успешно распарсенный токен
	void handleParsedToken(IParser& parser);
};

#endif // PARSERS_H