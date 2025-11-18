#include <gtest/gtest.h>
#include "models.h"

TEST(TimeTest, DefaultConstructor) {
	Time time;
	EXPECT_EQ(time.getHours(), 0);
	EXPECT_EQ(time.getMinutes(), 0);
}

TEST(TimeTest, SetTime) {
	Time time;
	time.setTime(2, 30);
	EXPECT_EQ(time.getHours(), 2);
	EXPECT_EQ(time.getMinutes(), 30);
}

TEST(TimeTest, TimeNormalization) {
	Time time(1, 65);
	EXPECT_EQ(time.getHours(), 2);
	EXPECT_EQ(time.getMinutes(), 5);
}

TEST(TimeTest, TotalMinutes) {
	Time time(2, 30);
	EXPECT_EQ(time.totalMinutes(), 150);
}

TEST(DishTest, CreateDish) {
	Time time(0, 30);
	Dish dish("Пицца Маргарита", 12.50, time);

	EXPECT_EQ(dish.name, "Пицца Маргарита");
	EXPECT_DOUBLE_EQ(dish.price, 12.50);
	EXPECT_EQ(dish.time.getHours(), 0);
	EXPECT_EQ(dish.time.getMinutes(), 30);
}