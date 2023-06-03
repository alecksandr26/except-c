# Table of Contents
* [Description](https://github.com/alecksandr26/c-exceptions#c-exceptions)
* [How to install it?](https://github.com/alecksandr26/c-exceptions#how-to-install-it-)
* [Getting started](https://github.com/alecksandr26/c-exceptions#getting-started)
* [References](https://github.com/alecksandr26/c-exceptions#references)

# Try-Catch-C(tc-c)
This module provides a simple macro interface that allows for easy exception handling in C, similar to Python's approach.

# How to install it ?
## Using `yay`
1. Open a terminal window and ensure that you have yay installed on your system.
2. Run the command `yay -S c-exceptions` to search for and install the package from the [AUR (Arch User Repository)](https://aur.archlinux.org/packages/c-exceptions).

## Using `make`
1. Download the source code for the `c-exceptions` package from [releases](https://github.com/alecksandr26/c-exceptions/releases/tag/v1.0.0)
2. Extract the contents of the downloaded archive to a directory of your choice.
3. Open a terminal window and navigate to the directory where you extracted the source code.
4. Run the command `make compile` to compile the dependencies.
5. Run the command `sudo make install` to install the package system-wide. You may be prompted to enter your ***administrator password***.

# Getting started
1. Install the [c-exceptions](https://github.com/alecksandr26/c-exceptions#how-to-install-it-) library on your system.
2. Include the except.h header file in your C source code file using the following statement:
```c
#include <except.h>
```
3. Define your exception(s) using the `Except` type, as shown in the following example:
```c
Except someError = {"Some error"};
```
In this example, `someError` is ***an exception*** of type `Except`, with the message "Some error".

4. Use the try, raise, and except macros to handle exceptions in your code. Here is an example code snippet:
```c
try {
    throw_except(someError); // <--- throw the exception of someError
} except(someError) {
    puts("Exception caught!");
} endtry;
```
his code snippet attempts to raise the someError exception using the raise macro. If the exception is raised, the except macro will catch it and execute the code block following it. In this case, the code block simply prints a message to the console.

5. Compile your code with ***the -lexcept flag*** to link against the ***c-exceptions library***. For example:
```
cc mycode.c -lexcept
```
## Video example

https://user-images.githubusercontent.com/66882463/236306906-a3222791-bc2c-4011-bf60-5b39bac510fc.mp4



# References
1. Mark. (2023). How setjmp and longjmp work. Offlinemark. https://offlinemark.com/2016/02/09/lets-understand-setjmp-longjmp/
2. Hanson, D. (1996). C Interfaces and Implementations: Techniques for Creating Reusable Software.
