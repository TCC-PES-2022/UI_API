#include "pch.h"
TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(CasoDeTeste_Init_UI_API, CasoDeTeste_Init_UI_API) {
	st_ui_image val;
	st_ui_aut val2;
	EXPECT_EQ(1, iniciar_UI_interface(&val, &val2));
	EXPECT_TRUE(true);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}