#include <iostream>
#include <vector>
#include <string>
#include <algorithm> 

const std::string invaildInputString = "<invalid input string>";

#define IS_VALID_CHAR(c) (c <= '9' && c >='0' || c <='z' && c >='a')
using namespace std;
int main(){
	std::vector< int > result;

	char buf[500];
	while (std::cin.getline(buf, 500)){
		std::string str(buf);
		str.push_back(',');
		if (str.size() == 0)
			continue;

		vector<int> line;
		while (str.size()!=0){
			int num = atoi(str.substr(0, str.find(',')).c_str());
			line.push_back(num);
			str = str.substr(str.find(',')+1, str.size());
		}
		int count = 0;
		int max_inverse = 0;
		for (int i = 0; i < line.size(); ++i){
			int inverse = 0;
			for (int j = i + 1; j < line.size(); ++j){
				if (line[i] > line[j]){
					count++;
					inverse++;
				}
			}
			int newCount = 0;
			for (int j = i + 1; j < line.size() - 1; ++j){
				if (line.back() > line[j]){
					newCount++;
				}
			}
			if ( inverse - newCount > max_inverse)
				max_inverse = inverse - newCount;
		}
		result.push_back(count - max_inverse);
	}
	for (int i = 0; i <result.size(); i++){
		cout << result[i] << endl;
	}
}