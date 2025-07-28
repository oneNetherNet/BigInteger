#pragma once
#include "bigInteger.h"

#define PRIVATE_EXPONENT 65537

struct key {
	BigInteger a;
	BigInteger b;
};

class RSA {
private:
	BigInteger p;
	BigInteger q;
	key publicKey;
	key privateKey;
	char block_char() const;
	char to_char(const std::string& str) const;
	std::string str_converter(const std::string& str) const;
	std::string str_unconverter(const std::string& str) const;

public:
	RSA();
	static int dfile_count;
	static int efile_count;
	key getPrivateKey() const { return privateKey; }
	key getPublicKey() const { return publicKey; }
	void setPrivateKey(BigInteger n, BigInteger d) {  privateKey.a = n; privateKey.b = d; }
	void setPublicKey(BigInteger n, BigInteger e) { publicKey.a = n; publicKey.b = e; }
	std::string encrypt(const std::string txt) const;
	std::string decrypt(const std::string txt) const;
	std::string encrypt(std::ifstream& file) const;
	std::string decrypt(std::ifstream& txt) const;
};

std::ostream& operator<<(std::ostream& out, const RSA& crypt);