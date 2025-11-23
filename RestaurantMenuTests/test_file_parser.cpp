#include <gtest/gtest.h>
#include "../RestaurantMenu/file_parser.h"

/// Тестирование извлечения значений из корректной строки с помощью регулярных выражений
TEST(FileParserTest, ExtractValuesWithRegex_ValidInput) {
	MenuFileParser parser;
	std::string name;
	double price;
	Time time;

	bool result = parser.extractValuesWithRegex(
		"\"Тестовое блюдо\" 12.50 01:30",
		name, price, time
	);

	EXPECT_TRUE(result);
	EXPECT_EQ(name, "Тестовое блюдо");
	EXPECT_DOUBLE_EQ(price, 12.50);
	EXPECT_EQ(time.getHours(), 1);
	EXPECT_EQ(time.getMinutes(), 30);
}

/// Тестирование извлечения значений с разными форматами чисел
TEST(FileParserTest, ExtractValuesWithRegex_DifferentNumberFormats) {
	MenuFileParser parser;
	std::string name;
	double price;
	Time time;

	// Тест с запятой как разделителем дробной части
	bool result = parser.extractValuesWithRegex(
		"\"Блюдо с запятой\" 7,99 00:45",
		name, price, time
	);

	EXPECT_TRUE(result);
	EXPECT_EQ(name, "Блюдо с запятой");
	EXPECT_DOUBLE_EQ(price, 7.99);
	EXPECT_EQ(time.getHours(), 0);
	EXPECT_EQ(time.getMinutes(), 45);
}

/// Тестирование извлечения значений с разным порядком элементов
TEST(FileParserTest, ExtractValuesWithRegex_DifferentOrder) {
	MenuFileParser parser;
	std::string name;
	double price;
	Time time;

	// Время перед ценой
	bool result = parser.extractValuesWithRegex(
		"\"Блюдо\" 01:15 25.75",
		name, price, time
	);

	EXPECT_TRUE(result);
	EXPECT_EQ(name, "Блюдо");
	EXPECT_DOUBLE_EQ(price, 25.75);
	EXPECT_EQ(time.getHours(), 1);
	EXPECT_EQ(time.getMinutes(), 15);
}

/// Тестирование извлечения значений с дополнительными пробелами
TEST(FileParserTest, ExtractValuesWithRegex_ExtraSpaces) {
	MenuFileParser parser;
	std::string name;
	double price;
	Time time;

	bool result = parser.extractValuesWithRegex(
		"  \"Блюдо с пробелами\"   15.99   02:45  ",
		name, price, time
	);

	EXPECT_TRUE(result);
	EXPECT_EQ(name, "Блюдо с пробелами");
	EXPECT_DOUBLE_EQ(price, 15.99);
	EXPECT_EQ(time.getHours(), 2);
	EXPECT_EQ(time.getMinutes(), 45);
}

/// Тестирование обработки невалидных строк
TEST(FileParserTest, ExtractValuesWithRegex_InvalidInput) {
	MenuFileParser parser;
	std::string name;
	double price;
	Time time;

	// Отсутствует название в кавычках
	EXPECT_FALSE(parser.extractValuesWithRegex(
		"Без кавычек 12.50 01:30",
		name, price, time
	));

	// Неправильный формат времени
	EXPECT_FALSE(parser.extractValuesWithRegex(
		"\"Блюдо\" 12.50 99:99",
		name, price, time
	));

	// Отсутствует цена
	EXPECT_FALSE(parser.extractValuesWithRegex(
		"\"Блюдо\" 01:30",
		name, price, time
	));

	// Пустая строка
	EXPECT_FALSE(parser.extractValuesWithRegex(
		"",
		name, price, time
	));

	// Только пробелы
	EXPECT_FALSE(parser.extractValuesWithRegex(
		"   ",
		name, price, time
	));
}

/// Тестирование обработки строк с лишними данными
TEST(FileParserTest, ExtractValuesWithRegex_ExtraData) {
	MenuFileParser parser;
	std::string name;
	double price;
	Time time;

	// Лишние данные в конце строки
	EXPECT_FALSE(parser.extractValuesWithRegex(
		"\"Блюдо\" 12.50 01:30 лишний текст",
		name, price, time
	));

	// Лишние данные в середине
	EXPECT_FALSE(parser.extractValuesWithRegex(
		"\"Блюдо\" лишнее 12.50 01:30",
		name, price, time
	));
}

/// Тестирование граничных случаев
TEST(FileParserTest, ExtractValuesWithRegex_BoundaryCases) {
	MenuFileParser parser;
	std::string name;
	double price;
	Time time;

	// Минимальные значения - валидный случай
	EXPECT_TRUE(parser.extractValuesWithRegex(
		"\"А\" 0.01 00:00",
		name, price, time
	));
	EXPECT_EQ(name, "А");
	EXPECT_DOUBLE_EQ(price, 0.01);
	EXPECT_EQ(time.getHours(), 0);
	EXPECT_EQ(time.getMinutes(), 0);

	// Пустое название - должно быть отсеяно парсером
	EXPECT_FALSE(parser.extractValuesWithRegex(  // <-- ИЗМЕНЕНО: ожидаем false
		"\"\" 1.00 00:01",
		name, price, time
	));
}

/// Тестирование специальных символов в названии
TEST(FileParserTest, ExtractValuesWithRegex_SpecialCharacters) {
	MenuFileParser parser;
	std::string name;
	double price;
	Time time;

	// Цифры и специальные символы в названии
	EXPECT_TRUE(parser.extractValuesWithRegex(
		"\"Блюдо 2.0 - спецверсия!\" 15.99 01:00",
		name, price, time
	));
	EXPECT_EQ(name, "Блюдо 2.0 - спецверсия!");
	EXPECT_DOUBLE_EQ(price, 15.99);
}