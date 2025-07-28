#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <fstream>

#define TRESHOLD	500
#define NULL_BI		BigInteger("0")
#define DEF_LEN		113
#define PRIME_A		9973

class BigInteger
{
private:
	std::vector<uint8_t> digits;
	bool isNegative = false;
	// Utilities:
	BigInteger gcde(BigInteger a, BigInteger b, BigInteger* x, BigInteger* y);
	BigInteger simpMlt(const BigInteger& other) const;
	void extend(size_t len);
	BigInteger shift_left(uint32_t pw) const;
	BigInteger normalize();
	BigInteger gcd(BigInteger a, BigInteger b);
	BigInteger epow(BigInteger e) const;
	BigInteger abs() const;
	bool isZero() const;

public:
	// Other:
	int getSize() const { return digits.size(); }
	BigInteger pow_mod(BigInteger b, const BigInteger& m);
	BigInteger getD(BigInteger e, BigInteger f);
	BigInteger genPrime(const int& len);
	BigInteger randC(size_t len) const;
	std::vector<int> primeArrGen(const int& rw);
	bool isPrime(uint8_t k) const;
	std::string to_string() const;
	int to_int() const;
	BigInteger getPrime(const int& len);
	BigInteger getOdd(const int& len);

	// Constructors:
	BigInteger() { digits.reserve(1); }
	BigInteger(const std::string& string);
	BigInteger(const int& num) { *this = BigInteger(std::to_string(num)); }

	// Arithmetic:
	BigInteger& operator=(const BigInteger& other);
	BigInteger& operator=(const int& other) { *this = BigInteger(std::to_string(other)); return *this; }
	BigInteger operator+(const BigInteger& other) const;
	BigInteger operator-(const BigInteger& other) const;
	BigInteger operator*(BigInteger& other);
	BigInteger operator/(const BigInteger& other) const;
	BigInteger operator%(const BigInteger& other) const;

	BigInteger operator*(const int& other) { BigInteger temp = BigInteger(std::to_string(other)); return *this * temp; }
	BigInteger operator/(const int& other) const { return *this / BigInteger(std::to_string(other)); }
	BigInteger operator%(const int& other) const { return *this % BigInteger(std::to_string(other)); }
	BigInteger operator+(const int& other) const { return *this + BigInteger(std::to_string(other)); }
	BigInteger operator-(const int& other) const { return *this - BigInteger(std::to_string(other)); }

	BigInteger operator++() { *this = *this + 1; return *this; }
	BigInteger operator++(int);
	BigInteger operator--() { *this = *this - 1; return *this; }
	BigInteger operator--(int);

	BigInteger operator+=(const BigInteger& other) { *this = *this + other; return *this; }
	BigInteger operator-=(const BigInteger& other) { *this = *this - other; return *this; }
	BigInteger operator*=(BigInteger& other) { *this = *this * other; return *this; }
	BigInteger operator/=(const BigInteger& other) { *this = *this / other; return *this; }
	BigInteger operator%=(const BigInteger& other) { *this = *this % other; return *this; }

	BigInteger operator+=(const int& other) { *this = *this + other; return *this; }
	BigInteger operator-=(const int& other) { *this = *this - other; return *this; }
	BigInteger operator*=(const int& other) { *this = *this * other; return *this; }
	BigInteger operator/=(const int& other) { *this = *this / other; return *this; }
	BigInteger operator%=(const int& other) { *this = *this % other; return *this; }


	// Comparison:
	bool operator<(const BigInteger& other) const;
	bool operator>(const BigInteger& other) const { return other < *this; }
	bool operator<=(const BigInteger& other) const { return !(other < *this); }
	bool operator>=(const BigInteger& other) const { return !(*this < other); }
	bool operator==(const BigInteger& other) const;
	bool operator!=(const BigInteger& other) const { return !(*this == other); }

	bool operator<(const int& other) const { return *this < BigInteger(std::to_string(other)); }
	bool operator>(const int& other) const { return BigInteger(std::to_string(other)) < *this; }
	bool operator<=(const int& other) const { return !(BigInteger(std::to_string(other)) < *this); }
	bool operator>=(const int& other) const { return !(*this < other); }
	bool operator==(const int& other) const { return *this == BigInteger(std::to_string(other)); }
	bool operator!=(const int& other) const { return !(*this == other); }
};

// IN/OUT
std::istream& operator>>(std::istream& in, BigInteger& num);
std::ostream& operator<<(std::ostream& out, const BigInteger& num);

//Arithmetic (int):
BigInteger operator+(const int& num1, const BigInteger& num2);
BigInteger operator-(const int& num1, const BigInteger& num2);
BigInteger operator*(const int& num1, BigInteger& num2);
BigInteger operator/(const int& num1, BigInteger& num2);
BigInteger operator%(const int& num1, BigInteger& num2);

// Comparison (int):
bool operator<(const int& num1, const BigInteger& num2);
bool operator>(const int& num1, const BigInteger& num2);
bool operator<=(const int& num1, const BigInteger& num2);
bool operator>=(const int& num1, const BigInteger& num2);
bool operator==(const int& num1, const BigInteger& num2);
bool operator!=(const int& num1, const BigInteger& num2);