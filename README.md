# simple-threads
![flow](https://cdn.searchenginejournal.com/wp-content/uploads/2021/09/leveraging-flow-for-marketers-amplify-your-productivity-6131ec1d805a8-sej-1520x800.jpg)

## Introduction 

**simple-threads** is a library for convenient and simple thread management.  The main class is **ThreadManager**.  It contains a pool of threads and a priority queue of tasks.  Tasks can be anything: lambda expressions, functions, methods, functors, and so on.

## TO-DO list

- [x] Priority
- [ ] Waiting or completing tasks (optional) when ThreadManger shutdown
- [ ] Abstract class Thread for infinite loops
- [ ] Class Worker that contains and manages std::thread
- [ ] Ability to start tasks after a while
- [ ] Add CI/CD pipeline 

## Build

> To build, you need the latest versions of Git, CMake, Ninja, and any compiler of your choice (MinGW, GCC, etc.)

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

> ==Warning==: The Debug preset generates an executable for examples.
> 	
> If you don't want this, add the 
> `-DSTH_BUILD_EXAMPLES=OFF` flag to the CMake file generation step.
> 	
> The docs preset does NOT build executable files, only documentation files.

## Contributing

Please read ['CONTRIBUTING.md'](https://github.com/N3WC0DER/simple-threads/blob/master/CONTRIBUTING.md) for details on how to contribute to this project.


Happy coding!
