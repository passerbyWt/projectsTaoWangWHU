#include "Astar.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Astar astar;
	Node* startPos = new Node(5, 1);
	Node* endPos = new Node(3, 8);

	//astar.printMap();
	astar.search(startPos, endPos);
	cout << endl;
	astar.printMap();
	system("pause");
	return 0;
}
