#include "bigInteger.h"

std::random_device rd;
std::mt19937 gen(rd());

//	Number to string utility
std::string BigInteger::to_string() const
{
	std::string res = "";
	if (isNegative) res = '-' + res;
	for (uint8_t d : digits)
		res.push_back(d + '0');
	return res;
}

int BigInteger::to_int() const
{
	int res = 0;
	for (int i = 0; i < digits.size(); i++)
		res += digits[i] * pow(10, digits.size() - (i + 1));
	if (isNegative) res *= -1;
	return res;
}

//	Prime numbers generator
BigInteger BigInteger::getPrime(const int& len)
{
	int prime_arr[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

	BigInteger s = PRIME_A;

	if (s.digits.size() > len) return s;

	BigInteger n;
	BigInteger temp = 2 * s + 1 - s;

	BigInteger r;
	while (s.digits.size() <= len) {
		r = rd();
		r = r % (2 * temp) + s;
		r.isNegative = false;

		if (r % 2 != 0) r++;

		n = s * r + 1;

		for (int i = 0; i < 10; i++) if (n % prime_arr[i] == 0) continue;

		BigInteger a;
		BigInteger d;
		while (true) {
			a = randC(n.digits.size() - 1);
			a.isNegative = false;

			if (a.pow_mod(n - 1, n) != 1) break;

			d = gcd((a.pow_mod(r, n) - 1), n);
			if (d != n) {
				if (d == 1) s = n;
				break;
			}
		}
	}
	return s;
}

//	Odd numbers generator
BigInteger BigInteger::getOdd(const int& len)
{
	BigInteger res = randC(len);
	if (res % 2 == 0) res++;
	return res;
}

BigInteger BigInteger::gcde(BigInteger a, BigInteger b, BigInteger* x, BigInteger* y)
{
	if (a == 0) {
		*x = 0;
		*y = 1;
		return b;
	}
	BigInteger x1, y1;
	BigInteger gd = gcde(b % a, a, &x1, &y1);
	*x = y1 - (b / a) * x1;
	*y = x1;
	return gd;
}

BigInteger BigInteger::getD(BigInteger e, BigInteger f)
{
	BigInteger x;
	BigInteger y;
	gcde(e, f, &x, &y);
	if (x < 0) x += f;
	return x;
}

BigInteger BigInteger::genPrime(const int& len)
{
		if (len <= 1) return BigInteger(2);

		BigInteger candidate = getOdd(len);
		candidate.isNegative = false;

		std::vector<int> small_primes = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };
		for (int p : small_primes) {
			if (candidate % p == 0 && candidate != p) {
				candidate += 2;
				continue;
			}
		}

		const int k = 5;
		while (true) {
			bool probably_prime = true;

			for (int i = 0; i < k && probably_prime; ++i) {
				BigInteger a = randC(len / 2) % (candidate - 3) + 2;
				if (a.pow_mod(candidate - 1, candidate) != 1) {
					probably_prime = false;
				}
			}

			if (probably_prime) return candidate;

			candidate += 2;
		}
}

//	Random numbers generator
BigInteger BigInteger::randC(size_t len) const
{
	BigInteger result;
	size_t rem_bits = len;

	if (!len) rem_bits = rd() % 401;

	while (rem_bits-- > 0) {
		uint8_t chunk = static_cast<uint8_t>(rd()) % 10;
		if (rem_bits == len - 1 && chunk == 0)
			while (chunk == 0) chunk = static_cast<uint8_t>(rd()) % 10;
		result.digits.push_back(chunk);
	}

	result.isNegative = false;
	result.normalize();
	return result;
}

//	Prime checker
bool BigInteger::isPrime(uint8_t k) const
{
	BigInteger n = *this;
	if (n % 2 == 0) return false;
	if (n <= 1) return false;
	if (n <= 3) return true;

	BigInteger d = n - 1;
	uint32_t s = 0;

	while (d % 2 == 0) {
		d /= 2;
		s++;
	}

	BigInteger a = randC(0) % (n - 3) + 2;
	bool composite = true;

	for (int i = 0; i < k; i++) {
		while (!(2 <= a && a <= n - 2)) a = randC(0) % (n - 3) + 2;
		BigInteger x = a.pow_mod(d, n);

		if (x == 1 || x == n - 1) {
			composite = false;
			continue;
		}

		for (int j = 0; j < s - 1; j++) {
			x = x.pow_mod(2, n);

			if (x == n - 1) {
				composite = false;
				break;
			}
		}
	}

	return !composite;
}

