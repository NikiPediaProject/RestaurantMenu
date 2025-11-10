#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

/// Набор утилитарных функций для работы со строками
class StringUtils {
public:
	/// Проверяет, является ли символ цифрой в кодировке ASCII
	static bool isDigitASCII(char c);

	/// Проверяет, состоит ли строка исключительно из цифровых символов
	static bool isOnlyDigits(const std::string& str);

	/// Проверяет, является ли символ безопасным пробельным символом
	static bool isSafeSpace(char c);

	/// Удаляет пробельные символы с начала и конца строки
	static std::string trim(const std::string& str);

	/// Разбивает строку на токены по пробельным символам
	static std::vector<std::string> tokenize(const std::string& str);
};

#endif // UTILS_H