# simple-threads

<div align="center">

![flow](https://cdn.searchenginejournal.com/wp-content/uploads/2021/09/leveraging-flow-for-marketers-amplify-your-productivity-6131ec1d805a8-sej-1520x800.jpg)

![](https://badgen.net/github/tag/N3WC0DER/simple-threads)
![](https://badgen.net/github/license/N3WC0DER/simple-threads)
![](https://badgen.net/github/stars/N3WC0DER/simple-threads)

![](https://badgen.net/github/checks/N3WC0DER/simple-threads/master)

</div>

## Introduction 

**simple-threads** is a library for convenient and simple thread management.  The main class is **ThreadManager**.  It contains a pool of threads and a priority queue of tasks.  Tasks can be anything: lambda expressions, functions, methods, functors, and so on.

## TO-DO list

- [x] Priority
- [ ] Add CI/CD pipeline
- [ ] Waiting or completing tasks (optional) when ThreadManger shutdown
- [ ] Abstract class Thread for infinite loops
- [ ] Class Worker that contains and manages std::thread
- [ ] Ability to start tasks after a while

## Build

> To build, you need the latest versions of Git, CMake version, at least 3.6.0, Ninja, and any compiler of your choice (MinGW, GCC, etc.) supporting the C++17 standard.

```shell
git clone https://github.com/N3WC0DER/simple-threads.git
cd simple-threads
```

There are several presets to choose from:
- debug
- release
- docs (for documentation)

```shell
cmake --preset <preset name>
ninja -C ./cmake/<preset name>/
```

The library file will be located along the path `./cmake/<preset name>/libsimple-threads.a`

> **Warning**: The Debug preset generates an executable for examples.
> 	
> If you don't want this, add the 
> `-DSTH_BUILD_EXAMPLES=OFF` flag to the CMake file generation step.
> 	
> The docs preset does NOT build executable files, only documentation files.

## Examples

The **ThreadManager** class is a **Singleton**, so its single instance can be accessed from anywhere in the program. But before using this class, it must be initialized. To do this, simply call the static `init()` method from the main thread. The parameters must be passed the number of threads that the **ThreadManager** can use.

```c++
ThreadManager::init(); // Initializes the class with the default number of threads (std::thread::hardware_concurrency())
ThreadManager::init(4); // Initializes a class with four threads

auto* tmanager = ThreadManager::get_instance(); // Getting an Instance
```

Call the `add_task()` method to add a task to the queue. The first parameter passes the priority, the second is a reference to the function, and then the parameters of this function.

```c++
// ...
void any_function() {
	// ...
}

int main() {
	// ...
	auto future = tmanager->add_task(sth::Priority::HIGHEST, &any_function);
}
```

> All kinds of functions that you can use as tasks can be viewed in the file `examples/main.cpp`

The `add_task()` method returns an instance of `std::future` that is associated with the task. With its help, you can get the result that the task function returns or wait for the task to be completed using the blocking `get()` method.

```c++
// ...
int return_five() {
	return 5;
}

int main() {
	// ...
	auto future = tmanager->add_task(sth::Priority::HIGHEST, &return_five);
	std::cout << future.get() << std::endl // Output: 5
}
```

The `wait_all()` method is blocking. It is waiting for the completion of all tasks that are currently in the queue.

```c++
// ...
auto wait = [] () {
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Freezes the execution of the current thread for 1 second
};

auto future = tmanager->add_task(sth::Priority::HIGHEST, wait);
auto future = tmanager->add_task(sth::Priority::HIGHEST, wait);
auto future = tmanager->add_task(sth::Priority::HIGHEST, wait);
auto future = tmanager->add_task(sth::Priority::HIGHEST, wait);

tmanager->wait_all();
```

Depending on the current threads, these tasks will be performed at different times. For 4 or more streams - 1 second, for 2 - 2 seconds, for 1 thread - 4 seconds.

Finally, the static `free()` function deletes the ThreadManager instance and clears the task queue. You can also call the `clear_queue()` method at any time and clear the task queue.

```c++
// ...
tmanager->wait_all(); // To wait for a task

ThreadManager::free(); // For deleting instance
```

## Tests

To run tests, you first need to build the project using the release preset, then run them:

```shell
cmake --preset release
ninja -C cmake/release

ctest --preset tests
```

## Contributing

Please read [`CONTRIBUTING.md`](https://github.com/N3WC0DER/simple-threads/blob/master/CONTRIBUTING.md) for details on how to contribute to this project.

Happy coding!
