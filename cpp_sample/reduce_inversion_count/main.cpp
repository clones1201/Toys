//题目3: Reduce inversion count
//时间限制 : 10000ms
//   单点时限 : 1000ms
//	  内存限制 : 256MB
//			 Description
//			 Find a pair in an integer array that swapping them would maximally decrease the inversion count of the array.If such a pair exists, return the new inversion count; otherwise returns the original inversion count.
//
//			 Definition of Inversion : Let(A[0], A[1] ... A[n], n <= 50) be a sequence of n numbers.If i < j and A[i] > A[j], then the pair(i, j) is called inversion of A.
//
//		 Example :
//				 Count(Inversion({ 3, 1, 2 })) = Count({ 3, 1 }, { 3, 2 }) = 2
//				 InversionCountOfSwap({ 3, 1, 2 }) = >
//				{
//					InversionCount({ 1, 3, 2 }) = 1 <--swapping 1 with 3, decreases inversion count by 1
//					InversionCount({ 2, 1, 3 }) = 1 <--swapping 2 with 3, decreases inversion count by 1
//					InversionCount({ 3, 2, 1 }) = 3 <--swapping 1 with 2, increases inversion count by 1
//				}
//
//
//
//	 Input
//		 Input consists of multiple cases, one case per line.Each case consists of a sequence of integers separated by comma.
//
//		 Output
//		 For each case, print exactly one line with the new inversion count or the original inversion count if it cannot be reduced.
//
//
//		 样例输入
//		 3, 1, 2
//		 1, 2, 3, 4, 5
//		 样例输出
//		 1
//		 0
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