//	a * b mod m
BigInteger BigInteger::pow_mod(BigInteger b, const BigInteger& m)
{
	BigInteger res = 1;
	BigInteger a = *this;
	if (isNegative) (b % 2 == 0 ? res.isNegative = false : res.isNegative = true);
	a %= m;
	while (b > 0) {
		if (b % 2 == 1) {
			res = (res * a) % m;
		}
		a = (a * a) % m;
		b /= 2;
	}
	return res;
}

//	Eratoshene's sieve generator
std::vector<int> BigInteger::primeArrGen(const int& rw)
{
	bool* res = new bool[rw + 1];
	std::vector<int> prime_arr;
	prime_arr.push_back(2);

	for (int i = 0; i <= rw; i++) res[i] = true;

	for (int i = 2; pow(i, 2) <= rw; i++)
		if (res[i])
			for (int j = pow(i, 2); j <= rw; j += i) res[j] = false;

	for (int i = 3; i <= rw; i += 2)
		if (res[i]) prime_arr.push_back(i);
	delete[] res;
	return prime_arr;
}

BigInteger BigInteger::abs() const {
	BigInteger result = *this;
	result.isNegative = false;
	return result;
}

bool BigInteger::isZero() const {
	return digits.size() == 1 && digits[0] == 0;
}


//	Naive multiplication
BigInteger BigInteger::simpMlt(const BigInteger& other) const
{
	int m = digits.size();
	int n = other.digits.size();
	bool nflag = false;
	std::string result(m + n, 0);

	if ((this->isNegative || other.isNegative) && !(this->isNegative && other.isNegative)) nflag = true;
	for (int i = m - 1; i >= 0; i--) {
		uint8_t carry = 0;
		for (int j = n - 1; j >= 0; j--) {
			uint16_t product = digits[i] * other.digits[j] + (result[i + j + 1] ? static_cast<uint8_t>(result[i + j + 1] - '0') : 0) + carry;
			result[i + j + 1] = static_cast<char>(product % 10 + '0');
			carry = product / 10;
		}
		result[i] += static_cast<char>(carry + '0');
	}

	while (result.size() > 1 && result[0] == '0') {
		result.erase(result.begin());
	}

	if (nflag) result = '-' + result;
	return BigInteger(result);
}

//	Extend of numbers 
void BigInteger::extend(size_t len)
{
	if (digits.size() < len)
		this->digits.insert(digits.begin(), len - digits.size(), 0);
}

//	10 power
BigInteger BigInteger::shift_left(uint32_t pw) const
{
	if (digits.size() == 1 && digits[0] == 0)
		return *this;
	BigInteger temp = *this;
	temp.digits.insert(temp.digits.end(), pw, 0);
	return temp;
}

//	Normalizator
BigInteger BigInteger::normalize()
{
	while (digits.size() > 1 && digits[0] == 0)
		digits.erase(digits.begin());
	return *this;
}

BigInteger BigInteger::gcd(BigInteger a, BigInteger b)
{
	BigInteger temp = b;
	while (b != 0) {
		temp = b;
		b = a % b;
		a = temp;
	}
	return a;
}

BigInteger BigInteger::epow(BigInteger e) const
{
	BigInteger res = 1;
	BigInteger a = *this;

	if (isNegative && e % 2 == 0) res.isNegative = false;

	while (e > 0) {
		if (e % 2 == 1) res *= a;
		a *= a;
		e /= 2;
	}
	return res;
}

//	String to number constructor
BigInteger::BigInteger(const std::string& string)
{
	size_t start = 0;
	if (string[0] == '-') {
		isNegative = true;
		start = 1;
	}
	for (size_t i = start; i < string.size(); i++)
		digits.push_back(string[i] - '0');
	this->normalize();
}

//	Assignment operator
BigInteger& BigInteger::operator=(const BigInteger& other)
{
	isNegative = other.isNegative;
	digits.resize(other.digits.size());
	for (int i = 0; i < other.digits.size(); i++)
		digits[i] = other.digits[i];
	return *this;
}

