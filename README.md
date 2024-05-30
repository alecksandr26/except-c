# Table of Contents
* [Description](https://github.com/alecksandr26/except-c?tab=readme-ov-file#except-c-except-c)
* [How to install it?](https://github.com/alecksandr26/except-c?tab=readme-ov-file#how-to-install-it-)
    * [Using yay](https://github.com/alecksandr26/except-c?tab=readme-ov-file#using-yay)
    * [Using make](https://github.com/alecksandr26/except-c?tab=readme-ov-file#using-make)
* [Getting started](https://github.com/alecksandr26/except-c?tab=readme-ov-file#getting-started)
    * [Video example](https://github.com/alecksandr26/except-c?tab=readme-ov-file#video-example)
* [References](https://github.com/alecksandr26/except-c?tab=readme-ov-file#references)

# Except C (except-c)
This module offers a straightforward macro interface that facilitates seamless exception handling in the C programming language, drawing inspiration from the paradigm employed in Python.

# How to install it ?
## Using `yay`
1. Open a terminal window and ensure that you have yay installed on your system.
2. Run the command `yay -S except-c` to search for and install the package from the [AUR (Arch User Repository)](https://aur.archlinux.org/packages/except-c).

## Using `make`
1. Download the source code for the `except-c` package from [releases](https://github.com/alecksandr26/except-c/archive/refs/tags/v2.0.0.tar.gz)
2. Extract the contents of the downloaded archive to a directory of your choice.
3. Open a terminal window and navigate to the directory where you extracted the source code.
4. Run the command `make pkg` to build a `x86_64.pkg.tar.zst` package, or you can run the command `make compile` to build the file`libexcept.a`.
5. Then just install the pacakge.

# Getting started
1. Install the [except-c](https://github.com/alecksandr26/except-c/tree/main?tab=readme-ov-file#how-to-install-it-) library on your system.
2. Include the `except.h` header file in your C source code file using the following statement:
```c
#include <except.h>
```
3. Define your exceptions using the `Except_T` type, as demonstrated in the following example:
```c
Except_T someError = INIT_EXCEPT_T("Some error");
```
In this instance, `someError` represents an exception of the `Except_T` type, accompanied by the message "Some error."
Alternatively, you can leverage the ***general-purpose exceptions*** already defined in the [except.h](https://github.com/alecksandr26/except-c/blob/main/include/except.h) header file, 
depending on your specific requirements.
```c
/* Logical exceptions */
extern T ExceptLogicalError;
extern T ExceptInvalidArgument;
extern T ExceptDomainError;
extern T ExceptLengthError;
extern T ExceptOutOfRange;

/* System error exceptions */
extern T ExceptRuntimeError;
extern T ExceptRangeError;
extern T ExceptOverflowError;
extern T ExceptSystemError;
extern T ExceptFileSystemError;
extern T ExceptThreadSystemError;
extern T ExceptSocketSystemError;
extern T ExceptReadSystemError;
extern T ExceptWriteSystemError;

/* Memory exceptions */
extern T ExceptBadAlloc;
extern T ExceptBadArrayNewLength;
extern T ExceptBadPtr;

/* Signal exceptions */
extern T ExceptSigAbrt;
extern T ExceptFpe;
extern T ExceptSigIll;
extern T ExceptSigSegv;
extern T ExceptSigTerm;
extern T ExceptSigSys;
```

4. Use the TRY, END_TRY, RAISE, and EXCEPT macros to handle exceptions in your code. Here is an example code snippet:
```c
TRY {
    RAISE(someError); // <--- raise the exception of someError
} EXCEPT(someError) {
    puts("Exception caught!");
} END_TRY;
```
This code snippet attempts to raise the someError exception using the throw macro. If the exception is raised, the except macro will catch it and execute the code block following it. 
In this case, the code block simply prints a message to the console.
Additionally, you have the option to include a ***message*** when raising an exception to provide supplementary information about the error:
```c
TRY {
    RAISE(someError, "This is a message"); // <--- Adding an message
} EXCEPT(someError) {
    puts("Exception caught!");
} END_TRY;
```
This modification allows you to add a descriptive message when throwing the `someError` exception, enhancing the contextual 
information available during exception handling and debugging.

5. Compile your code with ***the -lexcept flag*** to link against the ***except-c library***. For example:
```
cc mycode.c -lexcept
```

## Video example




https://github.com/alecksandr26/except-c/assets/66882463/66f9207a-b74d-4a96-a049-01d59aee353c



# The new `assert`
By including the `except/assert.h` header, you override the default `assert` functionality in C. With this new implementation, a `failed assertion` will be treated as an exception.
1. Include the `except/assert.h` header file in your C source code file using the following statement:
```c
#include <except/assert.h>
```
2. You can then use the `assert()` keyword to evaluate a condition and add a custom message.
```c
assert(0, "This is a message");   // condition, message
```
3. Compile your code with ***the -lexcept flag*** to link against the ***except-c library***. For example:
```
cc mycode.c -lexcept
```

## Output of the `assert`
This is an example output of an uncaught exception from a failed assertion.
```
[arch]$ cc assert.c -lexcept
[arch]$ ./a.out 
Traceback...
        File "mycode.c", line 9, raised in main()
UncaughtException:       "Assertion failed",     "This is a message"

Aborting....
Aborted (core dumped)
```


# References
1. Mark. (2023). How setjmp and longjmp work. Offlinemark. https://offlinemark.com/2016/02/09/lets-understand-setjmp-longjmp/
2. Hanson, D. (1996). C Interfaces and Implementations: Techniques for Creating Reusable Software.
