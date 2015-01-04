//题目1: String reorder
//时间限制 : 10000ms
//   单点时限 : 1000ms
//	  内存限制 : 256MB
//			Description
//			 For this question, your program is required to process an input string containing only ASCII characters between ‘0’ and ‘9’, or between ‘a’ and ‘z’(including ‘0’, ‘9’, ‘a’, ‘z’).
//
//			 Your program should reorder and split all input string characters into multiple segments, and output all segments as one concatenated string.The following requirements should also be met,
//			 1. Characters in each segment should be in strictly increasing order.For ordering, ‘9’ is larger than ‘0’, ‘a’ is larger than ‘9’, and ‘z’ is larger than ‘a’(basically following ASCII character order).
//			 2. Characters in the second segment must be the same as or a subset of the first segment; and every following segment must be the same as or a subset of its previous segment.
//
//			 Your program should output string “<invalid input string>” when the input contains any invalid characters(i.e., outside the '0' - '9' and 'a' - 'z' range).
//
//
//
//			 Input
//
//			 Input consists of multiple cases, one case per line.Each case is one string consisting of ASCII characters.
//
//			 Output
//
//			 For each case, print exactly one line with the reordered string based on the criteria above.
//
//
//			 样例输入
//			 aabbccdd
//			 007799aabbccddeeff113355zz
//			 1234.89898
//			 abcdefabcdefabcdefaaaaaaaaaaaaaabbbbbbbddddddee
//			 样例输出
//			 abcdabcd
//			 013579abcdefz013579abcdefz
//			 <invalid input string>
//			 abcdefabcdefabcdefabdeabdeabdabdabdabdabaaaaaaa

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
		 while (std::cin.getline(buf, 500)){
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
					 if (iter2 == line.end()){ Map.push_back(Set); break; }
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