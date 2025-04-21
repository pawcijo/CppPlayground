#pragma once


//Expected interface by the client
class ClientInterface
{
public:
  virtual ~ClientInterface() = default;

  virtual void doSomethingCool() = 0;
  virtual void doSomething() = 0;
  virtual void doSomethingElse() = 0;
};