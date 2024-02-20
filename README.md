# error handling library.

# installation

you may copy the files under errOr/include into your code or you may install into your system with the following: 

* ``py install.py``

and then include it with cmake into your project

```cmake
cmake_minimum_required_version(VERSION 3.25)

project(CoolProject LANGUAGES CXX)

find_package(errOr CONFIG REQUIRED)
add_executable(CoolProject source.cpp)
target_link_libraries(CoolProject PRIVATE errOr::errOr)
```

# examples
```c++
#include <errOr/ErrorOr.hpp>

#include <string>
#include <print>
#include <fstream>
#include <sstream>
#include <filesystem>

error::ErrorOr<std::string> read_file_contents(std::filesystem::path path)
{
    std::ifstream fileStream { path };

    if (fileStream.fail())
    {
        return error::make_error("Couldn't read file: \"{}\"", path.string());
    }

    std::stringstream contentStream {};
    contentStream << fileStream.rdbuf();
    
    return contentStream.str();
}

int main()
{
    auto const contents = MUST(read_file_contents("some_cool_file.txt"); // if the function fails, the program execution ends an error message is given.
    std::println("{}", contents);
}
```

```c++
#include <errOr/ErrorOr.hpp>

#include <string>
#include <print>
#include <fstream>
#include <sstream>
#include <filesystem>

error::ErrorOr<std::string> read_file_contents(std::filesystem::path path)
{
    std::ifstream fileStream { path };

    if (fileStream.fail())
    {
        return error::make_error("Couldn't read file: \"{}\"", path.string());
    }

    std::stringstream contentStream {};
    contentStream << fileStream.rdbuf();
    
    return contentStream.str();
}

error::ErrorOr<void> print_file_contents(std::filesystem::path path)
{
    auto const contents = TRY(read_file_contents("some_cool_file.txt")); // if the function fails, the error is pushed up into the call stack to be handled somewhere else.
    
    std::println("reading: {}", path.string());
    std::println("size in bytes: {}", contents.size());
    std::println("{}", contents);

    return {}; // this is how you return from void
}

int main()
{
    MUST(print_file_contents("some_cool_file.txt"));
}
```

for more, i recommend you to simply explore the code and see what you can do with it. seriously. do it.