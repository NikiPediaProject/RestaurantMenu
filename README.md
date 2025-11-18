# Restaurant Menu Management System

A comprehensive C++ application for managing restaurant menus with full Google Test coverage.

## Project Structure

RestaurantMenu/
├── RestaurantMenu/ # Main application project
├── RestaurantMenuTests/ # Test project with Google Test
├── lib/googletest-main/ # Google Test framework
└── menu.txt # Sample menu file

## Building and Running

### Main Application
1. Open `RestaurantMenu.sln` in Visual Studio 2017+
2. Set `RestaurantMenu` as startup project
3. Build and run (Ctrl+F5)

### Running Tests
1. Set `RestaurantMenuTests` as startup project  
2. Build and run (Ctrl+F5)
3. View test results in console

## Test Coverage

- Time class with normalization
- Dish class creation and output
- NumberParser for price validation
- TimeParser for time format parsing
- MenuStorage CRUD operations
- MenuSorter algorithms
- FileParser regex extraction

## Features

- Load menu from text files
- Interactive command interface
- Add/delete dishes
- Filter by price and time
- Save menus to files
- Comprehensive unit testing

## Commands

### File Level
- `/help` - show help
- `/clear` - clear console  
- `/stop` - exit program
- `filename` - load menu file

### Interactive Mode
- `help` - show commands
- `print` - display menu
- `add "Dish" price time` - add dish
- `delete "Dish"` - remove dish
- `save filename` - save menu
- `price time` - filter dishes