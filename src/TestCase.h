#include "jmw.h"
#include "TestScene.h"
#include <gmock\gmock.h>
#include <gtest\gtest.h>

#define GMOCK_ENABLE 0


class SceneTest : public ::testing::Test {
protected:
	virtual void SetUp() {

	}

	// virtual void TearDown() {}

	TestScene scene;
};


TEST_F(SceneTest, isLive) {

	ASSERT_TRUE(false == scene.m_IsPop);
}
