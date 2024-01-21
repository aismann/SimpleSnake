/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).
 Copyright (c) 2022-present @aismann; Peter Eismann, Germany; dreifrankensoft


 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "MainScene.h"

USING_NS_AX;

struct Snake
{
	int x, y;
} s[snakeBodies + StartBodies];

struct Fruit
{
	int x, y;
} f;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Content/' in front of filenames in MainScene.cpp\n");
}

inline void setNodeIgnoreDesignScale(ax::Node* node)
{
	AXASSERT(node, "node pointer cannot be null.");

	Size actualFrameSize = Director::getInstance()->getGLView()->getFrameSize();
	Size actualWinSize = Director::getInstance()->getWinSizeInPixels();
	node->setScaleX(actualWinSize.width / actualFrameSize.width);
	node->setScaleY(actualWinSize.height / actualFrameSize.height);
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = _director->getVisibleSize();
	auto origin = _director->getVisibleOrigin();
	auto safeArea = _director->getSafeAreaRect();
	auto safeOrigin = safeArea.origin;

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
		AX_CALLBACK_1(MainScene::menuCloseCallback, this));

	if (closeItem == nullptr || closeItem->getContentSize().width <= 0 || closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = safeOrigin.x + safeArea.size.width - closeItem->getContentSize().width / 2;
		float y = safeOrigin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// Some templates (uncomment what you  need)
	auto touchListener = EventListenerTouchAllAtOnce::create();
	touchListener->onTouchesBegan = AX_CALLBACK_2(MainScene::onTouchesBegan, this);
	touchListener->onTouchesMoved = AX_CALLBACK_2(MainScene::onTouchesMoved, this);
	touchListener->onTouchesEnded = AX_CALLBACK_2(MainScene::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//auto mouseListener           = EventListenerMouse::create();
	//mouseListener->onMouseMove   = AX_CALLBACK_1(MainScene::onMouseMove, this);
	//mouseListener->onMouseUp     = AX_CALLBACK_1(MainScene::onMouseUp, this);
	//mouseListener->onMouseDown   = AX_CALLBACK_1(MainScene::onMouseDown, this);
	//mouseListener->onMouseScroll = AX_CALLBACK_1(MainScene::onMouseScroll, this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = AX_CALLBACK_2(MainScene::onKeyPressed, this);
	//keyboardListener->onKeyReleased = AX_CALLBACK_2(MainScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithFixedPriority(keyboardListener, 11);



	// add a label shows "Hello World"
	// create and initialize a label

	auto label = Label::createWithTTF("Simple Snake", "fonts/arial.ttf", 24);
	if (label == nullptr)
	{
		problemLoading("'fonts/arial.ttf'");
	}
	else
	{
		// position the label on the center of the screen
		label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - label->getContentSize().height));

		// add the label as a child to this layer
		this->addChild(label, 1);
	}

	auto label2 = Label::createWithTTF("Eat as fast as you can!", "fonts/arial.ttf", 24);
	if (label2 == nullptr)
	{
		problemLoading("'fonts/arial.ttf'");
	}
	else
	{
		// position the label on the center of the screen
		label2->setPosition(
			Vec2(origin.x + visibleSize.width / 2,
				origin.y + visibleSize.height - label2->getContentSize().height * 3 - visibleSize.height / 2));

		// add the label as a child to this layer
		this->addChild(label2, 1);
	}

	// add "HelloWorld" splash screen"
	auto sprite = Sprite::create("logo.png"sv);
	//setNodeIgnoreDesignScale(sprite);
	if (sprite == nullptr)
	{
		problemLoading("'logo.png'");
	}
	else
	{
		// position the sprite on the center of the screen
		sprite->setPosition(offset + Vec2(origin.x + visibleSize.width / 2, 100));
		sprite->setScale(0.1);
		// add the sprite as a child to this layer
		this->addChild(sprite, 1);


		auto drawNode = DrawNode::create();
		drawNode->setPosition(Vec2(0, 0));
		addChild(drawNode);

		drawNode->drawRect(safeArea.origin + Vec2(1, 1), safeArea.origin + safeArea.size, Color4F::BLUE);

		auto powerdBy = Label::createWithTTF("POWERD BY", "fonts/arial.ttf", 24);
		powerdBy->setPosition(Vec2(origin.x + visibleSize.width / 2, 100 + powerdBy->getContentSize().height * 3));

		// add the label as a child to this layer
		this->addChild(powerdBy, 1);
	}
	srand(time(0));

	mydraw = DrawNode::create();
	addChild(mydraw, 10);

	background = Sprite::create("Background.png");
	background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	background->setScale(1.1);
	addChild(background, 0);

	f.x = 10;
	f.y = 10;
	s[0].x = 20;
	s[0].y = 20;

	char buffer[1024];

	for (int i = 0; i < snakeBodies + 1; i++)
	{
		myScore[i] = 0.0;
	    sprintf(buffer, "%02i: %04.3f", i + 1, myScore[i]);
		myScoreLabel[i] = Label::createWithTTF(std::string(buffer), "fonts/arial.ttf", 20);
		myScoreLabel[i]->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		myScoreLabel[i]->setPosition(Vec2(10, origin.y + visibleSize.height - 10 - i * 20));
		this->addChild(myScoreLabel[i], 1);
	}
	myScoreLabel[snakeBodies]->setString("");//"Eat as fast as you can!");

	// scheduleUpdate() is required to ensure update(float) is called on every loop
	scheduleUpdate();

	return true;
}


