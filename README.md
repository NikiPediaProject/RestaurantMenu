# Restaurant Menu Management System

A comprehensive C++ application for managing restaurant menus with full Google Test coverage.

## Project Structure

### RestaurantMenu/
- ├── RestaurantMenu/ # Main application project
- ├── RestaurantMenuTests/ # Test project with Google Test
- ├── lib/googletest-main/ # Google Test framework
- └── menu.txt # Sample menu file



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
- Dish name validation

## Features

- Load menu from text files
- Interactive command interface using command map for easy extensibility
- Add/delete dishes with validation
- Filter by price and time
- Save menus to files
- Comprehensive unit testing
- Dish name validation (must be non-empty after trimming)

## Implementation Details

The application uses several design patterns and modern C++ features:

- **Builder Pattern**: For flexible creation of the RestaurantMenuApp with configurable components
- **Command Map Pattern**: User commands are processed via a `std::map` of `std::function` objects, making it easy to add new commands
- **Interface Segregation**: Multiple interfaces (IParser, IMenuStorage, etc.) for better abstraction
- **Dependency Injection**: Components are injected through constructors for testability
- **RAII**: Resource management (file handling) using constructors and destructors
- **Regular Expressions**: For robust parsing of menu file lines

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

## Recent Improvements

- **Command Processing Refactoring**: Replaced conditional command handling with extensible command map using `std::map<std::string, std::function>`
- **Dish Name Validation**: Implemented non-empty dish name validation after whitespace trimming
- **Comprehensive Testing**: Added Google Test coverage for all major components
- **Code Organization**: Improved separation of concerns with dedicated command handlers

## Extensibility

The command system is designed for easy extension. To add a new command:
1. Add a handler method in `RestaurantMenuApp` class
2. Register it in the `initializeCommands()` method
3. Update the help text in `showHelp()` method