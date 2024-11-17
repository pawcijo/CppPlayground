#pragma once

#include <iostream>

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

inline void SingleResponsibilityPrinciple()
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

inline void OpenClosedPrinciple()
{
    LOG_FUNCTION_NAME("35");
    std::cout << std::endl
              << GREEN_TEXT_COLOR << "Open for Extension:" << TEXT_COLOR_CLEAR << R"(
This means that the behavior of a module, class, or function
can be extended or enhanced.
You should be able to add new functionality without altering existing code.)"
              << std::endl
              << std::endl
              << GREEN_TEXT_COLOR << "Closed for Modification:" << TEXT_COLOR_CLEAR << "\n"
              << R"(
This means that once a class or module has been developed and tested,
you should not change its internal source code.
Modifications should be avoided to prevent introducing bugs or
breaking existing functionality.
)" << std::endl

              << BOLD_TEXT_START << "Inheritance and Polymorphism " << TEXT_FORMAT_CLEAR
              << std::endl
              << "//Base class"
              <<
        R"(class Shape {
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

              << BOLD_TEXT_START << "Or Composition" << TEXT_FORMAT_CLEAR << "\n"

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
)" << std::endl;
}

inline void LiskovSubstitutionPrinciple()
{
    LOG_FUNCTION_NAME("34");

    std::cout << R"(
"Objects of a superclass should be replaceable with objects of a subclass
without affecting the correctness of the program.")"
              << std::endl
              << std::endl
              << BOLD_TEXT_START << "Breaking the principle exaple : \n\n"
              << TEXT_FORMAT_CLEAR << R"(class Bird {
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

inline void InterfaceSegregationPrinciple()
{
    LOG_FUNCTION_NAME("33");

    std::cout << "Clients should not be forced to depend on interfaces they do not use."
              << std::endl
              << std::endl;
    std::cout << BOLD_TEXT_START << "Breaking the principle exaple : \n" << TEXT_FORMAT_CLEAR;

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

    std::cout << BOLD_TEXT_START << "Applying the principle: : \n" << TEXT_COLOR_CLEAR;

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

inline void DependencyInversionPrinciple()
{
    LOG_FUNCTION_NAME("32");

    std::cout
        << R"( "High-level modules should not depend on low-level modules. Both should depend on abstractions." )"
        << std::endl;
    std::cout
        << R"( "Abstractions should not depend on details. Details should depend on abstractions." )"
        << std::endl
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
};)" << std::endl
              << std::endl;

    std::cout << BOLD_TEXT_START << "Applying the principle: : \n\n" << TEXT_FORMAT_CLEAR;
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

class SOLID_Demo : public DemoBase
{
   public:
    SOLID_Demo()
    {
        mName = "SOLID_Demo";
        mNotes = {};
    };
    ~SOLID_Demo() = default;

    void ShowExample() override
    {
        PrintNotes();

        std::cout << CYAN_TEXT_COLOR << "S" << TEXT_COLOR_CLEAR << RED_TEXT_COLOR << "O"
                  << TEXT_COLOR_CLEAR << BLUE_TEXT_COLOR << "L" << TEXT_COLOR_CLEAR
                  << YELLOW_TEXT_COLOR << "I" << TEXT_COLOR_CLEAR << GREEN_TEXT_COLOR << "D"
                  << TEXT_COLOR_CLEAR;

        SingleResponsibilityPrinciple();
        OpenClosedPrinciple();
        LiskovSubstitutionPrinciple();
        InterfaceSegregationPrinciple();
        DependencyInversionPrinciple();
    }

    void ShowDemo() override
    {
        ShowExample();
    };
};
