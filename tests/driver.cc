/**
 * Copyright (c) 2019 The cxkernel Authors. All rights reserved.
 * Use of this source code is governed by a MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
 * 
 * @file   driver.cc
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * @date   Created on December 27 2019, 5:20 PM
 */

#define __MODULE__ "UNITY"

#include <stdio.h>

#include "unitytest.h"
#include "unity/unity.h"

extern "C" void setUp()
{
    OS_LOG("setup()\n");
}

extern "C" void tearDown()
{
    OS_LOG("tearDown()\n");
}

extern "C" void TestModuleXXXX()
{
    TEST_ASSERT_EQUAL_HEX8(80, 90);
}

int UnitTest::main()
{
    UNITY_BEGIN();
    RUN_TEST(TestModuleXXXX);
    return UNITY_END();
}
