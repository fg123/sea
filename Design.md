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
