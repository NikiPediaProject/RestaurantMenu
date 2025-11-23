#pragma once
#ifndef PARSERS_H
#define PARSERS_H

#include "interfaces.h"
#include "models.h"
#include "utils.h"
#include <memory>
#include <sstream>

/// Парсер для числовых значений с поддержкой дробных чисел
class NumberParser : public IParser {
private:
	double result_;     ///< Результат успешного парсинга
	bool isValid_;      ///< Флаг валидности последней операции
	std::string errorMessage_; ///< Сообщение об ошибке

public:
	NumberParser();

	/// Парсит строку в числовое значение, поддерживает запятые и точки как разделители
	bool parse(const std::string& str) override;

	/// Возвращает true, если последняя операция парсинга была успешной
	bool isValid() const override;

	/// Возвращает результат парсинга (только если isValid() == true)
	double getResult() const;

	/// Возвращает сообщение об ошибке
	std::string getErrorMessage() const;

	/// Сбрасывает парсер в начальное состояние
	void reset() override;
};

/// Парсер для временных значений в формате "чч:мм" с автоматической нормализацией
class TimeParser : public IParser {
private:
	Time result_;       ///< Результат успешного парсинга
	bool isValid_;      ///< Флаг валидности последней операции
	std::string errorMessage_; ///< Сообщение об ошибке

public:
	TimeParser();

	/// Статический метод для проверки валидности объекта Time
	static bool isValidTime(const Time& time);

	/// Парсит строку в объект Time, поддерживает форматы "чч:мм", "дд:чч:мм" и т.д.
	bool parse(const std::string& token) override;

	/// Возвращает true, если последняя операция парсинга была успешной
	bool isValid() const override;

	/// Возвращает результат парсинга (только если isValid() == true)
	Time getResult() const;

	/// Возвращает сообщение об ошибке
	std::string getErrorMessage() const;

	/// Сбрасывает парсер в начальное состояние
	void reset() override;
};

/// Композитный парсер для обработки пользовательского ввода в интерактивном режиме
class UserInputParser {
private:
	std::unique_ptr<TimeParser> timeParser_;    ///< Парсер для временных значений
	std::unique_ptr<NumberParser> numberParser_; ///< Парсер для числовых значений

public:
	UserInputParser();

	/// Анализирует пользовательский ввод и извлекает критерии фильтрации (цена и/или время)
	bool parse(const std::string& input, double& price, Time& time, std::string& errorMessage);
};

#endif // PARSERS_H