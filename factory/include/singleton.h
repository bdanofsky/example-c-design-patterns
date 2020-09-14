/*
 * single.h
 * single base class
 *
 *
 */
#pragma once
#include <memory>

namespace Factory
{
  
  template<typename Derived>
  class Singleton
  {
    // To use Singleton make it a friend of Derived and
    // make the constructors of Derived private
    // This singleton pattern will loose its reference if all callers of create()
    // delete their shared pointers
  public:
    Singleton<Derived>(const Singleton&) = delete;
    
    template<typename... ARGS>
    static std::shared_ptr<Derived> Create(ARGS... args)
    {
      auto ret = instance.lock();
      if (ret != nullptr) return ret;

      // note that if the Derived constructor is private
      // then you cannot call std::make_shared which would be preferable
      std::shared_ptr<Derived> shared(new Derived(args...));
      instance = shared;
      return shared;
    }

  private:
    friend Derived;
    Singleton<Derived>() = default;
    inline static std::weak_ptr<Derived> instance {};
  };
    
}
