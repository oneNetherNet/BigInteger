# BigInteger library
Учебный проект, представляющий из себя реализацию хранения и оперирования длинными числами для реализации алгоритмов ассиметричного шифрования.
## Пример использования для текстовых файлов
```
...
std::ifstream file("test1.txt");
RSA test;
test.encrypt(file);
std::ifstream efile("encrypted_file(1).txt");
test.decrypt(efile);
...
```
Библиотека работает с текстовыми файлами и строками(string).
