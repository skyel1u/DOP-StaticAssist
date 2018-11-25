#include <iostream>
#include <fstream>
#include "picosha2.h"

// 从string生成
void from_string()
{
	std::string src_str = "Hello";
	std::string hash_hex_str;
	std::vector<unsigned char> hash(picosha2::k_digest_size);
	picosha2::hash256(src_str.begin(), src_str.end(), hash.begin(), hash.end());
	hash_hex_str = picosha2::bytes_to_hex_string(hash.begin(), hash.end());
	std::cout << hash_hex_str << std::endl;
	//this output is "185f8db32271fe25f561a6fc938b2e264306ec304eda518007d1764826381969"

	picosha2::hash256_hex_string(src_str, hash_hex_str);
	std::cout << hash_hex_str << std::endl;
	//this output is "185f8db32271fe25f561a6fc938b2e264306ec304eda518007d1764826381969"

	hash_hex_str = picosha2::hash256_hex_string(src_str);
	std::cout << hash_hex_str << std::endl;
	//this output is "185f8db32271fe25f561a6fc938b2e264306ec304eda518007d1764826381969"

	src_str = "Hello!"; //add '!'
	hash_hex_str = picosha2::hash256_hex_string(src_str.begin(), src_str.end());
	std::cout << hash_hex_str << std::endl;
	//this output is "334d016f755cd6dc58c53a86e183882f8ec14f52fb05345887c8a5edd42c87b7"
}

//从字节数组读取
void from_bytes()
{
	std::vector<unsigned char> src_vect(5, '\0');
	src_vect.at(0) = 'H';
	src_vect.at(1) = 'e';
	src_vect.at(2) = 'l';
	src_vect.at(3) = 'l';
	src_vect.at(4) = 'o';
	std::string hash_hex_str;
	picosha2::hash256_hex_string(src_vect, hash_hex_str);
	std::cout << hash_hex_str << std::endl;
	//this output is "185f8db32271fe25f561a6fc938b2e264306ec304eda518007d1764826381969"

	std::vector<unsigned char> _src_vect(picosha2::k_digest_size, '\0');
	_src_vect.at(0) = 'H';
	_src_vect.at(1) = 'e';
	_src_vect.at(2) = 'l';
	_src_vect.at(3) = 'l';
	_src_vect.at(4) = 'o';
	hash_hex_str = picosha2::hash256_hex_string(_src_vect);
	std::cout << hash_hex_str << std::endl;
	//this output is "cd4895ecd6438e664b91e74f0e632438cd092e27da585f77854e5c28239d2f76"

	unsigned char src_c_array[5] = {'H', 'e', 'l', 'l', 'o'};
	picosha2::hash256_hex_string(src_c_array, src_c_array + 5, hash_hex_str);
	std::cout << hash_hex_str << std::endl;
	//this output is "185f8db32271fe25f561a6fc938b2e264306ec304eda518007d1764826381969"

	unsigned char _src_c_array[picosha2::k_digest_size] = {'H', 'e', 'l', 'l', 'o'};
	hash_hex_str = picosha2::hash256_hex_string(_src_c_array, _src_c_array + picosha2::k_digest_size);
	std::cout << hash_hex_str << std::endl;
	//this output is "cd4895ecd6438e664b91e74f0e632438cd092e27da585f77854e5c28239d2f76"
}

//从文件读取
void from_file()
{
	std::ifstream ifs;
	ifs.open("file.txt", std::ios::binary); //file data is "Hello"

	std::vector<unsigned char> hash(picosha2::k_digest_size);
	picosha2::hash256(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>(), hash.begin(), hash.end());
	std::string hex_str = picosha2::bytes_to_hex_string(hash.begin(), hash.end());
	std::cout << hex_str << std::endl;
	//this output is "185f8db32271fe25f561a6fc938b2e264306ec304eda518007d1764826381969"
}

//从标准输入读取
void from_input()
{
	std::cout << "Input freely. To get hash, input \"hash!\". " << std::endl;
	picosha2::hash256_one_by_one hasher;
	while (true)
	{
		hasher.init(); //reset hasher state
		while (true)
		{
			std::string line;
			std::getline(std::cin, line);
			if (line == "hash!")
			{
				break;
			}
			hasher.process(line.begin(), line.end());
		}
		hasher.finish();
		std::string hex_str;
		picosha2::get_hash_hex_string(hasher, hex_str);
		std::cout << hex_str << std::endl;
	}
}

int main(int argc, char *argv[])
{
	// from_string();

	// from_bytes();

	// from_file();

	// from_input();

	std::cout << "SHA256" << std::endl;
	return 0;
}