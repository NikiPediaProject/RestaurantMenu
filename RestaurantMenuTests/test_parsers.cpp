#include <gtest/gtest.h>
#include "parsers.h"

TEST(NumberParserTest, ValidNumbers) {
	NumberParser parser;
	EXPECT_TRUE(parser.parse("12.50"));
	EXPECT_DOUBLE_EQ(parser.getResult(), 12.50);

	EXPECT_TRUE(parser.parse("7,99"));
	EXPECT_DOUBLE_EQ(parser.getResult(), 7.99);
}

TEST(NumberParserTest, InvalidNumbers) {
	NumberParser parser;
	EXPECT_FALSE(parser.parse("abc"));
	EXPECT_FALSE(parser.parse("12.50.30"));
}

TEST(TimeParserTest, ValidTime) {
	TimeParser parser;
	EXPECT_TRUE(parser.parse("12:30"));
	Time result = parser.getResult();
	EXPECT_EQ(result.getHours(), 12);
	EXPECT_EQ(result.getMinutes(), 30);
}

TEST(TimeParserTest, TimeNormalization) {
	TimeParser parser;

	EXPECT_TRUE(parser.parse("01:75"));
	Time result = parser.getResult();
	EXPECT_EQ(result.getHours(), 2);
	EXPECT_EQ(result.getMinutes(), 15);
}

TEST(UserInputParserTest, ParsePriceAndTime) {
	UserInputParser parser;
	double price;
	Time time;

	EXPECT_TRUE(parser.parse("15.50 00:45", price, time));
	EXPECT_DOUBLE_EQ(price, 15.50);
	EXPECT_EQ(time.getHours(), 0);
	EXPECT_EQ(time.getMinutes(), 45);
}