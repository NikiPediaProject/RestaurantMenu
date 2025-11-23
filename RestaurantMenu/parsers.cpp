#include "parsers.h"
#include <algorithm>

// ==================== NUMBER PARSER ====================

NumberParser::NumberParser() : result_(0.0), isValid_(false), errorMessage_("") {}

bool NumberParser::parse(const std::string& str) {
	reset();
	if (str.empty()) {
		errorMessage_ = "Ошибка: пустая строка";
		return false;
	}

	std::string numberStr = str;
	// Заменяем запятые на точки для корректного парсинга
	for (char& c : numberStr) {
		if (c == ',') c = '.';
	}

	// Проверяем допустимые символы
	for (char c : numberStr) {
		if (!StringUtils::isDigitASCII(c) && c != '.' && c != '-') {
			errorMessage_ = "Ошибка: строка содержит недопустимые символы";
			return false;
		}
	}

	std::stringstream ss(numberStr);
	double value;
	ss >> value;

	if (ss.fail() || !ss.eof()) {
		errorMessage_ = "Ошибка: не удалось преобразовать в число";
		return false;
	}

	result_ = value;
	isValid_ = true;
	return true;
}

bool NumberParser::isValid() const {
	return isValid_;
}

double NumberParser::getResult() const {
	return result_;
}

std::string NumberParser::getErrorMessage() const {
	return errorMessage_;
}

void NumberParser::reset() {
	result_ = 0.0;
	isValid_ = false;
	errorMessage_ = "";
}

// ==================== TIME PARSER ====================

TimeParser::TimeParser() : result_(0, 0), isValid_(false), errorMessage_("") {}

bool TimeParser::isValidTime(const Time& time) {
	return time.getHours() >= 0 && time.getMinutes() >= 0 && time.getMinutes() < 60;
}

bool TimeParser::parse(const std::string& token) {
	reset();
	std::string timeStr = StringUtils::trim(token);
	timeStr.erase(std::remove(timeStr.begin(), timeStr.end(), ' '), timeStr.end());

	if (timeStr.empty()) {
		errorMessage_ = "Ошибка: пустая строка времени";
		return false;
	}

	size_t colonPos = timeStr.find(':');
	if (colonPos == std::string::npos) {
		errorMessage_ = "Ошибка: отсутствует двоеточие в формате времени";
		return false;
	}

	try {
		std::string hourStr = timeStr.substr(0, colonPos);
		std::string minStr = timeStr.substr(colonPos + 1);

		// Проверяем, что обе части состоят только из цифр
		if (!StringUtils::isOnlyDigits(hourStr) || !StringUtils::isOnlyDigits(minStr)) {
			errorMessage_ = "Ошибка: часы или минуты содержат нецифровые символы";
			return false;
		}

		int h = std::stoi(hourStr);
		int m = std::stoi(minStr);

		// Проверяем валидность минут
		if (m < 0 || m >= 60) {
			errorMessage_ = "Ошибка: минуты должны быть в диапазоне 0-59";
			return false;
		}

		// Создаем Time - он сам выполнит нормализацию
		Time tempTime(h, m);
		if (isValidTime(tempTime)) {
			result_ = tempTime;
			isValid_ = true;
			return true;
		}
		else {
			errorMessage_ = "Ошибка: некорректное время";
			return false;
		}
	}
	catch (const std::exception& e) {
		errorMessage_ = std::string("Ошибка: исключение при парсинге времени - ") + e.what();
		return false;
	}
	catch (...) {
		errorMessage_ = "Ошибка: неизвестное исключение при парсинге времени";
		return false;
	}
}

bool TimeParser::isValid() const {
	return isValid_;
}

Time TimeParser::getResult() const {
	return result_;
}

std::string TimeParser::getErrorMessage() const {
	return errorMessage_;
}

void TimeParser::reset() {
	result_ = Time(0, 0);
	isValid_ = false;
	errorMessage_ = "";
}

// ==================== USER INPUT PARSER ====================

UserInputParser::UserInputParser()
	: timeParser_(std::make_unique<TimeParser>())
	, numberParser_(std::make_unique<NumberParser>())
{}

bool UserInputParser::parse(const std::string& input, double& price, Time& time, std::string& errorMessage) {
	std::istringstream iss(input);
	std::string token;
	bool hasPrice = false;
	bool hasTime = false;
	price = 0.0;
	time = Time();
	errorMessage = "";

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

	if (hasPrice || hasTime) {
		return true;
	}
	else {
		errorMessage = "Ошибка: не удалось извлечь ни цену, ни время";
		return false;
	}
}