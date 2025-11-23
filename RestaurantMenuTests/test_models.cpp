#include <gtest/gtest.h>
#include "../RestaurantMenu/models.h"

/// Тестирование конструктора Time по умолчанию
TEST(TimeTest, DefaultConstructor) {
	Time time;
	EXPECT_EQ(time.getHours(), 0);
	EXPECT_EQ(time.getMinutes(), 0);
	EXPECT_EQ(time.getDays(), 0);
	EXPECT_EQ(time.getYears(), 0);
}

/// Тестирование установки времени
TEST(TimeTest, SetTime) {
	Time time;

	// Устанавливаем время только с часами и минутами
	time.setTime(2, 30);
	EXPECT_EQ(time.getHours(), 2);
	EXPECT_EQ(time.getMinutes(), 30);
	EXPECT_EQ(time.getDays(), 0);
	EXPECT_EQ(time.getYears(), 0);

	// Устанавливаем время с днями и годами
	time.setTime(5, 45, 3, 1);
	EXPECT_EQ(time.getHours(), 5);
	EXPECT_EQ(time.getMinutes(), 45);
	EXPECT_EQ(time.getDays(), 3);
	EXPECT_EQ(time.getYears(), 1);
}

/// Тестирование автоматической нормализации времени
TEST(TimeTest, TimeNormalization) {
	// Минуты больше 59
	Time time1(1, 65);
	EXPECT_EQ(time1.getHours(), 2);
	EXPECT_EQ(time1.getMinutes(), 5);

	// Часы больше 23
	Time time2(25, 30);
	EXPECT_EQ(time2.getHours(), 1);
	EXPECT_EQ(time2.getDays(), 1);
	EXPECT_EQ(time2.getMinutes(), 30);

	// Дни больше 30 (условный месяц)
	Time time3(0, 0, 35, 0);
	EXPECT_EQ(time3.getDays(), 4);  // 35 - 31 = 4
	EXPECT_EQ(time3.getYears(), 1); // Переполнение в годы
}

/// Тестирование вычисления общего времени в минутах
TEST(TimeTest, TotalMinutes) {
	// Только часы и минуты
	Time time1(2, 30);
	EXPECT_EQ(time1.totalMinutes(), 150);

	// С днями
	Time time2(2, 30, 1, 0);
	EXPECT_EQ(time2.totalMinutes(), 150 + 24 * 60);

	// С годами
	Time time3(2, 30, 0, 1);
	EXPECT_EQ(time3.totalMinutes(), 150 + 31 * 24 * 60); // 31 день в "месяце"
}

/// Тестирование строкового представления времени
TEST(TimeTest, ToString) {
	Time time1(2, 30);
	EXPECT_EQ(time1.toString(), "2ч 30м");

	Time time2(0, 45);
	EXPECT_EQ(time2.toString(), "45м");

	Time time3(2, 30, 3, 1);
	EXPECT_EQ(time3.toString(), "1г. 3д. 2ч 30м");

	Time time4(0, 0, 0, 0);
	EXPECT_EQ(time4.toString(), "0м");
}

/// Тестирование операторов сравнения
TEST(TimeTest, ComparisonOperators) {
	Time time1(2, 30);
	Time time2(2, 30);
	Time time3(3, 0);

	// Оператор равенства
	EXPECT_TRUE(time1 == time2);
	EXPECT_FALSE(time1 == time3);

	// Оператор меньше
	EXPECT_TRUE(time1 < time3);
	EXPECT_FALSE(time3 < time1);
	EXPECT_FALSE(time1 < time2);
}

/// Тестирование создания блюда
TEST(DishTest, CreateDish) {
	Time time(0, 30);
	Dish dish("Пицца Маргарита", 12.50, time);

	EXPECT_EQ(dish.name, "Пицца Маргарита");
	EXPECT_DOUBLE_EQ(dish.price, 12.50);
	EXPECT_EQ(dish.time.getHours(), 0);
	EXPECT_EQ(dish.time.getMinutes(), 30);
}

/// Тестирование конструктора Dish с компонентами времени
TEST(DishTest, CreateDishWithTimeComponents) {
	Dish dish("Салат", 8.75, 0, 15);

	EXPECT_EQ(dish.name, "Салат");
	EXPECT_DOUBLE_EQ(dish.price, 8.75);
	EXPECT_EQ(dish.time.getHours(), 0);
	EXPECT_EQ(dish.time.getMinutes(), 15);
}

/// Тестирование установки времени приготовления
TEST(DishTest, SetTime) {
	Dish dish("Тестовое блюдо", 10.0, 0, 0);

	dish.setTime(1, 30, 2, 1);
	EXPECT_EQ(dish.time.getHours(), 1);
	EXPECT_EQ(dish.time.getMinutes(), 30);
	EXPECT_EQ(dish.time.getDays(), 2);
	EXPECT_EQ(dish.time.getYears(), 1);
}

/// Тестирование вычисления общего времени приготовления в минутах
TEST(DishTest, TotalMinutes) {
	Dish dish("Блюдо", 10.0, 2, 30);
	EXPECT_EQ(dish.totalMinutes(), 150);

	Dish dish2("Долгое блюдо", 15.0, 1, 0, 1, 0);
	EXPECT_EQ(dish2.totalMinutes(), 60 + 24 * 60); // 1 час + 1 день
}

/// Тестирование строкового представления времени приготовления
TEST(DishTest, GetTimeString) {
	Dish dish("Быстрое блюдо", 10.0, 0, 15);

	// Время 0 часов 15 минут должно выводиться как "15м"
	// Класс Time автоматически нормализует и форматирует вывод
	std::string timeString = dish.getTimeString();

	// Проверяем, что строка содержит минуты
	EXPECT_TRUE(timeString.find("15") != std::string::npos);
	EXPECT_TRUE(timeString.find("м") != std::string::npos);

	// Для надежности проверяем альтернативный формат
	Dish dish2("Долгое блюдо", 20.0, 2, 30, 1, 0);
	std::string timeString2 = dish2.getTimeString();
	EXPECT_FALSE(timeString2.empty());
}

/// Тестирование оператора вывода для Dish
TEST(DishTest, OutputOperator) {
	Dish dish("Тестовое блюдо", 12.50, 0, 30);

	std::stringstream ss;
	ss << dish;

	// Проверяем, что вывод содержит ожидаемые данные
	std::string output = ss.str();
	EXPECT_TRUE(output.find("Тестовое блюдо") != std::string::npos);
	EXPECT_TRUE(output.find("12.50") != std::string::npos);
	EXPECT_TRUE(output.find("30м") != std::string::npos);
}

/// Тестирование метода print()
TEST(DishTest, PrintMethod) {
	Dish dish("Печать тест", 9.99, 0, 25);

	// Перенаправляем stdout для проверки вывода
	testing::internal::CaptureStdout();
	dish.print();
	std::string output = testing::internal::GetCapturedStdout();

	EXPECT_FALSE(output.empty());
	EXPECT_TRUE(output.find("Печать тест") != std::string::npos);
}