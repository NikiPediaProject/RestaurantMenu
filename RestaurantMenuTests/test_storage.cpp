#include <gtest/gtest.h>
#include "../RestaurantMenu/storage.h"

/// Тестирование добавления блюд в хранилище
TEST(MenuStorageTest, AddDishes) {
	MenuStorage storage;
	Time time(0, 30);

	// Проверяем пустое хранилище
	EXPECT_EQ(storage.getDishesCount(), 0);

	// Добавляем первое блюдо
	storage.addDish("Блюдо 1", 10.50, time);
	EXPECT_EQ(storage.getDishesCount(), 1);

	// Добавляем второе блюдо
	storage.addDish("Блюдо 2", 15.75, Time(0, 45));
	EXPECT_EQ(storage.getDishesCount(), 2);

	// Проверяем, что блюда сохранились корректно
	const auto& dishes = storage.getDishes();
	EXPECT_EQ(dishes[0].name, "Блюдо 1");
	EXPECT_DOUBLE_EQ(dishes[0].price, 10.50);
	EXPECT_EQ(dishes[1].name, "Блюдо 2");
	EXPECT_DOUBLE_EQ(dishes[1].price, 15.75);
}

/// Тестирование очистки хранилища
TEST(MenuStorageTest, ClearStorage) {
	MenuStorage storage;
	Time time(0, 30);

	// Добавляем блюдо и проверяем
	storage.addDish("Блюдо", 10.0, time);
	EXPECT_EQ(storage.getDishesCount(), 1);

	// Очищаем хранилище
	storage.clear();
	EXPECT_EQ(storage.getDishesCount(), 0);

	// Проверяем, что можно добавлять после очистки
	storage.addDish("Новое блюдо", 15.0, time);
	EXPECT_EQ(storage.getDishesCount(), 1);
}

/// Тестирование удаления блюд из хранилища
TEST(MenuStorageTest, RemoveDish) {
	MenuStorage storage;
	Time time1(0, 30);
	Time time2(0, 45);

	// Добавляем несколько блюд
	storage.addDish("Блюдо 1", 10.0, time1);
	storage.addDish("Блюдо 2", 15.0, time2);
	storage.addDish("Блюдо 1", 12.0, time1); // Дублирующее название

	EXPECT_EQ(storage.getDishesCount(), 3);

	// Удаляем конкретное блюдо
	bool removed = storage.removeDish("Блюдо 1", 10.0, time1);
	EXPECT_TRUE(removed);
	EXPECT_EQ(storage.getDishesCount(), 2);

	// Пытаемся удалить несуществующее блюдо
	removed = storage.removeDish("Несуществующее", 20.0, time1);
	EXPECT_FALSE(removed);
	EXPECT_EQ(storage.getDishesCount(), 2);
}

/// Тестирование сохранения в файл (основная логика)
TEST(MenuStorageTest, SaveToFile) {
	MenuStorage storage;
	Time time(0, 30);

	// Добавляем тестовые данные
	storage.addDish("Тестовое блюдо", 12.50, time);

	// Пытаемся сохранить в файл
	EXPECT_NO_THROW(storage.saveToFile("test_output.txt"));

	// Файл должен быть создан, но мы не проверяем его содержимое в юнит-тестах
	// (это лучше делать в интеграционных тестах)
}

/// Тестирование алфавитной сортировки
TEST(MenuSorterTest, AlphabeticalSort) {
	MenuSorter sorter;
	std::vector<Dish> dishes;

	// Добавляем блюда в случайном порядке
	dishes.push_back(Dish("Ц", 15.0, Time(0, 30)));
	dishes.push_back(Dish("А", 10.0, Time(0, 20)));
	dishes.push_back(Dish("Б", 20.0, Time(0, 45)));

	// Сортируем по алфавиту
	sorter.sortAlphabetically(dishes);

	// Проверяем порядок
	EXPECT_EQ(dishes[0].name, "А");
	EXPECT_EQ(dishes[1].name, "Б");
	EXPECT_EQ(dishes[2].name, "Ц");
}

/// Тестирование сортировки по убыванию цены
TEST(MenuSorterTest, SortByPriceDesc) {
	MenuSorter sorter;
	std::vector<Dish> dishes;

	// Добавляем блюда с разными ценами
	dishes.push_back(Dish("Дешевое", 10.0, Time(0, 30)));
	dishes.push_back(Dish("Дорогое", 30.0, Time(0, 45)));
	dishes.push_back(Dish("Среднее", 20.0, Time(0, 25)));

	// Сортируем по убыванию цены
	sorter.sortByPriceDesc(dishes);

	// Проверяем порядок (от самой дорогой к самой дешевой)
	EXPECT_DOUBLE_EQ(dishes[0].price, 30.0);
	EXPECT_DOUBLE_EQ(dishes[1].price, 20.0);
	EXPECT_DOUBLE_EQ(dishes[2].price, 10.0);
}

