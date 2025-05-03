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

## 6. Doxygen Documentation

### 6.1. Ruls
provide the documentations in the header files only. In rare cases where you are generating documentations for executables, the doxygen documentation can be supplied in the .cpp file.  

* **General Doxygen Style:**

    * Use `///` for short documentations for methods and members.

    * Use `/** */` for classes and main methods which play a significant role in the class or code.

    * Place Doxygen comments *before* the declaration of the entity being documented (e.g., class, function, variable).

    * Use `@param` to document function parameters, `@return` for return values, `@brief` for a short description, and `@details` for a more in-depth explanation.

    * Use Markdown syntax within Doxygen comments for formatting.

* **File Headers:** Each file should contain a Doxygen comment at the top, including:

    * `@file` : The name of the file.

    * `@brief`: A brief description of the file's purpose.

    * `@author`: The author(s) of the file.

    * `@date` : The date of creation or last modification.

* **Class Documentation:**

    * Use `/** */` for class documentation.

    * Provide a `@brief` description of the class.

    * Use `@tparam` to document template parameters.

    * Document the purpose of the class, its invariants, and how it should be used.

* **Function/Method Documentation:**

    * Use `///` for short documentations.

    * Use `/** */` for main methods which play a significant role.

    * Provide a `@brief` description of the function.

    * Use `@param` to describe each parameter, including its purpose and whether it is an input, output, or input/output parameter.

    * Use `@return` to describe the return value, including its meaning and possible values.

    * Use `@pre` to document any preconditions that must be met before calling the function.

    * Use `@post` to document any postconditions that will be true after the function returns.

    * Use `@throws` to document any exceptions that the function may throw.

    * Use `@details` for a more detailed explanation of the function's behavior, algorithms, or any other relevant information.

* **Variable Documentation:**

    * Use `///<` for single-line documentation of variables.

### 6.2. Doxygen Documentation Examples

* **Class example**

```cpp
/**
 * @brief Represents a particle in the simulation.
 * @details This class stores the position, velocity, and other physical
 * properties of a particle.
 */
class Particle
{
private:

    Point     position_;   ///< The current position of the particle.

    Vector    velocity_;   ///< The current velocity of the particle.

    double     mass_;      ///< The mass of the particle.

public:

    /// Constructs a particle with default values.
    Particle();

    /**
     * @brief Updates the position of the particle based on its velocity
     * and the given time step.
     * @param deltaTime The time elapsed since the last update, in seconds.
     */
    void updatePosition(const timeInfo& ti );

    /// Gets the current position of the particle.
    Point getPosition() const;
};
```

* **Function Example**

```cpp
/**
 * @brief Calculates the distance between two points.
 * @param p1 The first point.
 * @param p2 The second point.
 * @return The distance between the two points.
 */
double calculateDistance(const Point& p1, const Point& p2)
{
    // Implementation
    return 0.0;
}

/// Returns the velocity of the particle.
Vector getVelocity() const
{
    return velocity_;
}
``` 
