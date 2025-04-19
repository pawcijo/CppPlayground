# Cpp Playground

Welcome to my C++ playground.

It includes examples showcasing both fundamental and advanced language features. <br />
You can find basic information about things like type deduction, smart pointers, polymorphism, throwing and catching exceptions, virtual functions and classes, deep and shallow copy differences and more.

All topics are numbered and you can run a demo that shows information about the code. <br />

You can also debug the code to learn more and get additional details.


Before running download submodules: 

```
git submodule update --init
```

Debug mode :
```
mkdir Build/Debug 
cd Build/Debug
cmake -DCMAKE_BUILD_TYPE=Debug ../..
make
```

Release mode :
```
mkdir Build/Release
cd Build/Release
cmake -DCMAKE_BUILD_TYPE=Release ../..
make
```

## TODO:

### Design patterns:
- adapter
- bridge
- decorator
- facade
- flyweight
- composite
- proxy
- chain of responsibility
- command
- iterator
- mediator
- memento
- observer 
- state
- strategy
- template
- visitor



#### C++20:
- bit
- compare
- concepts
- coroutine
- format
- numbers
- ranges
- source_location
- span
- syncstream
- version
- barrier
- latch
- semaphore
- stop_token 