/// Тестирование сортировки по возрастанию времени приготовления
TEST(MenuSorterTest, SortByTimeAsc) {
	MenuSorter sorter;
	std::vector<Dish> dishes;

	// Добавляем блюда с разным временем приготовления
	dishes.push_back(Dish("Долгое", 15.0, Time(1, 30)));
	dishes.push_back(Dish("Быстрое", 10.0, Time(0, 15)));
	dishes.push_back(Dish("Среднее", 20.0, Time(0, 45)));

	// Сортируем по возрастанию времени
	sorter.sortByTimeAsc(dishes);

	// Проверяем порядок (от самого быстрого к самому медленному)
	EXPECT_EQ(dishes[0].time.totalMinutes(), 15);
	EXPECT_EQ(dishes[1].time.totalMinutes(), 45);
	EXPECT_EQ(dishes[2].time.totalMinutes(), 90);
}

/// Тестирование фильтрации по цене
TEST(MenuFilterTest, FilterByPrice) {
	auto sorter = std::make_unique<MenuSorter>();
	MenuFilter filter(std::move(sorter));
	std::vector<Dish> dishes;

	// Создаем тестовые данные
	dishes.push_back(Dish("Дешевое", 10.0, Time(0, 30)));
	dishes.push_back(Dish("Дорогое", 30.0, Time(0, 45)));
	dishes.push_back(Dish("Среднее", 20.0, Time(0, 25)));

	// Фильтруем по максимальной цене 25.0
	auto filtered = filter.filterByPrice(dishes, 25.0);

	// Должны остаться только блюда дешевле 25.0
	EXPECT_EQ(filtered.size(), 2);
	EXPECT_DOUBLE_EQ(filtered[0].price, 20.0); // Отсортировано по убыванию цены
	EXPECT_DOUBLE_EQ(filtered[1].price, 10.0);
}

/// Тестирование фильтрации по времени
TEST(MenuFilterTest, FilterByTime) {
	auto sorter = std::make_unique<MenuSorter>();
	MenuFilter filter(std::move(sorter));
	std::vector<Dish> dishes;

	// Создаем тестовые данные
	dishes.push_back(Dish("Быстрое", 15.0, Time(0, 15)));
	dishes.push_back(Dish("Долгое", 25.0, Time(1, 0)));
	dishes.push_back(Dish("Среднее", 20.0, Time(0, 30)));

	// Фильтруем по максимальному времени 45 минут
	Time maxTime(0, 45);
	auto filtered = filter.filterByTime(dishes, maxTime);

	// Должны остаться только блюда с временем приготовления меньше 45 минут
	EXPECT_EQ(filtered.size(), 2);
	EXPECT_EQ(filtered[0].time.totalMinutes(), 15); // Отсортировано по возрастанию времени
	EXPECT_EQ(filtered[1].time.totalMinutes(), 30);
}

/// Тестирование комбинированной фильтрации по цене и времени
TEST(MenuFilterTest, FilterByPriceAndTime) {
	auto sorter = std::make_unique<MenuSorter>();
	MenuFilter filter(std::move(sorter));
	std::vector<Dish> dishes;

	// Создаем тестовые данные
	dishes.push_back(Dish("А - быстрое и дешевое", 10.0, Time(0, 15)));
	dishes.push_back(Dish("Б - дорогое и быстрое", 30.0, Time(0, 20)));
	dishes.push_back(Dish("В - дешевое и долгое", 15.0, Time(1, 0)));
	dishes.push_back(Dish("Г - дорогое и долгое", 25.0, Time(1, 30)));

	// Фильтруем по цене < 20.0 и времени < 30 минут
	Time maxTime(0, 30);
	auto filtered = filter.filterByPriceAndTime(dishes, 20.0, maxTime);

	// Должно остаться только одно блюдо, удовлетворяющее обоим критериям
	EXPECT_EQ(filtered.size(), 1);
	EXPECT_EQ(filtered[0].name, "А - быстрое и дешевое");
	EXPECT_DOUBLE_EQ(filtered[0].price, 10.0);
	EXPECT_EQ(filtered[0].time.totalMinutes(), 15);
}