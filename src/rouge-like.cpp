// rouge-like.cpp: 定义应用程序的入口点。
//

#include "rouge-like.h"

using namespace std;

int main()
{
	auto game = Game();
	game.init();
	game.start();
	return 0;
}
