# Spirick Tuning

A C++ Class and Template Library for Performance Critical Applications

# Optimized for Performance

The **Spirick Tuning** library provides a large number of classes for memory and object management, strings, multithreading etc.
All classes are optimized for performance.
There are many ways to reduce the memory consumption and to increase the computing speed.
Using the **Spirick Tuning** library, large datasets can be efficiently processed in main memory.

# Well-Defined Runtime Behaviour

Many container libraries attach great importance to specific interfaces, e.g Smalltalk-like collections.
These libraries provide no information about the implementation.
The runtime behaviour may change depending on the compiler version and the library version.

All **Spirick Tuning** classes have a well-defined runtime behaviour.
The implementation of all interfaces is documented in detail.
In this way an optimal solution can be found for almost all use cases.

# Strict Separation of Memory and Object Management

Store and block classes manage memory, container and collection classes manage objects - that's it!

# High Flexibility, Simple Extensibility

All store, block and container classes share common interfaces which allow
to flexibly combine various implementations and to easily extend the existing functionality.

# Long-Standing Stable Interfaces and Stable Implementation

Over a period of 25 years the **Spirick Tuning** library was extended with some new interfaces.
However, the existing interfaces remain mostly unchanged.
The library remains compatible to existing applications.

Over time, some new functionality was implemented.
However, the existing implementation remains mostly unchanged.
There were no changes in the runtime behaviour.

# Quality Assurance

* All source files compile warning-free at the highest warning level.
* Parameter values and internal error conditions are checked via Assert macros.
* The output of test programs is compared to reference data.

# Unique Features

The **Spirick Tuning** library has some unique features, for example:

1. Nested size and position types  
   All store, block and container classes contain nested size and position types.
   Some optimizations are only possible by using these nested types.
   Position values are a generalization of pointers and memory addresses.
2. Numerous predefined template instances  
   Class templates share common interfaces which allow to flexibly combine various implementations.
   Numerous template instances are predefined to easily use the store, block, container and string interfaces.
3. Memory-based reference counters  
   Memory blocks can be associated with reference counters.
   The reference counter is associated with the memory block and not with its contents, e.g. a C++ object.
   Deleting a C++ object and releasing the corresponding memory are two distinct steps.
   The C++ object can be deleted by its owner, and the corresponding memory block can be released by the reference counter.
   In this way isolated islands in complex, reference counting based data structures can be avoided.
