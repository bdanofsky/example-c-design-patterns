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
