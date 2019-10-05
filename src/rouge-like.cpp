// rouge-like.cpp: 定义应用程序的入口点。
//

#include "rouge-like.h"

using namespace std;

int main()
{
	game = make_shared<Game>(Game());
	game->init();
	game->start();
	return 0;
}