void MainScene::onTouchesBegan(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
	for (auto&& t : touches)
	{
		AXLOG("onTouchesBegan detected, X:%f  Y:%f", t->getLocation().x, t->getLocation().y);
	}
}

void MainScene::onTouchesMoved(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
	for (auto&& t : touches)
	{
		AXLOG("onTouchesMoved detected, X:%f  Y:%f", t->getLocation().x, t->getLocation().y);
	}
}

void MainScene::onTouchesEnded(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
	for (auto&& t : touches)
	{
		AXLOG("onTouchesEnded detected, X:%f  Y:%f", t->getLocation().x, t->getLocation().y);
	}
}

void MainScene::onMouseDown(Event* event)
{
	EventMouse* e = static_cast<EventMouse*>(event);
	AXLOG("onMouseDown detected, Key: %d", static_cast<int>(e->getMouseButton()));
}

void MainScene::onMouseUp(Event* event)
{
	EventMouse* e = static_cast<EventMouse*>(event);
	AXLOG("onMouseUp detected, Key: %d", static_cast<int>(e->getMouseButton()));
}

void MainScene::onMouseMove(Event* event)
{
	EventMouse* e = static_cast<EventMouse*>(event);
	AXLOG("onMouseMove detected, X:%f  Y:%f", e->getCursorX(), e->getCursorY());
}

void MainScene::onMouseScroll(Event* event)
{
	EventMouse* e = static_cast<EventMouse*>(event);
	AXLOG("onMouseScroll detected, X:%f  Y:%f", e->getScrollX(), e->getScrollY());
}

void MainScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event)
{
	AXLOG("onKeyPressed, keycode: %d", static_cast<int>(code));

	switch (code)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		if (dir != 2)
			dir = 1;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		if (dir != 1)
			dir = 2;
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		if (dir != 3)
			dir = 0;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		if (dir != 0)
			dir = 3;
		break;
	case EventKeyboard::KeyCode::KEY_1:
		Director::getInstance()->setStatsAnchor(AnchorPreset::BOTTOM_LEFT);
		break;
	case EventKeyboard::KeyCode::KEY_4:
		Director::getInstance()->setStatsAnchor(AnchorPreset::CENTER_LEFT);
		break;
	case EventKeyboard::KeyCode::KEY_7:
		Director::getInstance()->setStatsAnchor(AnchorPreset::TOP_LEFT);
		break;
	case EventKeyboard::KeyCode::KEY_8:
		Director::getInstance()->setStatsAnchor(AnchorPreset::TOP_CENTER);
		break;
	case EventKeyboard::KeyCode::KEY_9:
		Director::getInstance()->setStatsAnchor(AnchorPreset::TOP_RIGHT);
		break;
	case EventKeyboard::KeyCode::KEY_6:
		Director::getInstance()->setStatsAnchor(AnchorPreset::CENTER_RIGHT);
		break;
	case EventKeyboard::KeyCode::KEY_3:
		Director::getInstance()->setStatsAnchor(AnchorPreset::BOTTOM_RIGHT);
		break;
	case EventKeyboard::KeyCode::KEY_2:
		Director::getInstance()->setStatsAnchor(AnchorPreset::BOTTOM_CENTER);
		break;
	case EventKeyboard::KeyCode::KEY_5:
		Director::getInstance()->setStatsAnchor(AnchorPreset::CENTER);
		break;
	default:
		break;
	}
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event)
{
	AXLOG("onKeyReleased, keycode: %d", static_cast<int>(code));
}

