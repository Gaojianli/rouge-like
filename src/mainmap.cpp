// Mainmap todo
#include "mainmap.h"
#include "map.h"
MainMap::MainMap()
{
	for (int i = 0; i < mapSize; i++)
	{
		std::vector<Map> thisLine;
		for (int j = 0; j < mapSize; j++) // i for y, j for x
		{
			Map oneMap;
			if (0 == i)
			{
				oneMap.setDisabledGate(1);
				if (0 == j)
				{
					oneMap.setPortal(3);
				}
				else if ((mapSize - 1) == j)
				{
					oneMap.setPortal(1);
				}
			}
			else if ((mapSize - 1) == i)
			{
				oneMap.setDisabledGate(0);
				if ((mapSize - 1) == j)
				{
					oneMap.setPortal(2);
				}
				else if (0 == j)
				{
					oneMap.setPortal(4);
				}
			}
			if (0 == j)
			{
				oneMap.setDisabledGate(2);
			}
			if ((mapSize - 1) == j)
			{
				oneMap.setDisabledGate(3);
			}
			thisLine.push_back(oneMap);
		}
		maps.push_back(thisLine);
		std::reverse(maps.begin(), maps.end());
	}
}
bool MainMap::SetMapLocation(int x, int y)
{
	if ((x < 0) || (x >= mapSize) || (y < 0) || (y >= mapSize))
	{
		return false;
	}
	else
	{
		mainmap_x = x;
		mainmap_y = y;
		return true;
	}
}
int MainMap::GetMapXLocation()
{
	return mainmap_x;
}
int MainMap::GetMapYLocation()
{
	return mainmap_y;
}
bool MainMap::isOutOfRange(int x, int y)
{
	if ((x < 0) || (x >= mapSize) || (y < 0) || (y >= mapSize))
	{
		return true;
	}
	else
	{
		return false;
	}
}