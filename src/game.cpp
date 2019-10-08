#include "game.h"
#include "bottle.h"
#include "monster.h"
#include "mankind.h"
#include "mainmap.h"
#include "weapons.h"
#include "curses/curses.h"
#include <cmath>
#include <functional>
#include <time.h>
using std::string;

void Game::init()
{
	initscr();
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK); // normal
	init_pair(2, COLOR_BLACK, COLOR_WHITE); // selected
	init_pair(3, COLOR_RED, COLOR_BLACK);   // invalid
	init_pair(4, COLOR_WHITE, COLOR_GREEN);
	init_pair(5, COLOR_WHITE, COLOR_YELLOW);
	curs_set(0);
	raw();
	noecho();
	// Set time based random seed
	std::srand(unsigned(time(nullptr)));
	// Generate items
	std::vector<gameObject*> itemToDistribute;
	for (int i = 0; i < std::rand() % 8 + 16; i++) // Add bottle to item list
	{
		itemToDistribute.push_back(new Bottle());
	}
	for (int j = 0; j < 5; j++) // Add keys to item list
	{
		itemToDistribute.push_back(new Key(static_cast<Directions>(j), 1));
	}
	for (int k = 0; k < 12; k++) // Add sword to item list
	{
		switch (std::rand() % 3)
		{
		case 0: {
			itemToDistribute.push_back(new Weapons(WeaponsType::sword, std::rand() % 5 + 5, 0, 0));
			break;
		}
		case 1: {
			itemToDistribute.push_back(new Weapons(WeaponsType::shield, 0, std::rand() % 3 + 5, 0));
			break;
		}
		case 2: {
			itemToDistribute.push_back(new Weapons(WeaponsType::cane, std::rand() % 3 + 3, 0, std::rand() % 20 + 20));
			break;
		}
		default:
			break;
		}
	}
	for (int l = 0; l < 3; l++) // Add Monster to item list
	{
		switch (std::rand() % 5)
		{
		case 0:
		{
			Monster* m = new Monster({ 0, 0 }, ("Slime" + std::to_string(l)).c_str(), MonsterType::slime, static_cast<attitudes>(std::rand() % 2));
			itemToDistribute.push_back(m);
			characters.push_back(m);
			break;
		}
		case 1:
		{
			Monster* m = new Monster({ 0, 0 }, ("Skeleton" + std::to_string(l)).c_str(), MonsterType::skeleton, static_cast<attitudes>(std::rand() % 2));
			itemToDistribute.push_back(m);
			characters.push_back(m);
			break;
		}
		case 2:
		{
			Monster* m = new Monster({ 0, 0 }, ("Dragon" + std::to_string(l)).c_str(), MonsterType::dragon, static_cast<attitudes>(std::rand() % 2));
			itemToDistribute.push_back(m);
			characters.push_back(m);
			break;
		}
		case 3:
		{
			Monster* m = new Monster({ 0, 0 }, ("Snake" + std::to_string(l)).c_str(), MonsterType::snake, static_cast<attitudes>(std::rand() % 2));
			itemToDistribute.push_back(m);
			characters.push_back(m);
			break;
		}
		case 4:
		{
			Monster* m = new Monster({ 0, 0 }, ("Tarrasque" + std::to_string(l)).c_str(), MonsterType::tarrasque, static_cast<attitudes>(std::rand() % 2));
			itemToDistribute.push_back(m);
			characters.push_back(m);
			break;
		}
		default:
			break;
		}
	}
	const std::string roles[] = {
		"magician",
		"wizard",
		"warrior",
		"harpy",
		"amazon",
		"dwarf",
		"monkey" };
	for (int m = 0; m < 3; m++) // Generate three human NPC
	{
		int role = std::rand() % 7;
		Mankind* p = new Mankind({ 0, 0 }, (roles[role] + std::to_string(m)).c_str(), static_cast<Role>(role), static_cast<attitudes>(std::rand() % 2));
		itemToDistribute.push_back(p);
		characters.push_back(p);
	}
	// Init Maps.
	globalMainMap = std::make_shared<MainMap>(MainMap());
	// Roll Map.
	globalMainMap->SetMapLocation(std::rand() % 4, std::rand() % 4);// Roll first rom
	globalMap = globalMainMap->GetCurrentMap();

	// Send Items to Map

	for (auto i : itemToDistribute) {
		(globalMainMap->GetMapAt(std::rand() % 4, std::rand() % 4))->randomSetThings(i);
	}
}

