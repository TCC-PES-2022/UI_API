#include <gtest/gtest.h>
#include "..\src\UI_API.h"

TEST(example1, tt) {
	int res;
	res = quadrado(1);
	ASSERT_NEAR(res, 1, 1.0e-11);
}