Factory template code with auto registration of factory object types.
The code is executed using google test with some trivial tests showing how different
object types might register with the factory type.

you can build debug code as follows:

```
mkdir Debug
cd Debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

you can run this code as follows:
```
./tests/runTests
```

Note: this is meant to be an example of a simple design pattern for factory and abstract factory.
I also use this sandbox to try out things in c++ 11,14, 17 and 20.
This means that on occasion some of those trials creap into git

Note that I'm being sloppy with the use of the term "Factory".  The examples are variations on a theme.
The basic idea is as follows:

1) building a family of objects (technically an Abstract Factory)
2) minimal requirements for the user to use
    * generally the user type needs to inherit from Factory::Registrator
    * one issue is that, due to templates, the user must call the Registrator constructor
3) Type erasure removes the user type for a inherited interface and returns a pointer (smart or otherwise)
4) User calls the factory Create method to allocate an object

This is, obviously, not production worthy code.  There are a lot of things no handled, cv stripping, etc.
Generally this is just my note pad on some ideas and tricks.

Some interesting issues crop up in this code:
1) Constructor arguement handling.  Because of type erasure it should be assumed that each object conforms to a fixed constructor signature.
    The current checked in code assumes the signature is void.
    There are several ways to fix this.  The most obvious is to add variadic agruments to the factory template.
    The less obvious, and more complex, is to use Template Type Deduction and push the argument list in from the Object Registration.
    The former has a host of hoops to jump to get it to work.  The biggest issue being the signature of the unordered_map that holds all the object constructors.
2) Dealing with std::any.  I've played around with std::any a bit after writing this initial code.
   this link provided a good set of ideas: 
                    https://en.cppreference.com/w/cpp/utility/any/type
                    
   The any_cast<> gives some difficult issues (recall we have type erasure).
   I've written a third variation on this code using the lambda idea for doing the cast from the above link.
   At some point I will check that in.
3) My vision for this code is use in cases of startup/initialization (i.e. non performance paths). 
  These use case typically follow the path of creating and initialization and then the factory is no longer used.
  If you're worried about performance then you should profile the code.  Ideally all this code would be constexpr.
  If that were the case the performance would likely be really good. See youtube.com video:
        https://www.youtube.com/watch?v=INn3xa4pMfg
  Bottom line is everything the factory builds is known at compile time and the internal unordered_map<> should not change during runtime.
  Note this is really a compile time reflection problem :^)
   
Singleton is a note for me on using std::weak_ptr<>.  The use semantics are a bit different than the other smart pointers (ie. lock()).
The idea is to have a factory that is destructed once the user is done with it.  Obviously for 

Note that currently I have not added an example of Decorator.  This pattern, runtime, is pretty straight forward.
I would add an example of compile time decoration using CRTP but I have found that generally coding this pattern is very case specific.
If you are interested think of static decoration as the inverse of polymorphism since templates do not support virtual methods, unfortunately.
In polymorphism you "override" a function.  In static decoration you inherit a function, hence CRTP, but access is always from the derived class.
Access to the base class method is through BASE::Method() calls.  Note that building a class using static decoration removes the ability to
use type erasure on the Class hierarchy. This should be obvious since you are building a type via the compiler.


I'll add more notes as the ocassion arises
