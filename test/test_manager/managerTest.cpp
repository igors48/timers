#include <unity.h>

#include "core/app/manager.cpp"

App firstApp;
bool firstAppActive;
unsigned int nextWakeUpPeriodFirst;

App secondApp;
bool secondAppActive;
unsigned int nextWakeUpPeriodSecond;

App thirdApp;
bool thirdAppActive;
unsigned int nextWakeUpPeriodThird;

bool tilerNotifiedAboutApp;
bool tilerRenderApp;
Tiler tilerStub;
Manager manager;
const unsigned char APPS_COUNT = 3;
void *apps[APPS_COUNT];

void activateFirst()
{
    firstAppActive = true;
}

void activateSecond()
{
    secondAppActive = true;
}

void activateThird()
{
    thirdAppActive = true;
}

void deactivateFirst()
{
    firstAppActive = false;
}

void deactivateSecond()
{
    secondAppActive = false;
}

void deactivateThird()
{
    thirdAppActive = false;
}

unsigned int getNextWakeUpPeriodFirst()
{
    return nextWakeUpPeriodFirst;
}

unsigned int getNextWakeUpPeriodSecond()
{
    return nextWakeUpPeriodSecond;
}

unsigned int getNextWakeUpPeriodThird()
{
    return nextWakeUpPeriodThird;
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

    nextWakeUpPeriodFirst = NW_DONT_CARE;
    nextWakeUpPeriodSecond = NW_DONT_CARE;
    nextWakeUpPeriodThird = NW_DONT_CARE;
    
    firstApp = {
        .activate = activateFirst,
        .deactivate = deactivateFirst,
        .getNextWakeUpPeriod = getNextWakeUpPeriodFirst,
    };

    secondApp = {
        .activate = activateSecond,
        .deactivate = deactivateSecond,
        .getNextWakeUpPeriod = getNextWakeUpPeriodSecond,
    };

    thirdApp = {
        .activate = activateThird,
        .deactivate = deactivateThird,
        .getNextWakeUpPeriod = getNextWakeUpPeriodThird,
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

void whenAllAppsReturnsDontCareAboutNextWakeUp()
{
    unsigned int nextWakeUp = manager.getNextWakeUpPeriod();
    
    TEST_ASSERT_EQUAL_UINT32(NW_DONT_CARE, nextWakeUp); // THEN manager also dont care
}

void whenOneAppReturnsNoSleep()
{
    nextWakeUpPeriodFirst = 1;
    nextWakeUpPeriodSecond = NW_NO_SLEEP;

    unsigned int nextWakeUp = manager.getNextWakeUpPeriod();
    
    TEST_ASSERT_EQUAL_UINT32(NW_NO_SLEEP, nextWakeUp); // THEN manager also no sleep
}

void whenManagerAsksAboutNextWakeUpPeriod()
{
    nextWakeUpPeriodFirst = 1; // minimal(shortest) period
    nextWakeUpPeriodSecond = 4;

    unsigned int nextWakeUp = manager.getNextWakeUpPeriod();
    
    TEST_ASSERT_EQUAL_UINT32(1, nextWakeUp); // THEN minimal(shortest) period returns
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(whenActivatedByIndex);
    RUN_TEST(whenActivated);
    RUN_TEST(whenSwitchToNext);
    RUN_TEST(whenSwitchToPrev);
    RUN_TEST(whenAllAppsReturnsDontCareAboutNextWakeUp);
    RUN_TEST(whenOneAppReturnsNoSleep);
    RUN_TEST(whenManagerAsksAboutNextWakeUpPeriod);
    UNITY_END();
}