/*
 * factoryTest.h
 * simple test of factory constructors using demo example
 *
 *
 *
 *
 *
 */
#pragma once

#include "include/factory.h"
#include "include/abstractFactory.h"
#include "gtest/gtest.h"
#include "demoTypes.h"

#include <iostream>
#include <utility>
#include <vector>
#include <cassert>

namespace {

  template<typename T>
  struct bridge
  {
    template<typename... ARGS>
    T* Create(ARGS... args)
    {
      return new T(args...);
    }
  };
  
  struct visitor
  {
    bool isAccepted = false;
    virtual bool accept(std::string name, std::any) = 0;

    template<typename T> static T foo;
  };

  template<typename T> T visitor::foo;

  template<typename T>
  struct aVisitor: visitor
  {
      
    bool accept(std::string name, std::any a)
    {
      auto glambda = [](auto... param) { return new T(param...); };
      try{
	visitor::foo<T> = std::any_cast<T>(a);
	isAccepted = true;
      }catch(const std::bad_any_cast& e)
	{
	  isAccepted = false;
	}
      return isAccepted;
    }
  };
  
  
  TEST(SimpleTest, Factory) {

    auto demoFactory = new FactoryPattern::Factory();

    auto type0 = demoFactory->Create<Demo::Type0>();
    EXPECT_NE(type0, nullptr);

    auto type1 = demoFactory->Create<Demo::Type1>(5);
    EXPECT_NE( type1, nullptr);

    auto type2 = demoFactory->Create<Demo::Type2>();
    EXPECT_NE( type2, nullptr);

    auto type3 = demoFactory->Create<Demo::Type3>(5, 7.2, 7UL);
    EXPECT_NE( type3, nullptr);

    // this currently does not work
    auto type3Named = demoFactory->Create<>("Demo::Type3", 5, 7.2, 7UL);
    EXPECT_EQ( type3Named, nullptr);

    if (type0)
      type0->doSomeThing();
    if (type1)
      type1->doSomeThing();
    if (type2)
      type2->doSomeThing();
    if (type3)
      type3->doSomeThing();


    // Now try abstract factory
    // build polymorphic class by string type name
    auto abstractFactory = new FactoryPattern::AbstractFactory<Demo::DemoIF>();
    std::string name = "Demo::Type4";
    auto type4 = abstractFactory->Create(name);
    EXPECT_NE(type4, nullptr);
    name = "Demo::Type5";
    auto type5 = abstractFactory->Create(name);
    EXPECT_NE( type5, nullptr);
    if (type4)
      type4->doSomeThing();
    if (type5)
      type5->doSomeThing();


  }


}  // namespace
