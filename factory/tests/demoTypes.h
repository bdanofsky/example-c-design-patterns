/*
 * demoTypes.h
 * simple demo type that has its constructor stored in a vector
 * based on string.
 * Perhaps an ENUM class would be better
 *
 *
 *
 */
#pragma once

#include <typeinfo>
#include "include/factory.h"

namespace Demo
{
  using namespace FactoryPattern; // being lazy with namespaces
  
  template<typename T>
  using Registrator = Factory::Registrator<T>;
  
  template<typename Interface, typename T>
  using AbstractRegistrator = typename AbstractFactory<Interface>::template Registrator<T>;
  
  struct DemoIF
  {
    virtual void doSomeThing() = 0;
    virtual ~DemoIF() = default;
  };

  // polymorphic class with no constructor args
  class Type0: public DemoIF, public Registrator<Type0>
  {
  private:
    template<typename T> friend struct Factory::Registrator;
    Type0() = default;

  public:
    void doSomeThing()
    {
      std::cout << "hello "<< typeid(this).name() << " world" << std::endl;
    }
  };

  // polymorphic class with one constructor args
  class Type1: public DemoIF, public Registrator<Type1>
  {
  private:
    template<typename T> friend struct Factory::Registrator;
    Type1(int foo)
    {
      std::cout << "hey Type1 constructor called with one argument " << foo << std::endl;
    }

  public:
    void doSomeThing()
    {
      std::cout << "hello "<< typeid(this).name() << " world" << std::endl;
    }
  };


  // class that is not polymorphic with no constructor args
  class Type2: public Registrator<Type2>
  {
  private:
    template<typename T> friend struct Factory::Registrator;
    Type2() = default;

  public:
    void doSomeThing()
    {
      std::cout << "hello "<< typeid(this).name() << " world" << std::endl;
    }
  };


  // class that is not polymorphic with three constructor args
  class Type3: public Registrator<Type3>
  {
  private:
    template<typename T> friend struct Factory::Registrator;
    Type3(int x, double y, unsigned long z)
    {
      std::cout << "hey Type3 constructor called with three argument "
		<< x << " "
		<< y << " "
		<< z << " "
		<< std::endl;
    }

  public:
    void doSomeThing()
    {
      std::cout << "hello "<< typeid(this).name() << " world" << std::endl;
    }
  };


  // polymorphic class for abstract factory
  class Type4: public DemoIF//, AbstractRegistrator<DemoIF, Type4>
  {
  public:
    //friend struct AbstractFactory<DemoIF>::template Registrator<Type4>;
    Type4() = default;
    Type4(const Type4&) = delete;

  public:
    void doSomeThing()
    {
      std::cout << "hello "<< typeid(this).name() << " world" << std::endl;
    }
    inline static const bool isRegistered = AbstractFactory<DemoIF>::Register<Type4>();
  };


  class Type5: public DemoIF//, AbstractRegistrator<DemoIF, Type4>
  {
  public:
    //friend struct AbstractFactory<DemoIF>::template Registrator<Type5>;
    Type5() = default;
    Type5(const Type5&) = delete;

  public:
    void doSomeThing()
    {
      std::cout << "hello "<< typeid(this).name() << " world" << std::endl;
    }
    inline static const bool isRegistered = AbstractFactory<DemoIF>::Register<Type5>();
  };



}