void MainScene::update(float delta)
{
	switch (_gameState)
	{
	case GameState::init:
	{
		_gameState = GameState::update;
		break;
	}

	case GameState::update:
	{
		/////////////////////////////
		// Add your codes below...like....
		// 
		// UpdateJoyStick();
		// UpdatePlayer();
		// UpdatePhysics();
		// ...
		static float runTime = 0;
		static bool finish = false;
		runTime += delta;
		endLevelTime += delta;



		if (finish)
		{
			float myScale = runTime + 1.1;
			if (myScale > 3.0)
				myScale = 3.0;
			background->setScale(myScale);
			return;
		}

		if (runTime > level)
		{
			for (int i = num; i > 0; --i)
			{
				s[i] = s[i - 1];
			}
			switch (dir)
			{
			case 0:
				s[0].y++;
				if (s[0].y >= M) s[0].y = 0;
				break;
			case 1:
				s[0].x--;
				if (s[0].x < 0) s[0].x = N - 1;
				break;
			case 2:
				s[0].x++;
				if (s[0].x >= N) s[0].x = 0;
				break;
			case 3:
				s[0].y--;
				if (s[0].y < 0) s[0].y = M - 1;
				break;
			default:
				break;
			}

			if ((s[0].x == f.x) && (s[0].y == f.y))
			{
				bool posOk;
				do
				{
					posOk = true;
					f.x = rand() % N;
					f.y = rand() % M;

					for (int i = 0; i < num; i++)
					{
						if (f.x == s[i].x && f.y == s[i].y)
						{
							posOk = false;
							break;
						}
					}
				} while (!posOk);

				// next level
				num++;
				if (num >= (snakeBodies + StartBodies - 1))
				{
					myScoreLabel[snakeBodies]->setString("!!! FINISH !!!");
					finish = true;
					drawAll(finish);
				}
				level -= 0.01;
				startLevelTime = 0.0;
				endLevelTime = startLevelTime;
			}
			runTime = 0;
		}

		if (!finish)
		{
			drawAll(finish);
		}
		break;
	}

	case GameState::pause:
	{
		/////////////////////////////
		// Add your codes below...like....
		//
		// anyPauseStuff()

		break;
	}

	case GameState::menu1:
	{    /////////////////////////////
		// Add your codes below...like....
		// 
		// UpdateMenu1();
		break;
	}

	case GameState::menu2:
	{    /////////////////////////////
		// Add your codes below...like....
		// 
		// UpdateMenu2();
		break;
	}

	case GameState::end:
	{    /////////////////////////////
		// Add your codes below...like....
		// 
		// CleanUpMyCrap();
		menuCloseCallback(this);
		break;
	}

	} //switch
}

void MainScene::menuCloseCallback(Ref* sender)
{
	// Close the axmol game scene and quit the application
	_director->end();

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use
	 * _director->end() as given above,instead trigger a custom event created in RootViewController.mm
	 * as below*/

	 // EventCustom customEndEvent("game_scene_close_event");
	 //_eventDispatcher->dispatchEvent(&customEndEvent);
}

void MainScene::drawAll(bool finish)
{
	// score
	char buffer[124];
	myScore[num - StartBodies] = endLevelTime;
	sprintf(buffer, "%02i: %04.3f", num - StartBodies + 1, myScore[num - StartBodies]);
	myScoreLabel[num - StartBodies]->setString(buffer);


	mydraw->clear();
	// draw snake body
	for (int i = 1; i < num; i++)
	{
		mydraw->drawDot(Vec2(size / 2 + s[i].x * size, size / 2 + s[i].y * size), size / 2, Color4B::BLUE);
	}
	//draw snake head
	mydraw->drawDot(Vec2(size / 2 + s[0].x * size, size / 2 + s[0].y * size), size / 2, Color4B::MAGENTA);

	if (!finish)
	{
		// draw food
		mydraw->drawDot(Vec2(size / 2 + f.x * size, size / 2 + f.y * size), size / 2, Color4B::GREEN);
	}
}