void Game::start()
{
	createPlayer();
	//enter main game
	move(2, 0);
	clrtobot();
	refresh();
	int ch;
	drawMain();
	printHelp();
	WINDOW** menu = nullptr;
	bool menuEnable[8] = { true,true,true,true,true,true,true,true };
	bool moveStatus = true;
	unsigned lastAttackRoundNumber = 0;
	while (!wined)
	{
		drawMap();
		drawPlayer();
		drawPlayerStatus();
		drawBackPack();
		moveStatus = false;
		auto oldPosition = player->position;
		int backpackScroll = -1;
		ch = getch();
		switch (ch)
		{
		case 'M':
		case 'm':
			menuEnable[static_cast<int>(MenuType::Attack)] = isAround(ObjectType::creature) && (lastAttackRoundNumber != roundNumber);
			menuEnable[static_cast<int>(MenuType::PickUp)] = isAround(ObjectType::item) && (player->backpack.size() < 4);
			menuEnable[static_cast<int>(MenuType::Control)] = canControlAround() && (lastAttackRoundNumber != roundNumber);
			menuEnable[static_cast<int>(MenuType::Investigate)] = (isAround(ObjectType::creature) || isAround(ObjectType::item));
			menuEnable[static_cast<int>(MenuType::Backpack)] = !player->backpack.empty();
			menu = drawMenu(menuEnable);
			switch (scrollMenu(menu, 8, menuEnable))
			{
			case MenuType::Attack:
				if(attack()) lastAttackRoundNumber = roundNumber;
				break;
			case MenuType::Backpack:
				drawBackPack();
				backpackScroll = scrollBackpack();
				if (backpackScroll != -1) useOrThrowBackpack(backpackScroll);
				break;
			case MenuType::PickUp:
				pickup();
				break;
			case MenuType::Investigate:
				investigate();
				break;
			case MenuType::Control:
				if(conjoure()) lastAttackRoundNumber = roundNumber;
				break;
			case MenuType::NextRound:
				nextRound();
				drawMap();
				break;
			case MenuType::Help:
				printHelp();
				break;
			case MenuType::Exit:
				exit(0);
				break;
			default:
				break;
			}
			deleteMenu(menu, 9);
			break;
		case 'W': // move
		case 'w':
			moveStatus = player->move(MoveDirection::up);
			break;
		case 'S':
		case 's':
			moveStatus = player->move(MoveDirection::down);
			break;
		case 'A':
		case 'a':
			moveStatus = player->move(MoveDirection::left);
			break;
		case 'D':
		case 'd':
			moveStatus = player->move(MoveDirection::right);
			break;
		default:
			break;
		}
		//move into room detect
		if (moveStatus) {
			auto x = globalMainMap->GetMapXLocation(), y = globalMainMap->GetMapYLocation();
			if (abs(oldPosition.first - player->position.first) > 1) {
				if (player->position.first == 0) {
					// right
					if (!globalMainMap->isOutOfRange(x + 1, y)) {
						globalMainMap->SetMapLocation(x + 1, y);
						globalMap = globalMainMap->GetCurrentMap();
						addInfo("You enter the room on the right.");
					}
					else if (globalMap->getPortal() > 0 && !globalMainMap->isOutOfRange(x - 3, y)) {
						globalMainMap->SetMapLocation(x - 3, y);
						globalMap = globalMainMap->GetCurrentMap();
						addInfo("You pass through the portal to the far left of the map.");
					}
				}
				else {
					// left
					if (!globalMainMap->isOutOfRange(x - 1, y)) {
						globalMainMap->SetMapLocation(x - 1, y);
						globalMap = globalMainMap->GetCurrentMap();
						addInfo("You enter the room on the left.");
					}
					else if (globalMap->getPortal() > 0 && !globalMainMap->isOutOfRange(x + 3, y)) {
						globalMainMap->SetMapLocation(x + 3, y);
						globalMap = globalMainMap->GetCurrentMap();
						addInfo("You pass through the portal to the far right of the map.");
					}
				}
			}
			else if (abs(oldPosition.second - player->position.second) > 1) {
				if (player->position.second == 0) {
					// up
					if (!globalMainMap->isOutOfRange(x, y + 1)) {
						globalMainMap->SetMapLocation(x, y + 1);
						globalMap = globalMainMap->GetCurrentMap();
						addInfo("You enter the room on the up.");
					}
					else if (globalMap->getPortal() > 0 && !globalMainMap->isOutOfRange(x, y - 3)) {
						globalMainMap->SetMapLocation(x, y - 3);
						globalMap = globalMainMap->GetCurrentMap();
						addInfo("You pass through the portal to the far down of the map.");
					}
				}
				else {
					// down
					if (!globalMainMap->isOutOfRange(x, y - 1)) {
						globalMainMap->SetMapLocation(x, y - 1);
						globalMap = globalMainMap->GetCurrentMap();
						addInfo("You enter the room on the down.");
					}
					else if (globalMap->getPortal() > 0 && !globalMainMap->isOutOfRange(x, y + 3)) {
						globalMainMap->SetMapLocation(x, y + 3);
						globalMap = globalMainMap->GetCurrentMap();
						addInfo("You pass through the portal to the far up of the map.");
					}
				}
			}
		}
	}
}

