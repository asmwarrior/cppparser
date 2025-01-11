CppParser
=========

[![Build Status](https://github.com/satya-das/cppparser/actions/workflows/cmake-multi-platform.yml/badge.svg?event=push)](https://github.com/satya-das/cppparser/actions/workflows/cmake-multi-platform.yml)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/e2a1f6c5c8c149be816f1514ec491c98)](https://www.codacy.com/app/satya-das/cppparser?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=satya-das/cppparser&amp;utm_campaign=Badge_Grade)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

An easy, fast, and robust library to parse C/C++ source.

## Features
- No pre-processing, and preprocessors are part of the ast.
- Most comments are preserved too.
- Developed from scratch and uses back-tracking yacc (BtYacc) to write C++ grammer, that means **no dependency on libclang**.
- The result of parsing is an AST where elements of a file are arranged in a tree.
- Minimum dependency. Only external dependency is a [lexer](https://github.com/westes/flex) which is by default available on unix like platforms (Linux, Mac, etc.) and [Flex for Windows](http://gnuwin32.sourceforge.net/packages/flex.htm) is bundled with the project so it works out of the box on all Windows platforms too.
- Parsing of multi-file program is supported too.

## Motivation 
CppParser can be used to build tools that need parsing of C/C++ files.
I am using it to develop [cib](https://github.com/satya-das/cib/) which implements ABI stable SDK architecture for C++ library.

## Example

To begin with we will see an example of parsing a hello-world program and see what is the AST that `CppParser` creates:
```c++
#include <iostream>

int main()
{
  std::cout << "Hello, World!\n";

  return 0;
}

```

For the above hello-world program we can expect that when it is parsed the generated AST should look like following:
![AST for Hello World program](https://github.com/satya-das/cppparser/blob/master/cppparser/src/readme-assets/HelloWorldAST.svg "AST for Hello World program")

So, how we are going to access these elements of AST using `CppParser`?
Below is the program written as unit-test for validating the correctness of generated AST:

```c++
#include <catch/catch.hpp>

#include "cppparser/cppparser.h"

#include <filesystem>

namespace fs = std::filesystem;

TEST_CASE("Parsing hello world program")
{
  cppparser::CppParser parser;
  const auto           testFilePath = fs::path(__FILE__).parent_path() / "test-files/hello-world.cpp";
  const auto           ast          = parser.parseFile(testFilePath.string());
  REQUIRE(ast);

  const auto members = GetAllOwnedEntities(*ast);
  REQUIRE(members.size() == 2);

  const cppast::CppConstPreprocessorIncludeEPtr hashInclude = members[0];
  REQUIRE(hashInclude);
  CHECK(hashInclude->name() == "<iostream>");

  cppast::CppConstFunctionEPtr func = members[1];
  REQUIRE(func);
  CHECK(func->name() == "main");

  REQUIRE(func->defn());
  const auto mainBodyMembers = GetAllOwnedEntities(*func->defn());
  REQUIRE(mainBodyMembers.size() == 2);

  cppast::CppConstBinomialExprEPtr coutHelloWorld = mainBodyMembers[0];
  REQUIRE(coutHelloWorld);
  CHECK(coutHelloWorld->oper() == cppast::CppBinaryOperator::INSERTION);

  cppast::CppConstNameExprEPtr coutOperand1 = &(coutHelloWorld->term1());
  REQUIRE(coutOperand1);
  CHECK(coutOperand1->value() == "std::cout");

  cppast::CppConstStringLiteralExprEPtr coutOperand2 = &(coutHelloWorld->term2());
  REQUIRE(coutOperand2);
  CHECK(coutOperand2->value() == "\"Hello, World!\\n\"");
}

```

**This example is a real one and is part of actual unit test of CppParser**.

For AST traversing, see the [CppWriter](cppwriter), that uses the generated AST to create files.

## Building CppParser

### Get the source

```sh
git clone https://github.com/satya-das/common.git
git clone https://github.com/satya-das/CppParser.git
```

## Configure and build

```sh
cd cppparser
mkdir builds
cd builds
cmake ..
make && make test
```

*Alternatively, if you prefer `Ninja` instead of `make`*:

```sh
cd cppparser
mkdir builds
cd builds
cmake -G Ninja ..
ninja && ninja test
```