//	Addition of big integers
BigInteger BigInteger::operator+(const BigInteger& other) const
{
	if (isNegative) {
		BigInteger temp = *this;
		temp.isNegative = false;
		return other - temp;
	}
	if (other.isNegative) {
		BigInteger temp = other;
		temp.isNegative = false;
		return *this - temp;
	}
	std::string res;
	uint8_t carry = 0;
	int sum = 0;
	int i = digits.size() - 1;
	int j = other.digits.size() - 1;

	while (i >= 0 || j >= 0 || carry > 0) {
		sum = carry;
		if (i >= 0) sum += digits[i--];
		if (j >= 0) sum += other.digits[j--];
		res.insert(res.begin(), (sum % 10 + '0'));
		carry = sum / 10;
	}

	return BigInteger(res);
}

//	Subtraction of big integers
BigInteger BigInteger::operator-(const BigInteger& other) const
{
	std::string res;
	int sub = 0;
	int i = digits.size() - 1;
	int j = other.digits.size() - 1;
	uint8_t borrow = 0;

	if (isNegative && !other.isNegative) {
		BigInteger temp = *this;
		temp.isNegative = false;
		temp = temp + other;
		temp.isNegative = true;
		return temp.to_string();
	}

	if (!isNegative && other.isNegative) {
		BigInteger temp = other;
		temp.isNegative = false;
		return (temp + *this).to_string();
	}


	if ((*this < other && !isNegative) || (*this > other && isNegative)) {
		while (j >= 0) {
			sub = other.digits[j--] - borrow;
			if (i >= 0) sub -= digits[i--];

			if (sub < 0) {
				sub += 10;
				borrow = 1;
			}
			else borrow = 0;

			res.insert(res.begin(), sub + '0');
		}
		if (!isNegative) res = '-' + res;
	}
	else if ((*this > other && !isNegative) || (*this < other && isNegative)) {
		while (i >= 0) {
			sub = digits[i--] - borrow;
			if (j >= 0) sub -= other.digits[j--];

			if (sub < 0) {
				sub += 10;
				borrow = 1;
			}
			else borrow = 0;

			res.insert(res.begin(), sub + '0');
		}
		if (isNegative) res = '-' + res;
	}
	else return NULL_BI;

	if (res[0] == '-') while (!(res[1] - '0')) res.erase(1, 1);
	else while (!(res[0] - '0')) res.erase(0, 1);

	return BigInteger(res);
}

//	Multiplication of big integers
BigInteger BigInteger::operator*(BigInteger& other)
{
	int n = std::max(digits.size(), other.digits.size());

	if (*this == NULL_BI || other == NULL_BI || n == 0)
		return NULL_BI;

	if (n <= TRESHOLD)
		return simpMlt(other);

	if (n / std::min(digits.size(), other.digits.size()) >= 2)
	{
		BigInteger part1;
		BigInteger part2;
		BigInteger& temp = (digits.size() > other.digits.size() ? *this : other);
		BigInteger& temp2 = (digits.size() < other.digits.size() ? *this : other);
		part1.digits = std::vector<uint8_t>(temp.digits.begin(), temp.digits.begin() + (n - n / 2));
		part2.digits = std::vector<uint8_t>(temp.digits.begin() + (n - n / 2), temp.digits.end());
		int sizePart = part2.digits.size();
		part1 = part1 * temp2;
		part2 = part2 * temp2;
		return part1.shift_left(sizePart) + part2;
	}

	this->extend(n);
	other.extend(n);

	int m = n / 2;

	if (n - m > digits.size() || n - m > other.digits.size() ||
		m > digits.size() || m > other.digits.size() ||
		m == 0 || n - m == 0)
		return simpMlt(other);

	BigInteger a1, a0, b1, b0;
	a1.digits = std::vector<uint8_t>(this->digits.begin(), this->digits.begin() + (n - m));
	a0.digits = std::vector<uint8_t>(this->digits.begin() + (n - m), this->digits.end());
	b1.digits = std::vector<uint8_t>(other.digits.begin(), other.digits.begin() + (n - m));
	b0.digits = std::vector<uint8_t>(other.digits.begin() + (n - m), other.digits.end());

	BigInteger p0 = a0 * b0;
	BigInteger p1 = a1 * b1;
	BigInteger ad1 = a0 + a1;
	BigInteger ad2 = b0 + b1;
	BigInteger p2 = ad1 * ad2;

	BigInteger term1 = p1.shift_left(2 * m);
	BigInteger term2 = (p2 - p1 - p0).shift_left(m);

	BigInteger res = term1 + term2 + p0;

	while (res.digits.size() > 1 && res.digits[0] == 0) {
		res.digits.erase(res.digits.begin());
	}

	if (isNegative != other.isNegative) res.isNegative = true;

	return res;
}

