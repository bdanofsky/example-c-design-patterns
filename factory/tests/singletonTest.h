/*
 * singletonTest.h
 * simple test of Singleton
 *
 */
#pragma once

#include "include/singleton.h"
#include "gtest/gtest.h"

namespace {

  class Type0: public Factory::Singleton<Type0>
  {

  private:
    friend Factory::Singleton<Type0>;
    Type0()
    {
      std::cout << "Type0 was created by Singleton" << std::endl;
    }
  };
  
  class TypeN: public Factory::Singleton<TypeN>
  {

  private:
    friend Factory::Singleton<TypeN>;
    TypeN(int x, float y, std::shared_ptr<Type0> pT0)
    {
      std::cout << "TypeN was created by Singleton" << std::endl;
    }
  };
  
  TEST(SimpleTest, Singleton) {

    auto pType0 = Type0::Create();
    EXPECT_NE(pType0, nullptr);

    auto pTn = TypeN::Create(1, 5.0, pType0);
    EXPECT_NE(pTn, nullptr);
    
  }


}  // namespace
