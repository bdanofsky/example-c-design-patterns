/*
 * factory.h
 * self registering factory
 * currently supports variadic contructor of built types
 *
 */
#pragma once
#include <unordered_map>
#include <iostream>
#include <any>
#include <typeindex>
#include <typeinfo>
#include <stdexcept>

namespace FactoryPattern
{
  
  class Factory
  {
    using ContainerType = std::unordered_map<std::type_index, std::any>;
    template<typename T>
    using ObjType = std::shared_ptr<T>;
    
  private:
    
    template<typename T> friend struct Registrator;

    template<typename T>
    static bool Register()
    {
      auto& mapper = Factory::GetMap();
      std::cout << "trying to  register: " << typeid(T).name() << std::endl;
      auto idx = std::type_index(typeid(T));
      
      auto itr = mapper.find(idx);
      if (itr != mapper.end())
	{
	  std::cout << "ERROR: " << typeid(T).name() << " has already been registered" << std::endl;
	  return false;
	}
      else
	{
	  mapper[idx] = Registrator<T>();
	  std::cout << typeid(T).name() << ": registered" << std::endl;
	}

      return true;
    }
    
    
  public:
    Factory() = default;
    Factory(const Factory&) = delete;

    
    template<typename T>
    struct Registrator
    {
      Registrator() = default;

      template<typename... ARGS>
      auto Create(ARGS... args)
      {
	if (!isRegistered) throw std::runtime_error("ERROR: for some reason factory registration did not occur");
	return ObjType<T>(new T(args...));
      }
      
      inline static const bool isRegistered = Factory::Register<T>();
    };
    

    
    template<typename T, typename... ARGS>
    auto Create(ARGS... args)
    {
      auto& mapper = Factory::GetMap();
      auto idx = std::type_index(typeid(T));
      auto itr = mapper.find(idx);
      if (itr != mapper.end())
	{
	  auto registrator = std::any_cast<Registrator<T>>(itr->second);
	  return registrator.Create(std::move(args)...);
	}
      else
	{
	  std::cout << "ERROR: cannot find " << typeid(T).name() << " as available type" << std::endl;
	  return ObjType<T>(nullptr);
	}

    }

    // get object based on string name
    template<typename... ARGS>
    auto Create(std::string name, ARGS... args)
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

      auto& mapper = Factory::GetMap();
      for (auto idxAny: mapper)
	{
	  std::string typeInfoName (idxAny.first.name());
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
	      // TODO check the namespaces are correct
	      //auto registrator = std::any_cast<Registrator<T>>(itr->second);
	      //return registrator.Create(args...);
	      return nullptr;
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
