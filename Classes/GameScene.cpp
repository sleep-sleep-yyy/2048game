#include "GameScene.h"
#include "GameDefine.h"
#include "SimpleAudioEngine.h"
#include <ctime>
USING_NS_CC;
using namespace CocosDenshion;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}
bool GameScene::init()
{
	if (!Layer::init())
		return false;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//��ӱ���
	auto gameBkGround2 = LayerColor::create(Color4B(191, 191, 191, 255));
	this->addChild(gameBkGround2);//�Ƶ�
	auto gameBkGround1 = LayerColor::create(Color4B(119, 136, 153, 255), TileMapWidth, TileMapWidth);
	this->addChild(gameBkGround1);//�����ӵ�

	//��ӷ���
	auto title = LabelTTF::create("Score:", "Mouldy.ttf", 70);
	title->setColor(Color3B(119, 136, 153));
	title->setPosition(Vec2(visibleSize.width * 15 / 18, visibleSize.height * 3 / 4));
	this->addChild(title);
	score = 0;
	scoreLabel = LabelTTF::create("0", "Mouldy.ttf", 60);
	scoreLabel->setColor(Color3B(119, 136, 153));
	scoreLabel->setPosition(Point(visibleSize.width * 15 / 18 - 10, visibleSize.height * 8 / 12 - 20));
	this->addChild(scoreLabel);
	bestScore = UserDefault::getInstance()->getIntegerForKey("BEST");

	Vector<MenuItem*> MenuItems;
	auto newlabel1 = Label::createWithTTF("New Game", "fonts/Mouldy.ttf", 30);
	menuItemNew1 = MenuItemLabel::create(newlabel1, CC_CALLBACK_1(GameScene::restart, this));
	MenuItems.pushBack(menuItemNew1);
	menuItemNew1->setPosition(Vec2(visibleSize.width * 15 / 18, visibleSize.height / 8));
	menuItemNew1->setColor(Color3B(108, 123, 139));

	auto label2 = Label::createWithTTF("Exit", "fonts/Mouldy.ttf", 24);
	menuItemNew2 = MenuItemLabel::create(label2, [](Ref* sender) {Director::getInstance()->end(); });
	MenuItems.pushBack(menuItemNew2);
	menuItemNew2->setPosition(Vec2(visibleSize.width * 17 / 18 + 25, visibleSize.height - 25));
	menuItemNew2->setColor(Color3B(108, 123, 139));

	auto label3 = Label::createWithTTF("Ranking List", "fonts/Mouldy.ttf", 30);
	menuItemNew3 = MenuItemLabel::create(label3, CC_CALLBACK_1(GameScene::showRanking, this));
	MenuItems.pushBack(menuItemNew3);
	menuItemNew3->setPosition(Vec2(visibleSize.width * 15 / 18, visibleSize.height / 8 + 60));
	menuItemNew3->setColor(Color3B(108, 123, 139));

	auto label4 = Label::createWithTTF("Pause", "fonts/Mouldy.ttf", 30);
	menuItemNew4 = MenuItemLabel::create(label4, CC_CALLBACK_1(GameScene::pauseMenu, this));
	MenuItems.pushBack(menuItemNew4);
	menuItemNew4->setPosition(Vec2(visibleSize.width * 15 / 18, visibleSize.height / 8 + 120));
	menuItemNew4->setColor(Color3B(108, 123, 139));



	auto menu = Menu::createWithArray(MenuItems);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//��ʼ����Ƭ
	createCardArr(visibleSize);
	//�������������Ƭ
	randomCreateCard();
	randomCreateCard();

	//��Ӵ�������
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}
void GameScene::restart(Ref* sender)
{
	//ת�������¿�ʼ��Ϸ
	Director::getInstance()->replaceScene(TransitionFade::create(0.7f, GameScene::createScene()));
}
bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
	gettimeofday(&tv, NULL); //��¼��ǰʱ��
	startPt = touch->getLocation(); //���濪ʼ������
	return true;
}
void GameScene::onTouchEnded(Touch* touch, Event* event)
{
	timeval tv_end;
	gettimeofday(&tv_end, NULL);
	auto endPt = touch->getLocation();  //��ô���������
	offsetX = endPt.x - startPt.x;  //��������ƶ�ƫ��
	offsetY = endPt.y - startPt.y;
	bool isTouch = false;
	if (abs(offsetX) > abs(offsetY)) //�Ǿ������ҷ����
	{
		if (offsetX < -5)
			isTouch = moveLeft();
		else if (offsetX > 5)
			isTouch = moveRight();
	}
	else//��֮�����·���
	{
		if (offsetY > 5)
			isTouch = moveUp();
		else if (offsetY < -5)
			isTouch = moveDown();
	}
	if (isTouch)//�������
	{
		scoreLabel->setString(String::createWithFormat("%d", score)->getCString());
		checkGameWin();
		randomCreateCard();
		checkGameOver();
		//Ҫ�����ɿ����ٿ���û����
	}
}
void GameScene::createCardArr(Size size)
{
	//������Ƭ����
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//��������½�������Ϊ������Ϊ��
			CardSprite* card = CardSprite::createCard(0, i * TileWidth + (i + 1) * TileBorderWidth,
				j * TileWidth + (j + 1) * TileBorderWidth);
			this->addChild(card);
			cardArr[i][j] = card;  //�浽��Ƭ����
		}
	}
}
void GameScene::randomCreateCard()
{
	int row = CCRANDOM_0_1() * 4;
	int col = CCRANDOM_0_1() * 4;
	if (cardArr[row][col]->getNumber() > 0)//��������֣���ݹ����
		randomCreateCard();
	else
	{
		cardArr[row][col]->setNumber(CCRANDOM_0_1() * 10 < 1 ? 4 : 2); //��10%�ļ�������4
		//����Ч��
		auto action = Sequence::createWithTwoActions(ScaleTo::create(0, 0.3), ScaleTo::create(0.3f, 1));  //��0.15���ڴ�С���ŵ���
		cardArr[row][col]->getCardLayer()->runAction(action);
	}
}
//���󻬶��߼�
bool GameScene::moveLeft()//Ҫ����ʼ������Щ��Ƭ
{
	bool moved = false;//�Ƿ��ƶ�
	bool cleared = false;//�Ƿ�����
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++) {   //�ڲ���N^2���Ӷȵ�����ð������
			for (int x1 = x + 1; x1 < 4; x1++)
			{
				if (cardArr[x1][y]->getNumber() > 0) {  //x�ұߵĿ�Ƭ�����ֲŶ���
					if (cardArr[x][y]->getNumber() == 0) {
						cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
						cardArr[x1][y]->setNumber(0);
						x--;  //��ɨ��һ�飬ȷ�����н����ȷ
						moved = true;
					}
					else if (cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber()) {
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x1][y]->setNumber(0);
						//���ֺϲ�����
						auto merge = Sequence::create(ScaleTo::create(0.1f, 1.2f), ScaleTo::create(0.1f, 1.0f), NULL);
						cardArr[x][y]->getCardLayer()->runAction(merge);

						score += cardArr[x][y]->getNumber();
						moved = true;
						cleared = true;
					}
					break;   //�˴�break��ֹ����������2��bug
				}
			}
		}
	}
	//���ŵ÷�����
	if (moved && !cleared)
		SimpleAudioEngine::getInstance()->playEffect("Move.wav");
	if (cleared)
		SimpleAudioEngine::getInstance()->playEffect("MoveClear.mp3");
	return moved;
}
bool GameScene::moveRight()
{
	bool moved = false;
	bool cleared = false;
	for (int y = 0; y < 4; y++) {
		for (int x = 3; x >= 0; x--) {
			for (int x1 = x - 1; x1 >= 0; x1--) {
				if (cardArr[x1][y]->getNumber() > 0) {
					if (cardArr[x][y]->getNumber() == 0) {
						cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
						cardArr[x1][y]->setNumber(0);
						x++;
						moved = true;
					}
					else if (cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber()) {
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x1][y]->setNumber(0);

						auto merge = Sequence::create(ScaleTo::create(0.1f, 1.2f), ScaleTo::create(0.1f, 1.0f), NULL);
						cardArr[x][y]->getCardLayer()->runAction(merge);
						score += cardArr[x][y]->getNumber();
						moved = true;
						cleared = true;
					}
					break;
				}
			}
		}
	}
	//���ŵ÷�����
	if (moved && !cleared)
		SimpleAudioEngine::getInstance()->playEffect("Move.wav");
	if (cleared)
		SimpleAudioEngine::getInstance()->playEffect("MoveClear.mp3");
	return moved;
}
bool GameScene::moveUp()
{
	bool moved = false;
	bool cleared = false;
	for (int x = 0; x < 4; x++) {
		for (int y = 3; y >= 0; y--) {
			for (int y1 = y - 1; y1 >= 0; y1--) {
				if (cardArr[x][y1]->getNumber() > 0) {
					if (cardArr[x][y]->getNumber() == 0) {
						cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
						cardArr[x][y1]->setNumber(0);
						y++;
						moved = true;
					}
					else if (cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber()) {
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x][y1]->setNumber(0);
						auto merge = Sequence::create(ScaleTo::create(0.1f, 1.2f), ScaleTo::create(0.1f, 1.0f), NULL);
						cardArr[x][y]->getCardLayer()->runAction(merge);
						score += cardArr[x][y]->getNumber();
						moved = true;
						cleared = true;
					}
					break;
				}
			}
		}
	}
	//���ŵ÷�����
	if (moved && !cleared)
		SimpleAudioEngine::getInstance()->playEffect("Move.wav");
	if (cleared)
		SimpleAudioEngine::getInstance()->playEffect("MoveClear.mp3");
	return moved;
}
bool GameScene::moveDown() {
	bool moved = false;
	bool cleared = false;
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			for (int y1 = y + 1; y1 < 4; y1++) {
				if (cardArr[x][y1]->getNumber() > 0) {
					if (cardArr[x][y]->getNumber() == 0) {
						cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
						cardArr[x][y1]->setNumber(0);
						y--;
						moved = true;
					}
					else if (cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber()) {
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x][y1]->setNumber(0);

						auto merge = Sequence::create(ScaleTo::create(0.1f, 1.2f), ScaleTo::create(0.1f, 1.0f), NULL);
						cardArr[x][y]->getCardLayer()->runAction(merge);

						score += cardArr[x][y]->getNumber();
						moved = true;
						cleared = true;
					}
					break;
				}
			}
		}
	}
	//���ŵ÷�����
	if (moved && !cleared)
		SimpleAudioEngine::getInstance()->playEffect("Move.wav");
	if (cleared)
		SimpleAudioEngine::getInstance()->playEffect("MoveClear.mp3");
	return moved;
}
void GameScene::getNoSort()
{
	//ȡ����������
	std::string s_tmp;
	IntString new_result;
	int k = 1;
	for (; k <= 9; k++)
	{
		s_tmp = "Key" + std::to_string(k);
		std::string result = UserDefault::getInstance()->getStringForKey(s_tmp.c_str(), "");
		if (result == "")
		{
			break;
		}
		else
		{
			UserDefault::getInstance()->setStringForKey(s_tmp.c_str(), result);
			// ���Ҷ��ŵ�λ��
			size_t commaPos = result.find(',');
			// ��ȡ����ǰ��Ĳ��ֲ�ת��Ϊ����
			std::string numberString = result.substr(0, commaPos);
			new_result.number = std::stoi(numberString);
			// ��ȡ���ź���Ĳ�����Ϊ�µ��ַ���
			new_result.newString = result.substr(commaPos + 1);
			rankingList.push_back(new_result);
		}
	}
}
void GameScene::createRanking(const int& score)
{
	Score tmp;
	time_t now = time(0);
	tm* localTime = localtime(&now);
	tmp.score = score;
	tmp.year = localTime->tm_year + 1900;
	tmp.month = localTime->tm_mon + 1;
	tmp.day = localTime->tm_mday;
	tmp.hour = localTime->tm_hour;
	tmp.minute = localTime->tm_min;
	std::string arrayString;
	std::string s_tmp;
	arrayString += std::to_string(tmp.score) + "," + std::to_string(tmp.year) + "." + std::to_string(tmp.month) + "." + std::to_string(tmp.day)
		+ " " + std::to_string(tmp.hour) + ":" + std::to_string(tmp.minute);
	// �洢�����ַ����� UserDefault
	int order = 1;
	bool saved = false;//�Ƿ�ɹ��洢
	for (; order <= 9; order++)
	{
		s_tmp = "Key" + std::to_string(order);
		std::string result = UserDefault::getInstance()->getStringForKey(s_tmp.c_str(), "");
		if (result == "")
		{
			UserDefault::getInstance()->setStringForKey(s_tmp.c_str(), arrayString);
			break;
		}
		else
		{
			UserDefault::getInstance()->setStringForKey(s_tmp.c_str(), result);
			saved = true;
		}
	}
	if (saved == false)
	{
		getNoSort();
		unsigned num = rankingList.size();
		int min = score, min_index = 0, tmp;
		for (int i = 0; i < num; i++)
		{
			if (rankingList[i].number < min)
			{
				min_index = i;
				min = rankingList[i].number;
			}
		}
		if (min != score)//���ڵĻ��϶��Ų���ǰ�ţ��Ͳ��ù���
		{
			s_tmp = "Key" + std::to_string(min_index + 1);
			UserDefault::getInstance()->setStringForKey(s_tmp.c_str(), arrayString);
		}
	}
}
void GameScene::showMenu(std::string result)
{
	littleMenu = LittleMenu::create(Color4B(0, 0, 0, 100));
	this->addChild(littleMenu);
	auto menuSize = littleMenu->getContentSize();
	//��ӱ���
	auto menuTitle = LabelTTF::create(result, "Arial", 30);
	menuTitle->setPosition(menuSize.width / 2, menuSize.height / 2 + 50);
	littleMenu->addChild(menuTitle);
	//��ӵ�ǰ����
	const std::string scoreLabel = String::createWithFormat("current: %d", score)->getCString();
	createRanking(score);
	auto menuscoreLabel = LabelTTF::create(scoreLabel, "Arial", 20);
	menuscoreLabel->setPosition(menuSize.width / 2, menuSize.height / 2);
	littleMenu->addChild(menuscoreLabel);
	//�����÷���
	bestScore = UserDefault::getInstance()->getIntegerForKey("BEST");
	if (score > bestScore)
	{
		bestScore = score;
		UserDefault::getInstance()->setIntegerForKey("BEST", bestScore);
	}
	auto menuBestscoreLabel = LabelTTF::create(String::createWithFormat("best: %d", bestScore)->getCString(), "Arial", 20);
	menuBestscoreLabel->setPosition(menuSize.width / 2, menuSize.height / 2 - 30);
	littleMenu->addChild(menuBestscoreLabel);
	MenuItemFont::setFontName("Arial");
	MenuItemFont::setFontSize(25);
	auto menuItemRestart = MenuItemFont::create("Restart", this, menu_selector(GameScene::restart));
	menuItemRestart->setColor(Color3B(255, 255, 0));
	auto menu = Menu::create(menuItemRestart, NULL);
	littleMenu->addChild(menu);
	menu->setPosition(Point(menuSize.width / 2, menuSize.height / 2 - 80));
}
void GameScene::showRanking(Ref* sender)
{
	littleMenu = LittleMenu::create(Color4B(0, 0, 0, 100));
	this->addChild(littleMenu);
	menuItemNew3->setEnabled(false);
	menuItemNew4->setEnabled(false);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//��ӱ���
	auto gameBkGround2 = LayerColor::create(Color4B(119, 136, 153, 100));
	this->addChild(gameBkGround2);//����
	//���˳��
	auto order = LabelTTF::create("order", "Mouldy.ttf", 40);
	order->setColor(Color3B(191, 191, 191));
	order->setPosition(Vec2(visibleSize.width * 1 / 4 - 100, visibleSize.height - 45));
	this->addChild(order);
	//��ӷ�����ʱ��
	auto score = LabelTTF::create("score", "Mouldy.ttf", 40);
	score->setColor(Color3B(191, 191, 191));
	score->setPosition(Vec2(visibleSize.width * 2 / 5 - 100, visibleSize.height - 45));
	this->addChild(score);
	//���ʱ��
	auto time = LabelTTF::create("time", "Mouldy.ttf", 40);
	time->setColor(Color3B(191, 191, 191));
	time->setPosition(Vec2(visibleSize.width * 3 / 5 - 50, visibleSize.height - 45));
	this->addChild(time);
	//��Ӹ��
	for (int i = 0; i < 9; i++)
	{
		auto gameBkGround1 = LayerColor::create(Color4B(191, 191, 191, 255), visibleSize.width - 100, visibleSize.height / 10 - 8);
		gameBkGround1->setPosition(Vec2(51, visibleSize.height / 10 * i));
		this->addChild(gameBkGround1);
	}
	getNoSort();
	std::sort(rankingList.begin(), rankingList.end(), [](const GameScene::IntString& a, const GameScene::IntString& b) {
		return a.number > b.number; });//����һ��
	const unsigned num = rankingList.size();
	for (int i = 0; i < num; i++)
	{
		auto label1 = LabelTTF::create(String::createWithFormat("%d", i + 1)->getCString(), "Mouldy.ttf", 30);
		label1->setPosition(Vec2(visibleSize.width * 1 / 4 - 100, visibleSize.height / 10 * (9 - i) - 35));
		label1->setColor(Color3B(108, 123, 139));
		this->addChild(label1);
		auto label2 = LabelTTF::create(String::createWithFormat("%d", rankingList[i].number)->getCString(), "Mouldy.ttf", 30);
		label2->setPosition(Vec2(visibleSize.width * 2 / 4 - 200, visibleSize.height / 10 * (9 - i) - 35));
		label2->setColor(Color3B(108, 123, 139));
		this->addChild(label2);
		auto label3 = LabelTTF::create(rankingList[i].newString, "Mouldy.ttf", 30);
		label3->setPosition(Vec2(visibleSize.width * 3 / 4 - 175, visibleSize.height / 10 * (9 - i) - 35));
		label3->setColor(Color3B(108, 123, 139));
		this->addChild(label3);

	}
}
void GameScene::resume(Ref* sender)
{
	Director::getInstance()->resume();
	this->removeChild(littleMenu, true);
	menuItemNew1->setEnabled(true);
	menuItemNew2->setEnabled(true);
	menuItemNew3->setEnabled(true);
	menuItemNew4->setEnabled(true);
}
void GameScene::pauseMenu(Ref* sender)
{
	menuItemNew1->setEnabled(false);
	menuItemNew2->setEnabled(false);
	menuItemNew3->setEnabled(false);
	menuItemNew4->setEnabled(false);

	littleMenu = LittleMenu::create(Color4B(0, 0, 0, 100));
	this->addChild(littleMenu);
	auto menuTitle = LabelTTF::create("PAUSED", "Arial", 30);
	auto menuSize = littleMenu->getContentSize();
	menuTitle->setPosition(menuSize.width / 2, menuSize.height / 2 + 50);
	littleMenu->addChild(menuTitle);
	Director::getInstance()->pause();
	auto menuItemRestart = MenuItemFont::create("Click Here To Resume", CC_CALLBACK_1(GameScene::resume, this));
	menuItemRestart->setColor(Color3B(245, 245, 245));
	auto menu = Menu::create(menuItemRestart, NULL);
	littleMenu->addChild(menu);
	menu->setPosition(Point(menuSize.width / 2, menuSize.height / 2));


}
void GameScene::checkGameWin()
{
	bool isWin = false;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (2048 == cardArr[i][j]->getNumber())
				isWin = true;
	if (isWin)
	{
		//������Ч
		SimpleAudioEngine::getInstance()->playEffect("gamewin.mp3");
		showMenu("You Win");
	}
}

void GameScene::checkGameOver()
{
	bool isGameOver = true;
	//�����������Ϸ����
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if ((cardArr[i][j]->getNumber() == 0) ||
				(i > 0 && cardArr[i][j]->getNumber() == cardArr[i - 1][j]->getNumber()) ||
				(i < 3 && cardArr[i][j]->getNumber() == cardArr[i + 1][j]->getNumber()) ||
				(j > 0 && cardArr[i][j]->getNumber() == cardArr[i][j - 1]->getNumber()) ||
				(j < 3 && cardArr[i][j]->getNumber() == cardArr[i][j + 1]->getNumber()))
			{
				isGameOver = false;
			}
		}
	}
	//������Ϸ����
	if (isGameOver)
	{
		SimpleAudioEngine::getInstance()->playEffect("gameover.mp3");
		showMenu("Game Over");
	}
}

