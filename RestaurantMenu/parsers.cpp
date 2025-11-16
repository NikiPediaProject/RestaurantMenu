#include "parsers.h"
#include <algorithm>

// ==================== NUMBER PARSER ====================

// Конструктор парсера чисел
NumberParser::NumberParser() : result_(0.0), isValid_(false) {}

// Парсит строку в числовое значение
bool NumberParser::parse(const std::string& str) {
	reset();
	if (str.empty()) return false;

	std::string numberStr = str;
	// Заменяем запятые на точки для корректного парсинга
	for (char& c : numberStr) {
		if (c == ',') c = '.';
	}

	// Проверяем допустимые символы
	for (char c : numberStr) {
		if (!StringUtils::isDigitASCII(c) && c != '.' && c != '-') {
			return false;
		}
	}

	std::stringstream ss(numberStr);
	double value;
	ss >> value;

	if (ss.fail() || !ss.eof()) return false;

	result_ = value;
	isValid_ = true;
	return true;
}

// Проверяет валидность результата
bool NumberParser::isValid() const {
	return isValid_;
}

// Возвращает результат парсинга
double NumberParser::getResult() const {
	return result_;
}

// Сбрасывает парсер в начальное состояние
void NumberParser::reset() {
	result_ = 0.0;
	isValid_ = false;
}

// ==================== TIME PARSER ====================

// Конструктор парсера времени
TimeParser::TimeParser() : result_(0, 0), isValid_(false) {}

// Проверяет валидность объекта Time
bool TimeParser::isValidTime(const Time& time) {
	return time.getHours() >= 0 && time.getMinutes() >= 0;
}

// Парсит строку в объект Time
bool TimeParser::parse(const std::string& token) {
	reset();
	std::string timeStr = StringUtils::trim(token);
	timeStr.erase(std::remove(timeStr.begin(), timeStr.end(), ' '), timeStr.end());

	size_t colonPos = timeStr.find(':');
	if (colonPos == std::string::npos) return false;

	try {
		std::string hourStr = timeStr.substr(0, colonPos);
		std::string minStr = timeStr.substr(colonPos + 1);

		// Проверяем, что обе части состоят только из цифр
		if (!StringUtils::isOnlyDigits(hourStr) || !StringUtils::isOnlyDigits(minStr)) {
			return false;
		}

		int h = std::stoi(hourStr);
		int m = std::stoi(minStr);

		// Создаем Time - он сам выполнит нормализацию
		Time tempTime(h, m);
		if (isValidTime(tempTime)) {
			result_ = tempTime;
			isValid_ = true;
			return true;
		}
	}
	catch (...) {
		return false;
	}
	return false;
}

// Проверяет валидность результата
bool TimeParser::isValid() const {
	return isValid_;
}

// Возвращает результат парсинга
Time TimeParser::getResult() const {
	return result_;
}

// Сбрасывает парсер в начальное состояние
void TimeParser::reset() {
	result_ = Time(0, 0);
	isValid_ = false;
}

// ==================== USER INPUT PARSER ====================

// Конструктор парсера пользовательского ввода
UserInputParser::UserInputParser()
	: timeParser_(std::make_unique<TimeParser>())
	, numberParser_(std::make_unique<NumberParser>())
{}

// Анализирует пользовательский ввод и извлекает критерии фильтрации
bool UserInputParser::parse(const std::string& input, double& price, Time& time) {
	std::istringstream iss(input);
	std::string token;
	bool hasPrice = false;
	bool hasTime = false;

	// Обрабатываем каждый токен входной строки
	while (iss >> token) {
		// Сначала пробуем распарсить как время
		if (!hasTime && timeParser_->parse(token)) {
			time = timeParser_->getResult();
			hasTime = true;
			continue;
		}

		// Затем пробуем распарсить как цену
		if (!hasPrice && numberParser_->parse(token)) {
			price = numberParser_->getResult();
			hasPrice = true;
		}
	}

	return hasPrice || hasTime;
}