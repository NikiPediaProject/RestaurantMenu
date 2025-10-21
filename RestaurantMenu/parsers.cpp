#include "parsers.h"

// Реализации NumberParser

NumberParser::NumberParser() : result_(0.0), isValid_(false) {}

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

bool NumberParser::isValid() const { return isValid_; }
double NumberParser::getResult() const { return result_; }
void NumberParser::reset() { result_ = 0.0; isValid_ = false; }

// Реализации TimeParser

TimeParser::TimeParser() : result_(0, 0), isValid_(false) {}

bool TimeParser::isValidTime(const Time& time) {
	return time.hours >= 0 && time.minutes >= 0;
}

Time TimeParser::normalizeTime(const Time& time) {
	Time result = time;
	if (result.minutes >= 60) {
		result.hours += result.minutes / 60;
		result.minutes = result.minutes % 60;
	}
	if (result.hours > 100) {
		result.hours = 100;
		result.minutes = 0;
	}
	return result;
}

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

bool TimeParser::isValid() const { return isValid_; }
Time TimeParser::getResult() const { return result_; }
Time TimeParser::getNormalizedResult() const { return normalizeTime(result_); }
void TimeParser::reset() { result_ = Time(0, 0); isValid_ = false; }

// Реализации UserInputParser

UserInputParser::UserInputParser() : hasPrice_(false), hasTime_(false), price_(0.0), time_(0, 0) {
	// Инициализируем парсеры
	parsers_.push_back(std::make_unique<TimeParser>());
	parsers_.push_back(std::make_unique<NumberParser>());
}

void UserInputParser::addParser(std::unique_ptr<IParser> parser) {
	parsers_.push_back(std::move(parser));
}

void UserInputParser::parse(const std::string& input) {
	reset();
	std::istringstream iss(input);
	std::string token;

	// Обрабатываем каждый токен входной строки
	while (iss >> token) {
		for (auto& parser : parsers_) {
			parser->reset();
			if (parser->parse(token)) {
				handleParsedToken(*parser);
				break;
			}
		}
	}
}

bool UserInputParser::hasPrice() const { return hasPrice_; }
bool UserInputParser::hasTime() const { return hasTime_; }
double UserInputParser::getPrice() const { return price_; }
Time UserInputParser::getTime() const { return TimeParser::normalizeTime(time_); }

void UserInputParser::reset() {
	hasPrice_ = false;
	hasTime_ = false;
	price_ = 0.0;
	time_ = Time(0, 0);
}

void UserInputParser::handleParsedToken(IParser& parser) {
	if (auto timeParser = dynamic_cast<TimeParser*>(&parser)) {
		if (!hasTime_) {
			time_ = timeParser->getResult();
			hasTime_ = true;
		}
	}
	else if (auto numberParser = dynamic_cast<NumberParser*>(&parser)) {
		if (!hasPrice_) {
			price_ = numberParser->getResult();
			hasPrice_ = true;
		}
	}
}