#include <gtest/gtest.h>
#include "../RestaurantMenu/parsers.h"

// ==================== NUMBER PARSER TESTS ====================

/// Тестирование парсера чисел с различными валидными входными данными
TEST(NumberParserTest, ValidNumbers) {
	NumberParser parser;

	// Тест целого числа
	EXPECT_TRUE(parser.parse("42"));
	EXPECT_TRUE(parser.isValid());
	EXPECT_DOUBLE_EQ(parser.getResult(), 42.0);

	// Тест дробного числа с точкой
	EXPECT_TRUE(parser.parse("12.50"));
	EXPECT_TRUE(parser.isValid());
	EXPECT_DOUBLE_EQ(parser.getResult(), 12.50);

	// Тест дробного числа с запятой (должна конвертироваться в точку)
	EXPECT_TRUE(parser.parse("7,99"));
	EXPECT_TRUE(parser.isValid());
	EXPECT_DOUBLE_EQ(parser.getResult(), 7.99);

	// Тест отрицательного числа
	EXPECT_TRUE(parser.parse("-5.25"));
	EXPECT_TRUE(parser.isValid());
	EXPECT_DOUBLE_EQ(parser.getResult(), -5.25);
}

/// Тестирование парсера чисел с невалидными входными данными
TEST(NumberParserTest, InvalidNumbers) {
	NumberParser parser;

	// Пустая строка
	EXPECT_FALSE(parser.parse(""));
	EXPECT_FALSE(parser.isValid());
	EXPECT_FALSE(parser.getErrorMessage().empty());

	// Нечисловые символы
	EXPECT_FALSE(parser.parse("abc"));
	EXPECT_FALSE(parser.isValid());
	EXPECT_FALSE(parser.getErrorMessage().empty());

	// Неправильный формат числа
	EXPECT_FALSE(parser.parse("12.50.30"));
	EXPECT_FALSE(parser.isValid());
	EXPECT_FALSE(parser.getErrorMessage().empty());

	// Только символы пунктуации
	EXPECT_FALSE(parser.parse(".."));
	EXPECT_FALSE(parser.isValid());
	EXPECT_FALSE(parser.getErrorMessage().empty());
}

/// Тестирование сброса состояния парсера
TEST(NumberParserTest, ResetFunctionality) {
	NumberParser parser;

	// Парсим валидное число
	EXPECT_TRUE(parser.parse("42.5"));
	EXPECT_TRUE(parser.isValid());

	// Сбрасываем парсер
	parser.reset();

	// Проверяем, что состояние сброшено
	EXPECT_FALSE(parser.isValid());
	EXPECT_DOUBLE_EQ(parser.getResult(), 0.0);
	EXPECT_TRUE(parser.getErrorMessage().empty());
}

// ==================== TIME PARSER TESTS ====================

/// Тестирование парсера времени с валидными форматами
TEST(TimeParserTest, ValidTime) {
	TimeParser parser;

	// Стандартный формат времени
	EXPECT_TRUE(parser.parse("12:30"));
	EXPECT_TRUE(parser.isValid());
	Time time = parser.getResult();
	EXPECT_EQ(time.getHours(), 12);
	EXPECT_EQ(time.getMinutes(), 30);

	// Время с однозначными числами
	EXPECT_TRUE(parser.parse("1:5"));
	EXPECT_TRUE(parser.isValid());
	time = parser.getResult();
	EXPECT_EQ(time.getHours(), 1);
	EXPECT_EQ(time.getMinutes(), 5);

	// Граничные значения
	EXPECT_TRUE(parser.parse("0:0"));
	EXPECT_TRUE(parser.isValid());
	time = parser.getResult();
	EXPECT_EQ(time.getHours(), 0);
	EXPECT_EQ(time.getMinutes(), 0);

	EXPECT_TRUE(parser.parse("23:59"));
	EXPECT_TRUE(parser.isValid());
	time = parser.getResult();
	EXPECT_EQ(time.getHours(), 23);
	EXPECT_EQ(time.getMinutes(), 59);
}

/// Тестирование парсера времени с невалидными данными
TEST(TimeParserTest, InvalidTime) {
	TimeParser parser;

	// Пустая строка
	EXPECT_FALSE(parser.parse(""));
	EXPECT_FALSE(parser.isValid());
	EXPECT_FALSE(parser.getErrorMessage().empty());

	// Отсутствие двоеточия
	EXPECT_FALSE(parser.parse("1230"));
	EXPECT_FALSE(parser.isValid());
	EXPECT_FALSE(parser.getErrorMessage().empty());

	// Неправильные минуты
	EXPECT_FALSE(parser.parse("12:60"));
	EXPECT_FALSE(parser.isValid());
	EXPECT_FALSE(parser.getErrorMessage().empty());

	// Отрицательное время
	EXPECT_FALSE(parser.parse("-1:30"));
	EXPECT_FALSE(parser.isValid());
	EXPECT_FALSE(parser.getErrorMessage().empty());

	// Нечисловые символы
	EXPECT_FALSE(parser.parse("ab:cd"));
	EXPECT_FALSE(parser.isValid());
	EXPECT_FALSE(parser.getErrorMessage().empty());
}

