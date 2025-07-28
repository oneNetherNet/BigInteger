#include "RSA.h"
std::random_device rde;

int RSA::dfile_count = 1;
int RSA::efile_count = 1;

char RSA::to_char(const std::string& str) const
{
	int temp = 0;
	for (int i = 0; i < str.size(); i++)
		temp += (str[i] - '0') * pow(10, str.size() - (i + 1));
	return temp;
}

char RSA::block_char() const
{
	int c = rde() % 127;
	while (c >= 48 && c <= 57 && c != ' ' && c != '\n') c = rde() % 127;
	return c;
}

RSA::RSA()
{
	//	--------PUBLIC_KEY--------

	p = p.genPrime(16);
	q = q.genPrime(16);
	while (p == q) q = q.genPrime(16);

	BigInteger N = p * q;
	BigInteger ta = p - 1;
	BigInteger tb = q - 1;
	BigInteger f = ta * tb;

	BigInteger e = PRIVATE_EXPONENT;

	publicKey = { N, e };

	//	--------PRIVATE_KEY--------

	BigInteger d = d.getD(e, f);

	privateKey = { N, d };
}

std::string RSA::str_unconverter(const std::string& str) const
{
	std::string res;
	std::string temp;
	for (int i = 1; i < str.size(); i++) {
		if (i != 1 && (i - 1) % 3 == 0) {
			res.push_back(to_char(temp));
			temp = "";
		}
		temp.push_back(str[i]);
	}
	res.push_back(to_char(temp));
	return res;
}

std::string RSA::str_converter(const std::string& str) const
{
	std::string res = "1";
	for (char c : str) {
		if (c >= 100) res += std::to_string(static_cast<int>(c));
		else if (c < 100 && c >= 10) res += '0' + std::to_string(static_cast<int>(c));
		else if (c < 10) res += "00" + std::to_string(static_cast<int>(c));
	}
	return res;
}

std::string RSA::encrypt(const std::string txt) const
{
	int block_size = ((p.getSize() - 1) + (q.getSize() - 1)) / 3;
	std::string buffer;
	BigInteger block;
	std::string res;

	for (int i = 0; i < txt.size(); i++) {
		if ((i + 1) % block_size == 0) {
			buffer = str_converter(buffer);
			block = buffer;
			res += block.pow_mod(publicKey.b, publicKey.a).to_string() + block_char();
			buffer = "";
		}
		buffer.push_back(txt[i]);
	}

	if (buffer.size()) {
		buffer = str_converter(buffer);
		block = buffer;
		res += block.pow_mod(publicKey.b, publicKey.a).to_string() + block_char();
	}

	return res;
}

std::string RSA::decrypt(const std::string txt) const
{
	BigInteger block;
	std::string buffer;
	std::string res;

	for (int i = 0; i < txt.size(); i++) {
		if (txt[i] < 48 || txt[i] > 57) {
			block = buffer;
			buffer = block.pow_mod(privateKey.b, privateKey.a).to_string();
			res += str_unconverter(buffer);
			buffer = "";
			continue;
		}
		buffer.push_back(txt[i]);
	}

	return res;
}

std::string RSA::encrypt(std::ifstream& file) const
{
	std::string temp;
	std::string msg((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());
	std::string file_name = "encrypted_file(" + std::to_string(efile_count) + ").txt";
	std::ofstream e_file(file_name);
	if (!e_file) {
		throw std::runtime_error("Cannot open output file");
	}
	efile_count++;
	msg = encrypt(msg);
	e_file << msg;
	e_file.close();
	return msg;
}

std::string RSA::decrypt(std::ifstream& file) const
{
	std::string msg((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());
	std::string file_name = "decrypted_file(" + std::to_string(dfile_count) + ").txt";
	std::ofstream e_file(file_name);
	if (!e_file) {
		throw std::runtime_error("Cannot open output file");
	}
	dfile_count++;
	msg = decrypt(msg);
	e_file << msg;
	e_file.close();
	return msg;
}

std::ostream& operator<<(std::ostream& out, const RSA& crypt)
{
	std::string temp = "PRIVATE KEY: " + crypt.getPrivateKey().a.to_string() + ' ' +
		crypt.getPrivateKey().b.to_string() + '\n' + '\n' +
		"PUBLIC KEY: " + crypt.getPublicKey().a.to_string() + ' ' +
		crypt.getPublicKey().b.to_string();
	return out << temp;
}