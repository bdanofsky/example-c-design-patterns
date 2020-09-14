/*
 * abstractFactory.h
 * self registering factory
 *
 * TODO: it would be nice to support variadic constructors
 *       on the surface this might seem easy but type erasure
 *       causes issues
 *       if you look at factory.h you can see a different approach
 *       the classes are not polymorphic and we can use a variadic template
 *       But, a big but here, the type of the object is passed to Create.
 *       This would not support the use case of creating objects based on 
 *       string type name
 */
#pragma once
#include <functional>
#include <map>
#include <iostream>
#include <memory>
#include <any>
#include <typeindex>
#include <typeinfo>

namespace FactoryPattern
{
  // Factory to create Polymorphic class
  // class is created based on name string
  // one issue is that the class constructor must all use the same signature
  //

  // Note there may be issues with using TYPEID(T).name() if the name is completely mangled
  // TBD is there a better way
  // could pass the name on registration
  //
  template<typename Interface>
  class AbstractFactory
  {
  public:
    template<typename T> friend struct Registrator;

    template<typename T>
    static bool Register()
    {
      auto& mapper = AbstractFactory<Interface>::GetMap();
      std::string name = typeid(T).name();
      std::cout << "trying to  register: " << name << std::endl;
      auto algo = mapper.find(name);
      if (algo != mapper.end())
	{
	  std::cout << "ERROR: " << name << " has already been registered" << std::endl;
	  return false;
	}
      else
	{
	  mapper[name] = [](void) -> AbstractIFType { return std::make_shared<T>(); };
	  std::cout << name << ": registered" << std::endl;
	}

      return true;
    }

  public:    
    using AbstractIFType = std::shared_ptr<Interface>;
    using ConstructorType = std::function< AbstractIFType(void)>;
    using ContainerType = std::unordered_map<std::string, ConstructorType>;
    AbstractFactory() = default;
    AbstractFactory(const AbstractFactory&) = delete;

    template<typename Iface, typename T>
    struct Registrator
    {
      Registrator() = default;

      //template<typename T, typename ARGS...>
      auto Create()
      {
	if (!isRegistered) throw std::runtime_error("ERROR: for some reason abstract factory registration did not occur");
	return AbstractIFType(new T());
      }
      
      inline static const bool isRegistered = AbstractFactory<Iface>::template Register<T>();
    };
    
    std::shared_ptr<Interface> Create(const std::string& name)
    {
      std::vector<std::string> spaces;
      auto pos = name.find(":");
      auto typeName = name;
      while (pos != std::string::npos)
	{
	  auto sub = name.substr(0,pos);
	  spaces.push_back(sub);
	  while(std::strncmp(&name[pos], ":", 1) == 0) pos++;// move past ":" chars
	  typeName = name.substr(pos, std::string::npos); 
	  pos = name.find(":", pos); // next
	}

      auto& mapper = AbstractFactory::GetMap();
      for (auto nameConstructor: mapper) // pair (typeid name, constructor)
	{
	  std::string typeInfoName = nameConstructor.first;
	  if (typeInfoName.find(typeName) != std::string::npos)
	    {
	      bool nsMatch = true;
	      for (auto ns : spaces)// check namespace matches
		{
		  if (typeInfoName.find(ns) == std::string::npos)
		    {
		      nsMatch = false;
		      break;
		    }
		}
	      if (!nsMatch) continue;// same type name different namespace

	      return nameConstructor.second();
	    }
	}
      std::cout << "ERROR: cannot find " << name << " as available type" << std::endl;
      return nullptr;

    }

  static ContainerType& GetMap()
  {
    // force the container to be live to avoid
    // static variable init order issues
    static ContainerType container;
    return container;
  }

  };
    
}