void Game::throwItem(int backpackIndex)
{
	const int directionTable[4][2] = { {0,1}, {0,-1}, {-1,0},{1,0} };
	auto item = player->backpack[backpackIndex];
	auto x = player->position.first, y = player->position.second;
	bool directions[4] = { false };
	for (auto direction : directionTable) {
		if (!globalMap->isOutOfRange(x + direction[0], y + direction[1])) {
			if (auto objectType = globalMap->getLocationType(x + direction[0], y + direction[1]); objectType == ObjectType::nothing) {
				if (direction[0] == 0)
					directions[direction[1] == 1 ? 0 : 1] = true;
				else
					directions[direction[0] == -1 ? 2 : 3] = true;
			}
		}
	}
	addInfo("If you want to throw something away, choose a direction.");
	Directions selected;
	if ((selected = scrollDirections(directions)) == Directions::win) {
		addInfo("You choose to cancel.");
		return;
	}
	auto direction = directionTable[static_cast<int>(selected)];
	auto it = player->backpack.begin() + backpackIndex;
	player->backpack.erase(it);
	globalMap->setGameObjectat(x + direction[0], y + direction[1], item);
	addInfo("You threw it on the ground.");
}

void Game::useItem(int backpackIndex)
{
	const int directionTable[4][2] = { {0,1}, {0,-1}, {-1,0},{1,0} };
	auto item = player->backpack[backpackIndex];
	bool useStatus = false;;
	if (item->getItemType() == ItemType::bottle) {
		auto bottle = dynamic_cast<Bottle*>(item);
		if (bottle->type == BottleType::bloodBottle) {
			useStatus = bottle->use(&*player);
			addInfo("Used blood bottle. HP up!");
		}
		else if (bottle->type == BottleType::manaBottle) {
			useStatus = bottle->use(&*player);
			addInfo("Used mana bottle. MP up!");
		}
		else {
			addInfo("If you want to use poison, select the direction first.");
			Directions selected;
			auto x = player->position.first, y = player->position.second;
			bool directions[4] = { false };
			for (auto direction : directionTable) {
				if (!globalMap->isOutOfRange(x + direction[0], y + direction[1])) {
					if (auto objectType = globalMap->getLocationType(x + direction[0], y + direction[1]); objectType != ObjectType::nothing) {
						if (objectType == ObjectType::creature) {
							auto creatureObject = globalMap->getLocationCreature(x + direction[0], y + direction[1]);
							if (auto monsterObj = dynamic_cast<Monster*>(creatureObject); monsterObj != nullptr) {
								if (monsterObj->beControlled == 0) {
									if (direction[0] == 0)
										directions[direction[1] == 1 ? 0 : 1] = true;
									else
										directions[direction[0] == -1 ? 2 : 3] = true;
								}
							}
							else {
								if (direction[0] == 0)
									directions[direction[1] == 1 ? 0 : 1] = true;
								else
									directions[direction[0] == -1 ? 2 : 3] = true;
							}
						}
					}
				}
			}
			if ((selected = scrollDirections(directions)) == Directions::win) {
				addInfo("You choose to cancel use.");
				return;
			}
			auto direction = directionTable[static_cast<int>(selected)];
			useStatus = bottle->use(globalMap->getLocationCreature(x + direction[0], y + direction[1]));
			addInfo("The monster was poisoned.");
		}
		if (useStatus) {
			auto it = player->backpack.begin() + backpackIndex;
			player->backpack.erase(it);
			delete item;
		}
		else
			addInfo("Use failed.");
	}
	else { // key
		auto key = dynamic_cast<Key*>(item);
		if (key->direction == Directions::win) {
			gotoWin();
			return;
		}
		if (key->used) {
			addInfo("This key has been used.");
			return;
		}
		auto direction = directionTable[static_cast<int>(key->direction)];
		auto x = globalMainMap->GetMapXLocation(), y = globalMainMap->GetMapYLocation();
		if (globalMainMap->isOutOfRange(x + direction[0] * key->step, y + direction[1] * key->step)) {
			addInfo("Destination beyond map boundary, player will move to boundary.");
			switch (key->direction)
			{
			case Directions::up:
				globalMainMap->SetMapLocation(x, 3);
				break;
			case Directions::down:
				globalMainMap->SetMapLocation(x, 0);
				break;
			case Directions::left:
				globalMainMap->SetMapLocation(0, y);
				break;
			case Directions::right:
				globalMainMap->SetMapLocation(3, y);
				break;
			default:
				break;
			}
		}
		else
		{
			addInfo("Arrive at your destination.");
			globalMainMap->SetMapLocation(x + direction[0] * key->step, y + direction[1] * key->step);
		}
		globalMap = globalMainMap->GetCurrentMap();
		key->useIt();
	}
}

