# Sea

Java / Kotlin style language that transpiles into C / C++ with a clean design.

The generated files are guaranteed to be compilable. 

# Comments
Comments are denoted with `//` or `/* */` as typical in other languages.

# Types
All types are Object / Reference types in order to allow for extension
functions, even primitive types. These can be transpiled into primitives in 
C / C++. 

Nullability types provide null pointer dereference safety in C / C++.

# Packages
Each `.sea` file is a package that can be imported into another file. Public 
declarations are exposed in a public interface (which can be inter-op) with 
other pure C files. Declarations are private by default. Packages are denoted
simply by the directory structure of the project. 

Each `.sea` file produces three files, a `.seai` interface file used for Sea 
compilation, and a set of `.h/.cc` files for C++ compilation.

# Functions

# Control Flow
Standard control flow, but also has pattern matching?

# Statements

## Local Variable Declaration
```
const x = 10;        // Type Deduction
const y : Long = 10; // Specify Type
let z = 10;          // Mutable Value
let z : Long = 10;   // Mutable Value with specific type
```

# Interfaces
Interfaces are declared with `interface`. They cannot contain any fields, but
can contain base / reference implementations to methods.

Interfaces can extend any number of sub-interfaces
```
public interface A {
    function A (value : Int) {
        return 0;
    }
}

public interface B extends A {
    override function A (value : Int) {
        return 1;
    }
    function B (value : Int) {
        return 2;
    }
};
```