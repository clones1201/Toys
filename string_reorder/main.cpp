#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

const std::string invaildInputString = "<invalid input string>";

#define IS_VALID_CHAR(c) (c <= '9' && c >='0' || c <='z' && c >='a')
using namespace std;
int main(){
	std::vector< std::vector<char> > result;

	char buf[500];
	while ( std::cin.getline(buf,500) ){
		std::string str(buf);
		if (str.size() == 0)
			continue;

		std::vector<char> line;
		try{
			for (int i = 0; i < str.size(); i++){
				if (!IS_VALID_CHAR(str[i])){
					throw int(1);
				}
				else{
					line.push_back(str[i]);
				}
			}
		}
		catch (int){
			line.clear();
			for (int i = 0; i < invaildInputString.size(); i++){
				line.push_back(invaildInputString[i]);
			}
			result.push_back(line);
			continue;
		}

		std::sort(line.begin(), line.end());


		vector<char>::iterator iter1 = line.begin();
		vector<char>::iterator iter2 = line.begin();
		iter2++;
		vector< vector<char> > Map;
		while (iter2 != line.end()){
			vector<char> Set;
			Set.push_back(*iter1);
			if (*iter2 == *iter1){
				Set.push_back(*iter2);
				iter2++;
				while (iter2 != line.end() && *iter2 == *iter1){
					Set.push_back(*iter2);
					iter2++;
				}
				if (iter2 == line.end()){ Map.push_back(Set); break;}
				iter1 = iter2;
				iter1++;
				iter2++;				
			}
			Map.push_back(Set);
		}
		line.clear();
		while (Map.size() != 0){
			for (vector<vector<char> >::iterator iter = Map.begin();
				iter != Map.end();){
				if ((*iter).size() != 0){
					line.push_back((*iter).back());
					(*iter).pop_back();
					if ((*iter).size() == 0){
						iter = Map.erase(iter);
						continue;
					}
				}
				++iter;
			}
		}

		result.push_back(line);
	
	}
	for (int i = 0; i < result.size(); i++){
		for (int j = 0; j < result[i].size(); j++)
			std::cout << result[i][j];
		std::cout << std::endl;
	}
}