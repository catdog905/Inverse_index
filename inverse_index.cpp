#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <utility>

class InverseIndex{
	public:
	InverseIndex(std::string input_string) {
		forwardWalk(input_string);
	}
	std::vector<int> get_word_start_positions(std::string str){
		return word_pos_map_[str];
	}

	private:
	std::map<std::string, std::vector<int> > word_pos_map_;
	void forwardWalk(std::string input_string){
		std::string cur_word ("");
		int start_pos = 0;
		for (int i = 0; i < input_string.size(); i++){
			if (input_string[i] == ' ') {
				add_word_to_map(cur_word, start_pos);
				cur_word = "";
				start_pos = i+1;
			} else {
				cur_word += input_string[i];
			}
			
		}
		add_word_to_map(cur_word, start_pos);
	}
	void add_word_to_map(std::string cur_word, int start_pos) {
		auto search = word_pos_map_.find(cur_word);
		if (search == word_pos_map_.end()){
			std::vector<int> v {start_pos};
			word_pos_map_[cur_word] = v;
		} else {
			word_pos_map_[cur_word].push_back(start_pos);
		}
	}
};

class FileInverseIndex{
	public:
	FileInverseIndex(std::string file_name) : inverse_index_(read_file(file_name)){
	}
	std::vector<int> get_word_start_positions(std::string str){
		return inverse_index_.get_word_start_positions(str);
	}

	private:
	InverseIndex inverse_index_;
	std::string read_file(std::string file_name){
		std::ifstream file(file_name);
		std::string line;
		std::string input_string = "";
		while (std::getline(file, line))
		{
			input_string += line;
		}
		file.close();
		return input_string;
	}
};

class MultifileInverseIndex{
	public:
	MultifileInverseIndex(std::vector<std::string> file_names){
		for (auto file_name : file_names){
			FileInverseIndex file_inverse_index(file_name);
			file_inverse_indexes_.push_back(std::make_pair(file_name, file_inverse_index));
		}
	}
	std::vector<std::pair<std::string, std::vector<int> > > get_word_start_positions(std::string str){
		std::vector<std::pair<std::string, std::vector<int> > > count;
		for(auto elem : file_inverse_indexes_){
			count.push_back(std::make_pair(elem.first, elem.second.get_word_start_positions(str)));
		}
		return count;
	}

	private:
	std::vector<std::pair<std::string,FileInverseIndex> > file_inverse_indexes_;
};
/*
int main(){
	std::string str = "Alice Bob Alice Carl";
	InverseIndex inverse_index(str);
	std::vector<int> temp(inverse_index.get_word_start_positions("Alice"));
	for (auto x : temp){
		std::cout << x << std::endl;
	}
	return 0;
}
*/
unsigned int Factorial() {
    return 1;
}

TEST_CASE( "Inverse indexes use", "[inverse_indexes]" ) {
	SECTION("reading from input string"){
		std::string str = "Alice Bob Alice Carl";
		InverseIndex inverse_index(str);
		std::vector<int> temp(inverse_index.get_word_start_positions("Alice"));
		REQUIRE(temp[0] == 0);
		REQUIRE(temp[1] == 10);
		temp = inverse_index.get_word_start_positions("Bob");
		REQUIRE(temp[0] == 6);
		temp = inverse_index.get_word_start_positions("Carl");
		REQUIRE(temp[0] == 16);
	}
	SECTION("reading from file"){
		std::ofstream test_file ( "test.txt" );
		test_file << "Alice Bob Alice Carl";
		test_file.close();
		FileInverseIndex file_inverse_index("test.txt");
		std::vector<int> temp(file_inverse_index.get_word_start_positions("Alice"));
		REQUIRE(temp[0] == 0);
		REQUIRE(temp[1] == 10);
		temp = file_inverse_index.get_word_start_positions("Bob");
		REQUIRE(temp[0] == 6);
		temp = file_inverse_index.get_word_start_positions("Carl");
		REQUIRE(temp[0] == 16);
	}
	SECTION("reading from lots of file"){
		std::ofstream test_file1 ( "test1.txt" );
		test_file1 << "Alice Bob Alice Carl";
		test_file1.close();
		std::ofstream test_file2 ( "test2.txt" );
		test_file2 << "Carl Alice Bob Alice";
		test_file2.close();
		std::ofstream test_file3 ( "test3.txt" );
		test_file3 << "Bob Bob Carl Alice";
		test_file3.close();
		std::vector<std::string> file_names{"test1.txt", "test2.txt", "test3.txt" };
		MultifileInverseIndex multi_file_inverse_index(file_names);

		std::vector<std::pair<std::string, std::vector<int> > > temp = multi_file_inverse_index.get_word_start_positions("Alice");
		REQUIRE(temp[0].first == "test1.txt");
		REQUIRE((temp[0].second)[0] == 0);
		REQUIRE((temp[0].second)[1] == 10);
		REQUIRE(temp[1].first == "test2.txt");
		REQUIRE((temp[1].second)[0] == 5);
		REQUIRE((temp[1].second)[1] == 15);
		REQUIRE(temp[2].first == "test3.txt");
		REQUIRE((temp[2].second)[0] == 13);

		temp = multi_file_inverse_index.get_word_start_positions("Bob");
		REQUIRE(temp[0].first == "test1.txt");
		REQUIRE((temp[0].second)[0] == 6);
		REQUIRE(temp[1].first == "test2.txt");
		REQUIRE((temp[1].second)[0] == 11);
		REQUIRE(temp[2].first == "test3.txt");
		REQUIRE((temp[2].second)[0] == 0);
		REQUIRE((temp[2].second)[1] == 4);

		temp = multi_file_inverse_index.get_word_start_positions("Carl");
		REQUIRE(temp[0].first == "test1.txt");
		REQUIRE((temp[0].second)[0] == 16);
		REQUIRE(temp[1].first == "test2.txt");
		REQUIRE((temp[1].second)[0] == 0);
		REQUIRE(temp[2].first == "test3.txt");
		REQUIRE((temp[2].second)[0] == 8);
	}
}

