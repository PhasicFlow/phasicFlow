# PhasicFlow Coding Style Guidelines
This document outlines the coding style guidelines for the PhasicFlow codebase. 
Adhering to these guidelines ensures consistency, readability, and maintainability of the project.
## 1. FormattingIndentation: 
* Use 4 spaces for every logical level and block.
* Line Spacing: Leave two empty lines between sections (e.g., between functions in a .cpp file, between class members).

## 2. Naming ConventionsGeneral Naming: 
* All names should start with lowercase letters, except for special names (e.g., Ergun, Hertz).
* Macro names start with Upper case or all the letters are in UPPER case.
* Compound Names: For compound names, the first word starts with a lowercase letter, and subsequent words start with an uppercase letter (e.g., boundaryBase, motionModel).

## 3. File Structure 
* Header Files: Use the .hpp extension for header files.
* Source Files: Use the .cpp extension for source files.
* Header and Source File Headers: All header and source files must include a standardized header that describes the project's intention and licensing information.
* File Naming: Header and source file names should correspond to the class they contain. Aim for one class per file.
* Inline Functions: Place inline functions in a separate classNameI.hpp file to avoid cluttering the main header file.

## 4. Class DesignClass Member Order:
* Private members and methods
* Private static members and methods
* Public methods
* Public static methods
* Enumerations and Nested Classes: Declare enumerations and nested classes before all class members and methods.
* Special Functions: Each class must explicitly define all special functions:Constructor, Copy constructor and assignment operator, Move constructor and assignment operator
* Destructor: Each class must have an explicit destructor declaration:`~className() = default`; or `~className();`
* Interface classes or classes with virtual methods must have a virtual destructor.
* Virtual Method Overrides: When implementing a `virtual` method from a base class in a derived class, use the `override` keyword. The same applies to derived class destructors.
## 5. NamespacesOfficial Namespace: 
The official namespace for the codebase is pFlow. All entities should be defined within this namespace.

### Example File Structure
```
src/
├── componentName1/
│   ├── componentName1.hpp
│   ├── componentName1.cpp
│   ├── componentName1I.hpp
│   └── ...
└── componentName2/
    ├── componentName2.hpp
    ├── componentName2.cpp
    └── ...
```
### Example Class Structure
```C++
namespace pFlow 
{

class MyClass 
{
public:
    enum class MyEnum 
    {
        Value1,
        Value2
    };

    class NestedClass 
    {
        // ...
    };

private:
    int privateMember_;
    
    void privateMethod();

    
    static int privateStaticMember_;
    
    static void privateStaticMethod();

public:
    
    MyClass();

    MyClass(const MyClass& other);
    
    MyClass(MyClass&& other);
    
    MyClass& operator=(const MyClass& other);
    
    MyClass& operator=(MyClass&& other);
    
    ~MyClass();

    
    void publicMethod();
    
    static void publicStaticMethod();

};

// assuming base class has virtual methods 
class DerivedClass 
: 
    public BaseClass 
{
public:
     
     ...

    ~DerivedClass() override;
    
     void virtualMethod() override;
};

} // namespace pFlow
``` 