//	Division of big integers
BigInteger BigInteger::operator/(const BigInteger& other) const
{
	if (other == NULL_BI) throw std::string("Division by zero");
	if (*this == NULL_BI || (*this < other && !isNegative)) return NULL_BI;
	if (other == BigInteger("1")) return *this;

	std::string quo;
	BigInteger rem;
	uint8_t digit = 0;

	for (size_t i = 0; i < digits.size(); i++) {
		rem.digits.push_back(digits[i]);
		rem.normalize();

		digit = 0;
		while (other <= rem) {
			rem -= other;
			digit++;
		}
		quo.push_back(digit + '0');
	}

	while (!(quo[0] - '0')) quo.erase(0, 1);
	if (isNegative != other.isNegative) quo = '-' + quo;

	return BigInteger(quo);
}

//	Division of big integers by modulo
BigInteger BigInteger::operator%(const BigInteger& other) const
{
	if (other == NULL_BI) throw std::string("Division by zero");
	if (*this < other) return *this;
	if (*this == NULL_BI) return NULL_BI;
	if (other == BigInteger("1")) return NULL_BI;

	BigInteger rem;

	for (size_t i = 0; i < digits.size(); i++) {
		rem.digits.push_back(digits[i]);
		rem.normalize();

		while (other <= rem)
			rem -= other;
	}

	return rem.normalize();
}

BigInteger BigInteger::operator++(int)
{
	BigInteger temp = *this;
	++*this;
	return *this;
}

BigInteger BigInteger::operator--(int)
{
	BigInteger temp = *this;
	--*this;
	return *this;
}

//	Comparison of big integers by <
bool BigInteger::operator<(const BigInteger& other) const
{
	if (isNegative != other.isNegative) return isNegative;

	if (!isNegative && !other.isNegative) {
		if (digits.size() != other.digits.size())
			return digits.size() < other.digits.size();

		for (size_t i = 0; i < digits.size(); i++)
			if (digits[i] != other.digits[i])
				return digits[i] < other.digits[i];
	}
	else {
		if (digits.size() != other.digits.size())
			return digits.size() > other.digits.size();

		for (size_t i = 0; i < digits.size(); i++)
			if (digits[i] != other.digits[i])
				return digits[i] > other.digits[i];
	}

	return false;
}

bool BigInteger::operator==(const BigInteger& other) const
{
	if (digits.size() == other.digits.size() && isNegative == other.isNegative)
	{
		for (size_t i = 0; i < digits.size(); i++)
			if (digits[i] != other.digits[i])
				return false;
		return true;
	}
	return false;
}


//		*****************-OUT-OF-*****************


std::istream& operator>>(std::istream& in, BigInteger& num) {
	std::string temp;
	in >> temp;
	num = BigInteger(temp);
	return in;
}

std::ostream& operator<<(std::ostream& out, const BigInteger& num) { return out << num.to_string(); }

BigInteger operator+(const int& num1, const BigInteger& num2) { return num2 + num1; }

BigInteger operator-(const int& num1, const BigInteger& num2) { return BigInteger(std::to_string(num1)) - num2; }

BigInteger operator*(const int& num1, BigInteger& num2) { return num2 * num1; }

BigInteger operator/(const int& num1, BigInteger& num2) { return BigInteger(std::to_string(num1)) / num2; }

BigInteger operator%(const int& num1, BigInteger& num2) { return BigInteger(std::to_string(num1)) % num2; }

bool operator<(const int& num1, const BigInteger& num2) { return num2 > num1; }

bool operator>(const int& num1, const BigInteger& num2) { return num2 < num1; }

bool operator<=(const int& num1, const BigInteger& num2) { return num2 >= num1; }

bool operator>=(const int& num1, const BigInteger& num2) { return num2 <= num1; }

bool operator==(const int& num1, const BigInteger& num2) { return num2 == num1; }

bool operator!=(const int& num1, const BigInteger& num2) { return num2 != num1; }