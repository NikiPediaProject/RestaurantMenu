#include <gtest/gtest.h>
#include "file_parser.h"

TEST(FileParserTest, ExtractValuesWithRegex) {
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