template<typename Callback>
bool _isAround(Map* globalMap, std::shared_ptr<Player> player, Callback _callback) {
	const int directionTable[4][2] = { {1,0},{-1,0},{0,-1},{0,1} };
	auto playerPosition = player->position;
	auto x = playerPosition.first, y = playerPosition.second;
	auto flag = false;
	for (auto direction : directionTable) {
		if (!globalMap->isOutOfRange(x + direction[0], y + direction[1])) {
			_callback(x + direction[0], y + direction[1], flag);
		}
	}
	return flag;
}

bool Game::isAround(ObjectType target) {
	return _isAround(globalMap, player, [&](int x, int y, bool& flag) {
		if (globalMap->getLocationType(x, y) == target) {
			flag = true;
		}
		});
}

bool Game::canControlAround()
{
	return _isAround(globalMap, player, [&](int x, int y, bool& flag) {
		if (globalMap->getLocationType(x, y) == ObjectType::creature) {
			auto creature = globalMap->getLocationCreature(x, y);
			auto monster = dynamic_cast<Monster*>(creature);
			if (monster != nullptr && monster->beControlled == 0) {
				flag = true;
			}
		}
		});
}

void Game::drawMap()
{
	delwin(map);
	map = subwin(stdscr, 11, 20, 2, 1);
	wborder(map, '|', '|', '-', '-', '+', '+', '+', '+');
	auto mapStr = globalMap->drawablemap();
	auto gates = globalMap->getGates();
	for (int i = 0; i < 9; i++)
	{
		wmove(map, i + 1, 1);
		waddstr(map, mapStr[i].c_str());
	}
	if (gates[0]) // up
	{
		wmove(map, 0, 9);
		waddstr(map, "nn");
	}
	if (gates[1]) // down
	{
		wmove(map, 10, 9);
		waddstr(map, "nn");
	}
	if (gates[2]) // left
	{
		wmove(map, 5, 0);
		waddstr(map, "n");
	}
	if (gates[3]) //right
	{
		wmove(map, 5, 19);
		waddstr(map, "n");
	}
	auto portal = globalMap->getPortal();
	if (portal > 0) {
		auto x = globalMainMap->GetMapXLocation(), y = globalMainMap->GetMapYLocation();
		if (x == 0 && y == 0) {
			mvwaddstr(map, 5, 0, "X");
			mvwaddstr(map, 10, 9, "XX");
		}
		else if (x == 0 && y == 3) {
			mvwaddstr(map, 0, 9, "XX");
			mvwaddstr(map, 5, 0, "X");
		}
		else if (x == 3 && y == 3) {
			mvwaddstr(map, 0, 9, "XX");
			mvwaddstr(map, 5, 19, "X");
		}
		else {
			mvwaddstr(map, 10, 9, "XX");
			mvwaddstr(map, 5, 19, "X");
		}
	}
	wrefresh(map);
}

