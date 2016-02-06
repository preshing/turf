Turf is a configurable C++ platform adapter. It defines a common API for:

* Thread creation
* Thread affinities
* Thread IDs
* Atomic operations
* Mutexes
* Condition variables
* Read-write locks
* Semaphores
* Events
* Timers
* Virtual memory
* Heap allocators
* Asserts

It then implements those things using POSIX, Win32, Mach, Linux, Boost, C++11 and possibly other platform APIs. You configure Turf to use the API you want.

Turf is used by [Junction](https://github.com/preshing/junction), a library of concurrent data structures in C++.

## License

Turf uses the Simplified BSD License. You can use the source code freely in any project, including commercial applications, as long as you give credit by publishing the contents of the `LICENSE` file in your documentation somewhere.

## Why Not Use C++11?

Junction was originally developed in C++11, but quickly ran into various limitations. Turf was written to address those limitations.

* Turf exposes affinities, integer thread IDs, semaphores and a few other things that C++11 does not.
* Turf is configurable, so you can compare implementations and even provide your own. The standard C++11 library is not configurable.
* Turf atomics have more flexible initialization, copy and cast rules than C++11 atomics.
* Turf can be used by developers who don't have access to a C++11 compiler.

## Adding Turf to Your Project

For now, refer to Junction's [README](https://github.com/preshing/junction) file for instructions.

## Feedback

If you have any comments or feedback, feel free to [open an issue](https://github.com/preshing/junction/issues) on GitHub, or send a direct message using the [contact form](http://preshing.com/contact/) on my blog.
