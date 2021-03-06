## C++ metaprogramming: evolution and future directions @ [Meeting C++][] 2016

This repository contains my [reveal.js][]-based presentation on metaprogramming
for Meeting C++ 2016.

## Basic usage
Go to https://ldionne.com/meetingcpp-2016 or open `index.html` with your browser.

## Advanced usage
From the root of the repository,
```sh
npm install
grunt serve &
```

and then connect to `localhost:8000` to view locally.

## Building the code samples

```sh
(mkdir build && cd build && cmake ..)
cmake --build build install-dependencies
cmake --build build
```

Note that if you want the reflection example to work, you should use

```sh
export LLVM_ROOT=path/to/llvm/tree
cmake .. -DCMAKE_CXX_COMPILER=path/to/custom/clang \
         -DCMAKE_CXX_FLAGS="-I${LLVM_ROOT}/tools/clang/reflection -Xclang -freflection"
```

<!-- Links -->
[Meeting C++]: http://meetingcpp.com
[reveal.js]: https://github.com/hakimel/reveal.js