void Game::nextRound()
{
	player->movePoints = 5;//reset move points
	roundNumber++;
	//regenerate bottles
	globalMainMap->GetMapAt(rand() % 4, rand() % 4)
		->randomSetThings(new Bottle(static_cast<BottleType>(std::rand() % 3), 10));
	globalMainMap->GetMapAt(rand() % 4, rand() % 4)
		->randomSetThings(new Bottle(static_cast<BottleType>(std::rand() % 3), 10));

	//reset keys
	for (auto i : player->backpack) {
		if (i->getItemType() == ItemType::key)
			dynamic_cast<Key*>(i)->used = false;
	}

	for (auto& charac : characters)
	{
		if (charac->attitude == attitudes::agressive || charac->beAttacked == true)
		{ //attack randomly
		}
		if (charac->bePoisoned > 0)
		{
			charac->health -= charac->bePoisoned * 2; //health loss of ponison
			if (charac->health <= 0)
				charac->died();
			else
				charac->bePoisoned--;
		}
		if (auto monsterChar = dynamic_cast<Monster*>(charac); monsterChar != nullptr)
		{
			//if it is monster, rest controlled rounds decrease
			if (monsterChar->beControlled > 0) {
				monsterChar->beControlled--;
				if (monsterChar->beControlled == 0) {
					player->follower.erase(std::find(player->follower.begin(), player->follower.end(), monsterChar));
					addInfo((monsterChar->name + " is out of your control.").c_str());
					globalMap->randomSetThings(monsterChar);
				}
			}
		}
	}
	auto sameRoomCharacters = globalMap->getSameRoomObjectList(); //all objects in same room
	for (auto& obj : sameRoomCharacters)
	{
		if (obj->getType() == ObjectType::creature)
		{
			if (auto creatureObj = dynamic_cast<Creature*>(obj); creatureObj->name != player->name)
			{
				//temp function to calculate distance
				auto calDistance = [](std::pair<int, int> postionA, std::pair<int, int> postionB) {
					auto sumX = powf(float(postionA.first - postionB.first), float(2.0));
					auto sumY = powf(float(postionA.second - postionB.second), float(2.0));
					return sqrt(sumX + sumY);
				};
				for (int i = 4; i > 0; i--)
				{
					auto oldPostion = creatureObj->position;
					if (!creatureObj->move(static_cast<MoveDirection>(std::rand() % 4)))
						i++; //move failed, try again;
					else {
						globalMap->moveObject(oldPostion.first, oldPostion.second, creatureObj->position.first, creatureObj->position.second);
					}

				}
				if (creatureObj->attitude == attitudes::agressive || creatureObj->beAttacked == true)
				{ //attack randomly
					if (calDistance(creatureObj->position, player->position) == 1)
					{ //can attack player
						creatureObj->attack(*player);
					}
				}
				//NPC pick item
				if (auto mankindObj = dynamic_cast<Mankind*>(creatureObj); mankindObj != nullptr && mankindObj->backpack.size() <= 4)
				{
					if (globalMap->getLocationType(creatureObj->position.first + 1, creatureObj->position.second) == ObjectType::item)
					{
						mankindObj->pick(globalMap->getLocationItem(creatureObj->position.first + 1, creatureObj->position.second));
						globalMap->eraseGameObjectAt(creatureObj->position.first + 1, creatureObj->position.second, false);
					}
					else if (globalMap->getLocationType(creatureObj->position.first - 1, creatureObj->position.second) == ObjectType::item)
					{
						mankindObj->pick(globalMap->getLocationItem(creatureObj->position.first - 1, creatureObj->position.second));
						globalMap->eraseGameObjectAt(creatureObj->position.first - 1, creatureObj->position.second, false);
					}
					else if (globalMap->getLocationType(creatureObj->position.first, creatureObj->position.second + 1) == ObjectType::item)
					{
						mankindObj->pick(globalMap->getLocationItem(creatureObj->position.first, creatureObj->position.second + 1));
						globalMap->eraseGameObjectAt(creatureObj->position.first, creatureObj->position.second + 1, false);
					}
					else if (globalMap->getLocationType(creatureObj->position.first, creatureObj->position.second - 1) == ObjectType::item)
					{
						mankindObj->pick(globalMap->getLocationItem(creatureObj->position.first, creatureObj->position.second - 1));
						globalMap->eraseGameObjectAt(creatureObj->position.first, creatureObj->position.second - 1, false);
					}
				}
			}
		}
	}
}

