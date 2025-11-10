#include "parsers.h"

// ==================== РЕАЛИЗАЦИЯ NUMBER PARSER ====================

/// Конструктор парсера чисел - инициализирует состояние
NumberParser::NumberParser() : result_(0.0), isValid_(false) {}

/// Парсит строку в числовое значение с поддержкой дробных чисел и заменой запятых
bool NumberParser::parse(const std::string& str) {
	reset(); // Сбрасываем предыдущее состояние

	if (str.empty()) return false;

	std::string numberStr = str;

	// Заменяем запятые на точки для унификации формата дробных чисел
	for (char& c : numberStr) {
		if (c == ',') c = '.';
	}

	// Проверяем, что строка содержит только допустимые символы
	for (char c : numberStr) {
		if (!StringUtils::isDigitASCII(c) && c != '.' && c != '-') {
			return false;
		}
	}

	// Используем stringstream для преобразования строки в число
	std::stringstream ss(numberStr);
	double value;
	ss >> value;

	// Проверяем успешность преобразования и полное потребление строки
	if (ss.fail() || !ss.eof()) return false;

	result_ = value;
	isValid_ = true;
	return true;
}

/// Возвращает статус валидности последней операции парсинга
bool NumberParser::isValid() const {
	return isValid_;
}

/// Возвращает результат успешного парсинга (только при isValid() == true)
double NumberParser::getResult() const {
	return result_;
}

/// Сбрасывает парсер в начальное состояние
void NumberParser::reset() {
	result_ = 0.0;
	isValid_ = false;
}

// ==================== РЕАЛИЗАЦИЯ TIME PARSER ====================

/// Конструктор парсера времени - инициализирует нулевым временем
TimeParser::TimeParser() : result_(0, 0), isValid_(false) {}

/// Статический метод для проверки валидности объекта Time
bool TimeParser::isValidTime(const Time& time) {
	// Разрешаем любые неотрицательные значения - нормализация преобразует их корректно
	return time.getHours() >= 0 && time.getMinutes() >= 0;
}

/// Парсит строку времени в формате "чч:мм" с автоматической нормализацией
bool TimeParser::parse(const std::string& token) {
	reset(); // Сбрасываем предыдущее состояние

	std::string timeStr = StringUtils::trim(token);

	// Удаляем все пробелы для обработки форматов типа "12 : 30"
	timeStr.erase(std::remove(timeStr.begin(), timeStr.end(), ' '), timeStr.end());

	// Ищем разделитель - двоеточие
	size_t colonPos = timeStr.find(':');
	if (colonPos == std::string::npos) return false;

	try {
		// Извлекаем компоненты часа и минуты
		std::string hourStr = timeStr.substr(0, colonPos);
		std::string minStr = timeStr.substr(colonPos + 1);

		// Проверяем, что обе части состоят только из цифр
		if (!StringUtils::isOnlyDigits(hourStr) || !StringUtils::isOnlyDigits(minStr)) {
			return false;
		}

		// Преобразуем строки в числа
		int h = std::stoi(hourStr);
		int m = std::stoi(minStr);

		// Создаем временный объект Time - конструктор выполнит нормализацию
		Time tempTime(h, m);
		if (isValidTime(tempTime)) {
			result_ = tempTime;
			isValid_ = true;
			return true;
		}
	}
	catch (...) {
		// Ловим любые исключения при преобразовании строк в числа
		return false;
	}
	return false;
}

/// Возвращает статус валидности последней операции парсинга
bool TimeParser::isValid() const {
	return isValid_;
}

/// Возвращает результат успешного парсинга (только при isValid() == true)
Time TimeParser::getResult() const {
	return result_;
}

/// Сбрасывает парсер в начальное состояние
void TimeParser::reset() {
	result_ = Time(0, 0);
	isValid_ = false;
}

// ==================== РЕАЛИЗАЦИЯ USER INPUT PARSER ====================

/// Конструктор композитного парсера - создает дочерние парсеры
UserInputParser::UserInputParser()
	: timeParser_(std::make_unique<TimeParser>())
	, numberParser_(std::make_unique<NumberParser>())
{}

/// Анализирует пользовательский ввод и извлекает критерии фильтрации
bool UserInputParser::parse(const std::string& input, double& price, Time& time) {
	std::istringstream iss(input);
	std::string token;
	bool hasPrice = false;
	bool hasTime = false;

	// Обрабатываем каждый токен входной строки последовательно
	while (iss >> token) {
		// Сначала пробуем распарсить как время
		if (!hasTime && timeParser_->parse(token)) {
			time = timeParser_->getResult();
			hasTime = true;
			continue; // Переходим к следующему токену
		}

		// Затем пробуем распарсить как цену
		if (!hasPrice && numberParser_->parse(token)) {
			price = numberParser_->getResult();
			hasPrice = true;
		}
	}

	// Возвращаем true если найден хотя бы один критерий фильтрации
	return hasPrice || hasTime;
}