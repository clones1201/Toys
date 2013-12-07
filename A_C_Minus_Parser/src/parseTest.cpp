
#include "cminus.h"

int main(int argc, char * argv[])
{
	if (argc != 2)
	{
		cerr<<"No File Name";
		exit(1);
	}
	ifstream pFile(argv[1],ios_base::in);
	if(!pFile.is_open())
	{
		cerr<<"No such File";
		exit(1);
	}

    string outputFile;
    outputFile.append(argv[1]);
    outputFile = outputFile.substr(0,outputFile.size()-3);
    outputFile.append(".lextext");
	Scaner scan(pFile);
    scan.Scan(outputFile);
    cout<<"输出文件："<<outputFile<<endl;

    Parse parse(scan);

    Node* parseTree = parse.program();

#ifdef _DEBUG
    parse.printTree( parseTree );
#endif
    cout<<"按下任何键继续..."<<endl;
    cin.get();
    return 0;
}
