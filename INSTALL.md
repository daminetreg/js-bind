# Installing cpp-js::js-bind
js-bind can be installed in three ways :

* [Copy paste in your project](#copy-paste-in-your-project)
* [Install](#install)
* [Install without dependencies](#install-without-dependencies)

## Copy paste in your project
1. Copy this folder in your project
2. Add compile the following way :

```
emcc --bind -I js-bind/ \
-I js-bind/deps/boostorg/preprocessor/include/ \
-I js-bind/deps/cpp-pre/type_traits/ \
```

## Install
Install the library
```sh
mkdir build/ && cd build/
cmake ..
make install
```

You can tweak destination with `-DCMAKE_INSTALL_PREFIX=/install/path` to change the default `/usr/local`.

## If you use CMake :
```cmake
find_package(js-bind REQUIRED)
add_executable(app your_code.cpp)
target_link_libraries(app cpp-js::js-bind)
```

Don't forget to add the toolchain file for emscripten with : 
```sh
source emsdk_portable/emsdk_env.sh 
-DCMAKE_TOOLCHAIN_FILE=${EMSCRIPTEN}/cmake/Modules/Platform/Emscripten.cmake 
```

## If you don't use CMake : 
Add to your include path : 
```
INSTALL_PREFIX/include/js-bind/deps/
INSTALL_PREFIX/include/
```

## Install without dependencies
Install the library
```sh
mkdir build/ && cd build/
cmake .. -DINGLUED=OFF
make install
```

This library was built with [`#inglued <>`](https://github.com/header-only/inglued) therefore it forwards dependencies with it, but you might prefer the use of your own deps, therefore simply disable them via `INGLUED=OFF`.

