#include <gtest/gtest.h>
#include "storage.h"

TEST(MenuStorageTest, AddDishes) {
	MenuStorage storage;
	Time time(0, 30);

	EXPECT_EQ(storage.getDishesCount(), 0);
	storage.addDish("Блюдо 1", 10.50, time);
	storage.addDish("Блюдо 2", 15.75, Time(0, 45));
	EXPECT_EQ(storage.getDishesCount(), 2);
}

TEST(MenuStorageTest, ClearStorage) {
	MenuStorage storage;
	Time time(0, 30);

	storage.addDish("Блюдо", 10.0, time);
	EXPECT_EQ(storage.getDishesCount(), 1);
	storage.clear();
	EXPECT_EQ(storage.getDishesCount(), 0);
}

TEST(MenuSorterTest, AlphabeticalSort) {
	MenuSorter sorter;
	std::vector<Dish> dishes;

	dishes.push_back(Dish("Ц", 15.0, Time(0, 30)));
	dishes.push_back(Dish("А", 10.0, Time(0, 20)));
	dishes.push_back(Dish("Б", 20.0, Time(0, 45)));

	sorter.sortAlphabetically(dishes);
	EXPECT_EQ(dishes[0].name, "А");
	EXPECT_EQ(dishes[1].name, "Б");
	EXPECT_EQ(dishes[2].name, "Ц");
}