#pragma once

#include "Common.hpp"

#include <iostream>

namespace SOLID
{
    void SingleResponsibilityPrinciple()
    {

        LOG_FUNCTION_NAME("36");
        std::cout << R"(
"There should never be more than one reason for a class to change."
In other words, every class should have only one responsibility.

class DoOneThing
{   
int variable;
double another_variabl;
...
public:
   void DoStuff();
};

)" << std::endl;
    }

    void OpenClosedPrinciple()
    {
        LOG_FUNCTION_NAME("35");
        std::cout << std::endl
                  << "\033[1;32;43mOpen for Extension:\033[0m\n"
                  << R"( 
This means that the behavior of a module, class, or function 
can be extended or enhanced. 
You should be able to add new functionality without altering existing code.)"
                  << std::endl
                  << std::endl
                  << "\033[1;37;43mClosed for Modification:\033[0m \n"
                  << R"(
This means that once a class or module has been developed and tested, 
you should not change its internal source code. 
Modifications should be avoided to prevent introducing bugs or 
breaking existing functionality.
)" << std::endl

                  << "\033[1;37mInheritance and Polymorphism\033[0m\n"

                  << R"(
// Base class
class Shape {
public:
    virtual void draw() const = 0; // Pure virtual function
};

// Extended class
class Circle : public Shape {
public:
    void draw() const override {
        std::cout << "Drawing a circle" << std::endl;
    }
};

// Another extended class
class Square : public Shape {
public:
    void draw() const override {
        std::cout << "Drawing a square" << std::endl;
    }
};
)" << std::endl

                  << "\033[1;37mOr Composition\033[0m\n"

                  << R"(
class Logger {
public:
    virtual void log(const std::string& message) = 0;
};

class FileLogger : public Logger {
public:
    void log(const std::string& message) override {
        // Log message to a file
    }
};

class DatabaseLogger : public Logger {
public:
    void log(const std::string& message) override {
        // Log message to a database
    }
};

class Application {
private:
    Logger& logger;
public:
    Application(Logger& logger) : logger(logger) {}

    void doSomething() {
        logger.log("Doing something...");
    }
};

Here, 

)" << std::endl;
    }

    void LiskovSubstitutionPrinciple()
    {
        LOG_FUNCTION_NAME("34");

        std::cout << R"(
"Objects of a superclass should be replaceable with objects of a subclass
without affecting the correctness of the program.")"
                  << std::endl
                  << std::endl
                  << "\033[1;37mBreaking the principle exaple :\033[0m \n\n"

                  << R"(class Bird {
public:
    virtual void fly() {
        std::cout << "Flying!" << std::endl;
    }
};

class Penguin : public Bird {
public:
    void fly() override {
        // Penguins can't fly!
        std::cout << "I can't fly!" << std::endl;
    }
};)" << std::endl

                  << "\033[1;37mApplying the principle:\033[0m \n"

                  << R"(
class Bird {
public:
    virtual void move() = 0; // Pure virtual function
};

class FlyingBird : public Bird {
public:
    void move() override {
        std::cout << "Flying!" << std::endl;
    }
};

class Penguin : public Bird {
public:
    void move() override {
        std::cout << "Swimming!" << std::endl;
    }
};
)";
    }

    void InterfaceSegregationPrinciple()
    {
        LOG_FUNCTION_NAME("33");

        std::cout << "Clients should not be forced to depend on interfaces they do not use." << std::endl;
        std::cout << "\033[1;37mBreaking the principle exaple :\033[0m \n\n";

        std::cout << R"(
    class Worker {
public:
    virtual void work() = 0;
    virtual void eat() = 0;
    virtual void sleep() = 0;
};

class Human : public Worker {
public:
    void work() override { /*...*/ }
    void eat() override { /*...*/ }
    void sleep() override { /*...*/ }
};

class Robot : public Worker {
public:
    void work() override { /*...*/ }
    void eat() override { /*...*/ }  // Robots don't eat!
    void sleep() override { /*...*/ } // Robots don't sleep!
};
    )";

        std::cout << "\033[1;37mApplying the principle: :\033[0m \n\n";

        std::cout << R"(
    class Workable {
public:
    virtual void work() = 0;
};

class Eatable {
public:
    virtual void eat() = 0;
};

class Sleepable {
public:
    virtual void sleep() = 0;
};

class Human : public Workable, public Eatable, public Sleepable {
public:
    void work() override { /*...*/ }
    void eat() override { /*...*/ }
    void sleep() override { /*...*/ }
};


)";
    }

    void DependencyInversionPrinciple()
    {
        LOG_FUNCTION_NAME("32");

        std::cout << R"( "High-level modules should not depend on low-level modules. Both should depend on abstractions." )" << std::endl;
        std::cout << R"( "Abstractions should not depend on details. Details should depend on abstractions." )" << std::endl
                  << std::endl;

        std::cout << "\033[1;37mBreaking the principle exaple :\033[0m \n\n";
        std::cout << R"(
class Database {
public:
    void connect() { /* ... */ }
    void saveUser(std::string user) { /* ... */ }
};

class UserService {
    Database db;
public:
    UserService() : db() {}
    void addUser(std::string user) {
        db.connect();
        db.saveUser(user);
    }
};)" << std::endl;

        std::cout << "\033[1;37mApplying the principle: :\033[0m \n\n";
        std::cout << R"(
// Abstraction
class IDatabase {
public:
    virtual void connect() = 0;
    virtual void saveUser(std::string user) = 0;
    virtual ~IDatabase() = default;
};

// Low-Level Module
class Database : public IDatabase {
public:
    void connect() override { /* ... */ }
    void saveUser(std::string user) override { /* ... */ }
};

// High-Level Module
class UserService {
    IDatabase& db;
public:
    UserService(IDatabase& db) : db(db) {}
    void addUser(std::string user) {
        db.connect();
        db.saveUser(user);
    }
};
)" << std::endl;
    }

    void SOLID_Demo()
    {

        std::cout << "\033[1;36mS \033[0m "
                  << "\033[1;35mO \033[0m "
                  << "\033[1;34mL \033[0m "
                  << "\033[1;33mI \033[0m "
                  << "\033[1;32mD\033[0m";

        SingleResponsibilityPrinciple();
        OpenClosedPrinciple();
        LiskovSubstitutionPrinciple();
        InterfaceSegregationPrinciple();
        DependencyInversionPrinciple();
    }
}