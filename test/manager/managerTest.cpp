#include <unity.h>

#include "core/app/manager.cpp"

App firstApp;
bool firstAppActive;
App secondApp;
bool secondAppActive;
App thirdApp;
bool thirdAppActive;
bool tilerNotifiedAboutApp;
bool tilerRenderApp;
Tiler tilerStub;
Manager manager;
const unsigned char APPS_COUNT = 3;
void *apps[APPS_COUNT];

void activateFirst(App *app)
{
    firstAppActive = true;
}

void activateSecond(App *app)
{
    secondAppActive = true;
}

void activateThird(App *app)
{
    thirdAppActive = true;
}

void deactivateFirst(App *app)
{
    firstAppActive = false;
}

void deactivateSecond(App *app)
{
    secondAppActive = false;
}

void deactivateThird(App *app)
{
    thirdAppActive = false;
}

void setApp(App *app)
{
    tilerNotifiedAboutApp = true;
}

void renderApp(bool forced)
{
    tilerRenderApp = true;
}

void setUp(void)
{
    firstAppActive = false;
    secondAppActive = false;
    thirdAppActive = false;

    firstApp = {
        .activate = activateFirst,
        .deactivate = deactivateFirst,
    };

    secondApp = {
        .activate = activateSecond,
        .deactivate = deactivateSecond,
    };

    thirdApp = {
        .activate = activateThird,
        .deactivate = deactivateThird,
    };

    tilerNotifiedAboutApp = false;
    tilerRenderApp = false;

    tilerStub = {
        .setApp = setApp,
        .renderApp = renderApp,
    };

    apps[0] = &firstApp;
    apps[1] = &secondApp;
    apps[2] = &thirdApp;

    manager = createManager(APPS_COUNT, apps, &tilerStub);
}

void whenActivatedByIndex()
{
    manager.activateApp(0);
    TEST_ASSERT_TRUE(firstAppActive); // THEN only first app is active
    TEST_ASSERT_FALSE(secondAppActive);
    TEST_ASSERT_FALSE(thirdAppActive);

    manager.activateApp(1);
    TEST_ASSERT_FALSE(firstAppActive); 
    TEST_ASSERT_TRUE(secondAppActive); // THEN only second app is active
    TEST_ASSERT_FALSE(thirdAppActive);

    manager.activateApp(2);
    TEST_ASSERT_FALSE(firstAppActive); 
    TEST_ASSERT_FALSE(secondAppActive); 
    TEST_ASSERT_TRUE(thirdAppActive); // THEN only third app is active
}

void whenActivated()
{
    manager.activateApp(1);
    TEST_ASSERT_TRUE(tilerNotifiedAboutApp); // THEN tiler is notified
    TEST_ASSERT_TRUE(tilerRenderApp); // THEN tiler renders app
}

void whenSwitchToNext()
{
    manager.activateApp(0);

    manager.switchApp(true);
    TEST_ASSERT_FALSE(firstAppActive); 
    TEST_ASSERT_TRUE(secondAppActive); // THEN next app is active
    TEST_ASSERT_FALSE(thirdAppActive);

    manager.switchApp(true);
    TEST_ASSERT_FALSE(firstAppActive); 
    TEST_ASSERT_FALSE(secondAppActive);
    TEST_ASSERT_TRUE(thirdAppActive); // THEN next app is active

    manager.switchApp(true);
    TEST_ASSERT_TRUE(firstAppActive); // THEN next app is active 
    TEST_ASSERT_FALSE(secondAppActive);
    TEST_ASSERT_FALSE(thirdAppActive);
}

void whenSwitchToPrev()
{
    manager.activateApp(0);

    manager.switchApp(false);
    TEST_ASSERT_FALSE(firstAppActive); 
    TEST_ASSERT_FALSE(secondAppActive);
    TEST_ASSERT_TRUE(thirdAppActive); // THEN prev app is active

    manager.switchApp(false);
    TEST_ASSERT_FALSE(firstAppActive); 
    TEST_ASSERT_TRUE(secondAppActive); // THEN next app is active
    TEST_ASSERT_FALSE(thirdAppActive);

    manager.switchApp(false);
    TEST_ASSERT_TRUE(firstAppActive); // THEN next app is active 
    TEST_ASSERT_FALSE(secondAppActive);
    TEST_ASSERT_FALSE(thirdAppActive);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenActivatedByIndex);
    RUN_TEST(whenActivated);
    RUN_TEST(whenSwitchToNext);
    RUN_TEST(whenSwitchToPrev);
    UNITY_END();
}