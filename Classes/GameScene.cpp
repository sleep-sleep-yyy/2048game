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
	const Size visibleSize = Director::getInstance()->getVisibleSize();
	//添加背景
	const auto gameBkGround2 = LayerColor::create(Color4B(191, 191, 191, 255));
	this->addChild(gameBkGround2);//黄底
	const auto gameBkGround1 = LayerColor::create(Color4B(119, 136, 153, 255), TileMapWidth, TileMapWidth);
	this->addChild(gameBkGround1);//蓝格子底

	//添加分数
	const auto title = Label::createWithTTF("Score:", "fonts/calibri.ttf", 70);
	title->setColor(Color3B(119, 136, 153));
	title->setPosition(Vec2(visibleSize.width * 15 / 18, visibleSize.height * 3 / 4));
	this->addChild(title);
	score = 0;
	scoreLabel = Label::createWithTTF("0", "fonts/calibri.ttf",60);
	scoreLabel->setColor(Color3B(119, 136, 153));
	scoreLabel->setPosition(Point(visibleSize.width * 15 / 18-10, visibleSize.height * 8 / 12-20));
	this->addChild(scoreLabel);
	bestScore = UserDefault::getInstance()->getIntegerForKey("BEST");

	Vector<MenuItem*> MenuItems;
	const auto label1 = Label::createWithTTF("New Game", "fonts/Mouldy.ttf", 30);
	menuItemNew1 = MenuItemLabel::create(label1, CC_CALLBACK_1(GameScene::restart, this));
	MenuItems.pushBack(menuItemNew1);
	menuItemNew1->setPosition(Vec2(visibleSize.width * 15 / 18, visibleSize.height / 8-60));
	menuItemNew1->setColor(Color3B(108,123,139));

	const auto label2 = Label::createWithTTF("Exit", "fonts/Mouldy.ttf", 24);
	menuItemNew2 =MenuItemLabel::create(label2,[](Ref* sender) {Director::getInstance()->end();});
	MenuItems.pushBack(menuItemNew2);
	menuItemNew2->setPosition(Vec2(visibleSize.width * 17 / 18+25, visibleSize.height-25));
	menuItemNew2->setColor(Color3B(108, 123, 139));

	const auto label3 = Label::createWithTTF("Ranking List", "fonts/Mouldy.ttf", 30);
	menuItemNew3 = MenuItemLabel::create(label3, CC_CALLBACK_1(GameScene::showRanking, this));
	MenuItems.pushBack(menuItemNew3);
	menuItemNew3->setPosition(Vec2(visibleSize.width * 15 / 18, visibleSize.height / 8));
	menuItemNew3->setColor(Color3B(108, 123, 139));

	const auto label4 = Label::createWithTTF("Pause", "fonts/Mouldy.ttf", 30);
	menuItemNew4 = MenuItemLabel::create(label4, CC_CALLBACK_1(GameScene::pauseMenu, this));
	MenuItems.pushBack(menuItemNew4);
	menuItemNew4->setPosition(Vec2(visibleSize.width * 15 / 18, visibleSize.height / 8 + 60));
	menuItemNew4->setColor(Color3B(108, 123, 139));

	mode = UserDefault::getInstance()->getIntegerForKey("mode");
	const int otherMode = (mode == 4) ? 5 : 4;
	const auto label5 = Label::createWithTTF(String::createWithFormat("%d*%d", otherMode, otherMode)->getCString(), "fonts/Mouldy.ttf", 30);
	menuItemNew5 = MenuItemLabel::create(label5, CC_CALLBACK_1(GameScene::modeChange, this));
	MenuItems.pushBack(menuItemNew5);
	menuItemNew5->setPosition(Vec2(visibleSize.width * 15 / 18, visibleSize.height / 8 + 120));
	menuItemNew5->setColor(Color3B(108, 123, 139));

	const auto menu = Menu::createWithArray(MenuItems);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	
	isSoundOn= UserDefault::getInstance()->getBoolForKey("sound");//获取声音变量
	//声音按钮
	if(isSoundOn==true)
	    soundButton = cocos2d::Sprite::create("soundOn.png");
	else
		soundButton = cocos2d::Sprite::create("soundOff.png");
	soundButton->setPosition(Vec2(visibleSize.width * 15 / 18, visibleSize.height / 8 + 180));
	this->addChild(soundButton);

	//初始化卡片
	createCardArr();
	//随机生成两个卡片
	randomCreateCard();
	randomCreateCard();

	//添加触摸监听
	const auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);


	//添加键盘监听
	keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);


	return true;
}
void GameScene::restart(Ref* sender)
{
	//转场，重新开始游戏
	Director::getInstance()->replaceScene(TransitionFade::create(0.7f, GameScene::createScene()));
}
bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
	gettimeofday(&tv, NULL); //记录当前时间
	startPt = touch->getLocation(); //保存开始触摸点
	return true;
}
void GameScene::onTouchEnded(Touch* touch, Event* event)
{
	timeval tv_end;
	gettimeofday(&tv_end, NULL);
	const auto endPt = touch->getLocation();  //获得触摸结束点
	offsetX = endPt.x - startPt.x;  //计算鼠标移动偏移
	offsetY = endPt.y - startPt.y;
	bool isTouch = false;
	if (abs(offsetX) > abs(offsetY)) //那就是左右方向的
	{
		if (offsetX < -5)
			isTouch = moveLeft();
		else if (offsetX > 5)
			isTouch = moveRight();
	}
	else//反之是上下方向
	{
		if (offsetY > 5)
			isTouch = moveUp();
		else if (offsetY < -5)
			isTouch = moveDown();
	}
	if (isTouch)//如果滑动
	{
		scoreLabel->setString(String::createWithFormat("%d", score)->getCString());
		checkGameWin();
		randomCreateCard();
		checkGameOver();
		//要先生成卡牌再看有没有输
	}
	if (soundButton->getBoundingBox().containsPoint(endPt))//如果点到了声音控制
		toggleSound();
}
void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	bool isTouch = false;
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		isTouch=moveLeft();
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		isTouch=moveRight();
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		isTouch = moveUp();
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		isTouch = moveDown();
		break;
	default:
		break;
	}
	if (isTouch)//如果滑动
	{
		scoreLabel->setString(String::createWithFormat("%d", score)->getCString());
		checkGameWin();
		randomCreateCard();
		checkGameOver();
		//要先生成卡牌再看有没有输
	}
}
void GameScene::createCardArr()
{
	//创建卡片矩阵
	for (int i = 0; i < mode; i++)
	{
		for (int j = 0; j < mode; j++)
		{
			CardSprite* card;
			//坐标从左下角算起，右为正，上为正
			if (mode == 4)
			{
				card = CardSprite::createCard(0, i * TileWidth + (i + 1) * TileBorderWidth,
					j * TileWidth + (j + 1) * TileBorderWidth,mode);
			}
			else
			{
				card = CardSprite::createCard(0, i * TileWidth_2 + (i + 1) * TileBorderWidth_2,
					j * TileWidth_2 + (j + 1) * TileBorderWidth_2, mode);
			}
			this->addChild(card);
			cardArr[i][j] = card;  //存到卡片矩阵
		}
	}
}
void GameScene::randomCreateCard()
{
	const int row = CCRANDOM_0_1() * mode;
	const int col = CCRANDOM_0_1() * mode;
	if (cardArr[row][col]->getNumber() > 0)//如果有数字，则递归调用
		randomCreateCard();
	else
	{
		cardArr[row][col]->setNumber(CCRANDOM_0_1() * 10 < 1 ? 4 : 2); //有10%的几率生成4
		//动画效果
		const auto action = Sequence::createWithTwoActions(ScaleTo::create(0.0f, 0.3f), ScaleTo::create(0.3f, 1));  //在0.15秒内从小缩放到大
		cardArr[row][col]->getCardLayer()->runAction(action); 
	}
}
//向左滑动逻辑
bool GameScene::moveLeft()//要从左开始处理这些卡片
{
	bool moved = false;//是否移动
	bool cleared = false;//是否消除
	for (int y = 0; y < mode; y++)
	{
		for (int x = 0; x < mode; x++){   //内部的N^2复杂度的类似冒泡排序
			for (int x1 = x + 1; x1 < mode; x1++)
			{
				if (cardArr[x1][y]->getNumber() > 0){  //x右边的卡片有数字才动作
					if (cardArr[x][y]->getNumber() == 0){
						cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
						cardArr[x1][y]->setNumber(0);
						x--;  //再扫描一遍，确保所有结果正确
						moved = true;
					}
					else if (cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber()){
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x1][y]->setNumber(0);
						//数字合并动画
						auto merge = Sequence::create(ScaleTo::create(0.1f, 1.2f), ScaleTo::create(0.1f, 1.0f), NULL);
						cardArr[x][y]->getCardLayer()->runAction(merge);

						score += cardArr[x][y]->getNumber();
						moved = true;
						cleared = true;
					}
					break;   //此处break防止出现连续乘2的bug
				}
			}
		}
	}
	//播放得分声音
	if (moved&&!cleared && isSoundOn)
		SimpleAudioEngine::getInstance()->playEffect("Move.wav");
	if(cleared && isSoundOn)
		SimpleAudioEngine::getInstance()->playEffect("MoveClear.mp3");
	return moved;
}
bool GameScene::moveRight()
{
	bool moved = false;
	bool cleared = false;
	for (int y = 0; y < mode; y++){
		for (int x = mode-1; x >= 0; x--){
			for (int x1 = x - 1; x1 >= 0; x1--){
				if (cardArr[x1][y]->getNumber() > 0) {
					if (cardArr[x][y]->getNumber() == 0){
						cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
						cardArr[x1][y]->setNumber(0);
						x++;
						moved = true;
					}
					else if (cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber()){
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
	//播放得分声音
	if (moved && !cleared && isSoundOn)
		SimpleAudioEngine::getInstance()->playEffect("Move.wav");
	if (cleared && isSoundOn)
		SimpleAudioEngine::getInstance()->playEffect("MoveClear.mp3");
	return moved;
}
bool GameScene::moveUp()
{
	bool moved = false;
	bool cleared = false;
	for (int x = 0; x < mode; x++) {
		for (int y = mode-1; y >= 0; y--){
			for (int y1 = y - 1; y1 >= 0; y1--){
				if (cardArr[x][y1]->getNumber() > 0) {
					if (cardArr[x][y]->getNumber() == 0){
						cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
						cardArr[x][y1]->setNumber(0);
						y++;
						moved = true;
					}
					else if (cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber()){
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
	//播放得分声音
	if (moved && !cleared && isSoundOn)
		SimpleAudioEngine::getInstance()->playEffect("Move.wav");
	if (cleared && isSoundOn)
		SimpleAudioEngine::getInstance()->playEffect("MoveClear.mp3");
	return moved;
}
bool GameScene::moveDown() {
	bool moved = false;
	bool cleared = false;
	for (int x = 0; x < mode; x++){
		for (int y = 0; y < mode; y++){
			for (int y1 = y + 1; y1 < mode; y1++){
				if (cardArr[x][y1]->getNumber() > 0) {
					if (cardArr[x][y]->getNumber() == 0){
						cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
						cardArr[x][y1]->setNumber(0);
						y--;
						moved = true;
					}
					else if (cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber()){
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
	//播放得分声音
	if (moved && !cleared&&isSoundOn)
		SimpleAudioEngine::getInstance()->playEffect("Move.wav");
	if (cleared && isSoundOn)
		SimpleAudioEngine::getInstance()->playEffect("MoveClear.mp3");
	return moved;
}

void GameScene::getNoSort()
{
	//取出数据排序
	std::string s_tmp;
	IntString new_result;
	int k = 1;
	for (; k <=9; k++)
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
			// 查找逗号的位置
			const size_t commaPos = result.find(',');
			// 提取逗号前面的部分并转换为整数
			std::string numberString = result.substr(0, commaPos);
			new_result.number = std::stoi(numberString);
			// 提取逗号后面的部分作为新的字符串
			new_result.newString = result.substr(commaPos + 1);
			rankingList.push_back(new_result);
		}
	}
}
void GameScene::createRanking(const int& score)
{
	Score tmp;
	const time_t now = time(0);
	const tm* localTime = localtime(&now);
	tmp.score = score;
	tmp.year= localTime->tm_year + 1900;
	tmp.month = localTime->tm_mon + 1;
	tmp.day = localTime->tm_mday;
	tmp.hour = localTime->tm_hour;
	tmp.minute = localTime->tm_min;
	std::string arrayString;
	std::string s_tmp;
	arrayString += std::to_string(tmp.score) + "," + std::to_string(tmp.year) + "." + std::to_string(tmp.month) + "." + std::to_string(tmp.day)
		+ " " + std::to_string(tmp.hour) + ":" + std::to_string(tmp.minute)+"("+ std::to_string(mode)+"*"+ std::to_string(mode)+")";
	// 存储数组字符串到 UserDefault
	int order= 1;
	bool saved = false;//是否成功存储
	for (; order <=9; order++)
	{
		s_tmp = "Key" + std::to_string(order);
		std::string result = UserDefault::getInstance()->getStringForKey(s_tmp.c_str(),"");
		if (result == "")//如果有位置没被占据，那么就把它填进去，示为已保存
		{
			UserDefault::getInstance()->setStringForKey(s_tmp.c_str(), arrayString);
			saved = true;
			break;
		}
		else
		{
			UserDefault::getInstance()->setStringForKey(s_tmp.c_str(), result);
		}
	}
	if (saved == false)
	{
		getNoSort();
		const unsigned num = rankingList.size();
		int min=score,min_index=0;
		for (unsigned int i=0;i<num;i++)
		{
			if (rankingList[i].number < min)
			{
				min_index = i;
				min = rankingList[i].number;
			}
		}
		if (min != score)//等于的话肯定排不上前九，就不用管了
		{
			s_tmp = "Key" + std::to_string(min_index+1);
			UserDefault::getInstance()->setStringForKey(s_tmp.c_str(), arrayString);
		}
	}
}
void GameScene::showMenu(std::string result)
{
	_eventDispatcher->removeEventListenersForType(EventListener::Type::KEYBOARD);
	littleMenu = LittleMenu::create(Color4B(0, 0, 0, 100));
	this->addChild(littleMenu);
	const auto menuSize = littleMenu->getContentSize();
	//添加标题
	const TTFConfig config("fonts/Arial.ttf", 30);//字体
	const auto menuTitle = Label::createWithTTF(config,result);
	menuTitle->setPosition(menuSize.width / 2, menuSize.height / 2 + 50);
	littleMenu->addChild(menuTitle);
	//添加当前分数
	const std::string scoreLabel = String::createWithFormat("current: %d", score)->getCString();
	createRanking(score);
	const TTFConfig config1("fonts/Arial.ttf", 20);//字体
	const auto menuscoreLabel = Label::createWithTTF(config1,scoreLabel);
	menuscoreLabel->setPosition(menuSize.width / 2, menuSize.height / 2);
	littleMenu->addChild(menuscoreLabel);
	//添加最好分数
	bestScore = UserDefault::getInstance()->getIntegerForKey("BEST");
	if (score > bestScore)
	{
		bestScore = score;
		UserDefault::getInstance()->setIntegerForKey("BEST", bestScore);
	}
	const auto menuBestscoreLabel = Label::createWithTTF(config1,String::createWithFormat("best: %d", bestScore)->getCString());
	menuBestscoreLabel->setPosition(menuSize.width / 2, menuSize.height / 2 - 30);
	littleMenu->addChild(menuBestscoreLabel);
	MenuItemFont::setFontName("Arial");
	MenuItemFont::setFontSize(25);
	const auto label= Label::createWithTTF("Restart", "fonts/Mouldy.ttf", 30);
	const auto menuItemRestart = MenuItemLabel::create(label, CC_CALLBACK_1(GameScene::restart, this));
	menuItemRestart->setColor(Color3B(255, 255, 0));
	const auto menu = Menu::create(menuItemRestart, NULL);
	littleMenu->addChild(menu);
	menu->setPosition(Point(menuSize.width / 2, menuSize.height / 2 - 80));
}
void GameScene::showRanking(Ref* sender)
{
	keyboardListener->setEnabled(false);
	littleMenu = LittleMenu::create(Color4B(0, 0, 0, 100));
	this->addChild(littleMenu);
	menuItemNew3->setEnabled(false);
	menuItemNew4->setEnabled(false);
	const Size visibleSize = Director::getInstance()->getVisibleSize();
	//添加背景
	const auto gameBkGround2 = LayerColor::create(Color4B(119, 136, 153, 100));
	this->addChild(gameBkGround2);//蓝底
	//添加顺序
	const auto order = Label::createWithTTF("order", "fonts/calibri.ttf", 40);
	order->setColor(Color3B(191, 191, 191));
	order->setPosition(Vec2(visibleSize.width * 1 / 4-100, visibleSize.height - 45));
	this->addChild(order);
	//添加分数和时间
	const auto score = Label::createWithTTF("score", "fonts/calibri.ttf", 40);
	score->setColor(Color3B(191, 191, 191));
	score->setPosition(Vec2(visibleSize.width * 2 / 5-100, visibleSize.height - 45));
	this->addChild(score);
	//添加时间和模式
	const auto time = Label::createWithTTF("time and mode", "fonts/calibri.ttf", 40);
	time->setColor(Color3B(191, 191, 191));
	time->setPosition(Vec2(visibleSize.width * 3 / 5-50, visibleSize.height - 45));
	this->addChild(time);
	//添加格格
	for (int i = 0; i < 9; i++)
	{
		const auto gameBkGround1 = LayerColor::create(Color4B(191, 191, 191, 255), visibleSize.width - 100, visibleSize.height / 10 - 8);
		gameBkGround1->setPosition(Vec2(51, visibleSize.height / 10 * i));
		this->addChild(gameBkGround1);
	}
	//得到vector,排序，展示
	getNoSort();
	std::sort(rankingList.begin(), rankingList.end(), [](const GameScene::IntString& a, const GameScene::IntString& b) noexcept {
		return a.number > b.number; });//排序一下
	const unsigned num = rankingList.size();
	for (unsigned int i = 0; i < num; i++)
	{
		const auto label1 = Label::createWithTTF(String::createWithFormat("%d", i + 1)->getCString(), "fonts/calibri.ttf", 30);
		label1->setPosition(Vec2(visibleSize.width * 1 / 4-100, visibleSize.height / 10 * (9 - i)-35));
		label1->setColor(Color3B(108, 123, 139));
		this->addChild(label1);

		const auto label2 = Label::createWithTTF(String::createWithFormat("%d", rankingList[i].number)->getCString(), "fonts/calibri.ttf", 30);
		label2->setPosition(Vec2(visibleSize.width * 2 / 4-200, visibleSize.height / 10 * (9 - i)-35));
		label2->setColor(Color3B(108, 123, 139));
		this->addChild(label2);

		const auto label3= Label::createWithTTF(rankingList[i].newString, "fonts/calibri.ttf", 30);
		label3->setPosition(Vec2(visibleSize.width * 3 / 4-175, visibleSize.height / 10 * (9 - i)-35));
		label3->setColor(Color3B(108, 123, 139));
		this->addChild(label3);
	}
}
void GameScene::resume(Ref* sender)
{
	keyboardListener->setEnabled(true);
	Director::getInstance()->resume();
	this->removeChild(littleMenu, true);
	menuItemNew1->setEnabled(true);
	menuItemNew2->setEnabled(true);
	menuItemNew3->setEnabled(true);
	menuItemNew4->setEnabled(true);
	menuItemNew5->setEnabled(true);
}
void GameScene::pauseMenu(Ref* sender)
{
	keyboardListener->setEnabled(false);
	//_eventDispatcher->removeEventListenersForType(EventListener::Type::KEYBOARD);
	menuItemNew1->setEnabled(false);
	menuItemNew2->setEnabled(false);
	menuItemNew3->setEnabled(false);
	menuItemNew4->setEnabled(false);
	menuItemNew5->setEnabled(false);
	littleMenu = LittleMenu::create(Color4B(0, 0, 0, 100));
	this->addChild(littleMenu);
	auto  menuTitle = Label::createWithTTF("PAUSED","fonts/calibri.ttf", 30);
	const auto menuSize = littleMenu->getContentSize();
	menuTitle->setPosition(menuSize.width / 2, menuSize.height / 2 + 50);
	littleMenu->addChild(menuTitle);
	Director::getInstance()->pause();
	const auto menuItemRestart = MenuItemFont::create("Click Here To Resume", CC_CALLBACK_1(GameScene::resume, this));
	menuItemRestart->setColor(Color3B(245,245, 245));
	const auto menu = Menu::create(menuItemRestart, NULL);
	littleMenu->addChild(menu);
	menu->setPosition(Point(menuSize.width / 2, menuSize.height / 2));
}
void GameScene::modeChange(Ref* sender) 
{
	mode = (mode == 4) ? 5 : 4;
	UserDefault::getInstance()->setIntegerForKey("mode", mode);
	Director::getInstance()->replaceScene(TransitionFade::create(0.7f, GameScene::createScene()));
	//失败尝试：虽然重新初始化mode会被保留，但当前状态的其他量比如cardarr也会被保留，可以尝试在这里把他们重新初始化
	/*
	this->removeAllChildrenWithCleanup(true);
	//重新初始化场景
	if (!this->init()) {
      //初始化失败处理
		cocos2d::log("Scene initialization failed!");
	}
	*/
}
void GameScene::checkGameWin()
{
	bool isWin = false;
	for (int i = 0; i < mode; i++)
		for (int j = 0; j < mode; j++)
			if (2048 == cardArr[i][j]->getNumber())
				isWin = true;
	if (isWin && isSoundOn)
	{
		//播放音效
		SimpleAudioEngine::getInstance()->playEffect("gamewin.mp3");
		showMenu("You Win");
	}
}
void GameScene::checkGameOver()
{
	bool isGameOver = true;
	//以下情况则游戏继续
	for (int j = 0; j < mode; j++)
	{
		for (int i = 0; i < mode; i++)
		{//单凡有一个空格或有两个连一起的就没有输
			if ((cardArr[i][j]->getNumber() == 0) ||
				(i > 0 && cardArr[i][j]->getNumber() == cardArr[i - 1][j]->getNumber()) ||
				(i < mode-1 && cardArr[i][j]->getNumber() == cardArr[i + 1][j]->getNumber()) ||
				(j > 0 && cardArr[i][j]->getNumber() == cardArr[i][j - 1]->getNumber()) ||
				(j < mode-1 && cardArr[i][j]->getNumber() == cardArr[i][j + 1]->getNumber()))
			{
				isGameOver = false;
				break;
			}
		}
	}
	//否则游戏结束
	if (isGameOver && isSoundOn)
	{			
		SimpleAudioEngine::getInstance()->playEffect("gameover.mp3");
		showMenu("Game Over");
	}
}
void GameScene::toggleSound()
{
	if (isSoundOn)
	{
		// 关闭声音
		soundButton->setTexture("soundOff.png");
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		UserDefault::getInstance()->setBoolForKey("sound", false);//存储当前声音变量，应对restart
	}
	else
	{
		// 打开声音
		soundButton->setTexture("soundOn.png");
		SimpleAudioEngine::getInstance()->playBackgroundMusic("BackGround.mp3", true);
		//SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		UserDefault::getInstance()->setBoolForKey("sound", true);//存储当前声音变量，应对restart
	}
	isSoundOn = !isSoundOn;
}