void Game::investigate() {
	const int directionTable[4][2] = { {1,0},{-1,0},{0,-1},{0,1} };
	auto playerPosition = player->position;
	auto x = playerPosition.first, y = playerPosition.second;
	auto flag = false;
	for (auto direction : directionTable) {
		if (!globalMap->isOutOfRange(x + direction[0], y + direction[1])) {
			if (auto objectType = globalMap->getLocationType(x + direction[0], y + direction[1]); objectType != ObjectType::nothing) {
				gameObject* toInvestigate;
				if (objectType == ObjectType::creature)
					toInvestigate = globalMap->getLocationCreature(x + direction[0], y + direction[1]);
				else
					toInvestigate = globalMap->getLocationItem(x + direction[0], y + direction[1]);
				addInfo("Investigate result:");
				addInfo(toInvestigate->getInfo().c_str());
			}
		}
	}
}

bool Game::conjoure()
{
	const int directionTable[4][2] = { {0,1}, {0,-1}, {-1,0},{1,0} };
	auto x = player->position.first, y = player->position.second;
	bool directions[4] = { false };
	for (auto direction : directionTable) {
		if (!globalMap->isOutOfRange(x + direction[0], y + direction[1])) {
			if (auto objectType = globalMap->getLocationType(x + direction[0], y + direction[1]); objectType != ObjectType::nothing) {
				if (objectType == ObjectType::creature) {
					if (auto monsterObj = dynamic_cast<Monster*>(globalMap->getLocationCreature(x + direction[0], y + direction[1])); monsterObj != nullptr) {
						if (direction[0] == 0)
							directions[direction[1] == 1 ? 0 : 1] = true;
						else
							directions[direction[0] == -1 ? 2 : 3] = true;
					}
				}
			}
		}
	}
	auto conjoureDirec = scrollDirections(directions);
	if (conjoureDirec == Directions::win) return false;
	auto status = player->conjure(dynamic_cast<Monster*>(globalMap->getLocationCreature(x + directionTable[static_cast<int>(conjoureDirec)][0], y + directionTable[static_cast<int>(conjoureDirec)][1])));
	if (status)
		globalMap->eraseGameObjectAt(x + directionTable[static_cast<int>(conjoureDirec)][0], y + directionTable[static_cast<int>(conjoureDirec)][1], false);
	return true;
}

void Game::pickup()
{
	const int directionTable[4][2] = { {0,1}, {0,-1}, {-1,0},{1,0} };
	auto x = player->position.first, y = player->position.second;
	bool directions[4] = { false };
	for (auto direction : directionTable) {
		if (!globalMap->isOutOfRange(x + direction[0], y + direction[1])) {
			if (auto objectType = globalMap->getLocationType(x + direction[0], y + direction[1]); objectType != ObjectType::nothing) {
				if (objectType == ObjectType::item) {
					if (direction[0] == 0)
						directions[direction[1] == 1 ? 0 : 1] = true;
					else
						directions[direction[0] == -1 ? 2 : 3] = true;
				}
			}
		}
	}
	auto itemDirec = scrollDirections(directions);
	if (itemDirec == Directions::win) return;
	auto status = player->pick(
		dynamic_cast<Item*>(\
			globalMap->getLocationItem(\
				x + directionTable[static_cast<int>(itemDirec)][0], \
				y + directionTable[static_cast<int>(itemDirec)][1]  \
			) 														\
		)	 														\
	);
	if (status)
		globalMap->eraseGameObjectAt(x + directionTable[static_cast<int>(itemDirec)][0], y + directionTable[static_cast<int>(itemDirec)][1], false);
}

void Game::gotoWin()
{
	wined = true;
	addInfo("**************************");
	addInfo("Congratulations on your success in finding the win key. You win the game.");
	addInfo("**************************");
	addInfo("Press any key to end the game.");
	getch();
}

bool Game::attack()
{
	const int directionTable[4][2] = { {0,1}, {0,-1}, {-1,0},{1,0} };
	auto x = player->position.first, y = player->position.second;
	bool directions[4] = { false };
	for (auto direction : directionTable) {
		if (!globalMap->isOutOfRange(x + direction[0], y + direction[1])) {
			if (auto objectType = globalMap->getLocationType(x + direction[0], y + direction[1]); objectType != ObjectType::nothing) {
				if (objectType == ObjectType::creature) {
					if (direction[0] == 0)
						directions[direction[1] == 1 ? 0 : 1] = true;
					else
						directions[direction[0] == -1 ? 2 : 3] = true;
				}
			}
		}
	}
	auto attackDirec = scrollDirections(directions);
	if (attackDirec == Directions::win) return false;
	player->attack(*globalMap->getLocationCreature(x + directionTable[static_cast<int>(attackDirec)][0], y + directionTable[static_cast<int>(attackDirec)][1]));
	return true;
}