/// Тестирование автоматической нормализации времени
TEST(TimeParserTest, TimeNormalization) {
    TimeParser parser;

    // Парсер должен отклонять неправильные минуты (>= 60)
    EXPECT_FALSE(parser.parse("1:75"));
    EXPECT_FALSE(parser.isValid());
    
    // Вместо этого тестируем валидные случаи нормализации через сам класс Time
    Time time(1, 75);  // Time сам выполнит нормализацию
    EXPECT_EQ(time.getHours(), 2);
    EXPECT_EQ(time.getMinutes(), 15);
}

/// Тестирование статического метода проверки валидности времени
TEST(TimeParserTest, StaticValidationMethod) {
    Time validTime(10, 30);
    Time invalidTime(-1, 30); // Отрицательные часы
    
    // Time(10, 60) будет нормализован в (11, 0) - валидное время
    Time normalizedTime(10, 60); 

    EXPECT_TRUE(TimeParser::isValidTime(validTime));
    EXPECT_FALSE(TimeParser::isValidTime(invalidTime));
    EXPECT_TRUE(TimeParser::isValidTime(normalizedTime));
}

// ==================== USER INPUT PARSER TESTS ====================

/// Тестирование парсера пользовательского ввода с ценой и временем
TEST(UserInputParserTest, ParsePriceAndTime) {
	UserInputParser parser;
	double price;
	Time time;
	std::string errorMessage;

	EXPECT_TRUE(parser.parse("15.50 00:45", price, time, errorMessage));
	EXPECT_DOUBLE_EQ(price, 15.50);
	EXPECT_EQ(time.getHours(), 0);
	EXPECT_EQ(time.getMinutes(), 45);
	EXPECT_TRUE(errorMessage.empty());
}

/// Тестирование парсера пользовательского ввода только с ценой
TEST(UserInputParserTest, ParseOnlyPrice) {
	UserInputParser parser;
	double price;
	Time time;
	std::string errorMessage;

	EXPECT_TRUE(parser.parse("20.00", price, time, errorMessage));
	EXPECT_DOUBLE_EQ(price, 20.00);
	EXPECT_EQ(time.totalMinutes(), 0);
	EXPECT_TRUE(errorMessage.empty());
}

/// Тестирование парсера пользовательского ввода только со временем
TEST(UserInputParserTest, ParseOnlyTime) {
	UserInputParser parser;
	double price;
	Time time;
	std::string errorMessage;

	EXPECT_TRUE(parser.parse("01:30", price, time, errorMessage));
	EXPECT_DOUBLE_EQ(price, 0.0);
	EXPECT_EQ(time.getHours(), 1);
	EXPECT_EQ(time.getMinutes(), 30);
	EXPECT_TRUE(errorMessage.empty());
}

/// Тестирование парсера пользовательского ввода с невалидными данными
TEST(UserInputParserTest, ParseInvalidInput) {
	UserInputParser parser;
	double price;
	Time time;
	std::string errorMessage;

	EXPECT_FALSE(parser.parse("abc def", price, time, errorMessage));
	EXPECT_FALSE(errorMessage.empty());

	// Проверяем, что значения не изменились при невалидном вводе
	EXPECT_DOUBLE_EQ(price, 0.0);
	EXPECT_EQ(time.totalMinutes(), 0);
}

/// Тестирование парсера пользовательского ввода с разным порядком аргументов
TEST(UserInputParserTest, ParseDifferentOrder) {
	UserInputParser parser;
	double price;
	Time time;
	std::string errorMessage;

	// Время перед ценой
	EXPECT_TRUE(parser.parse("01:15 25.75", price, time, errorMessage));
	EXPECT_DOUBLE_EQ(price, 25.75);
	EXPECT_EQ(time.getHours(), 1);
	EXPECT_EQ(time.getMinutes(), 15);
}

/// Тестирование парсера пользовательского ввода с лишними пробелами
TEST(UserInputParserTest, ParseWithExtraSpaces) {
	UserInputParser parser;
	double price;
	Time time;
	std::string errorMessage;

	EXPECT_TRUE(parser.parse("  15.50   01:30  ", price, time, errorMessage));
	EXPECT_DOUBLE_EQ(price, 15.50);
	EXPECT_EQ(time.getHours(), 1);
	EXPECT_EQ(time.getMinutes(), 30);
}