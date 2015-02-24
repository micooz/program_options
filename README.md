# CommandLine Parser

CommandLine Parser can be used for parsing command line.

It stores options and values into a map.

It's easy to check whether an option exist, and get value with a specified type.

# Usage

1. Include the header 'commandline_parser.h'.
2. Declare an object associate with CParser.
3. Call CParser::parse() manually.
4. Verify an option by calling has(),has_or(),has_and().
5. To get a value from specified option by calling get() and then as<T>().

```cpp
// $ path/to/program -xyz 3.14 --option value
CParser parser;
parser.parse(argc, argv);

// to get a value from an option
parser.get("z")->as<double>();
parser.get("option")->val(); // std::string

// verify an option
parser.has("option")

// verify a sequence of options
parser.has_or(2, "x", "y");
parser.has_and(2, "option", "c");
parser.has_and(1, "-xy");
```

For more details and examples, see test/test.cc

# TODOs

just like the 'program_options' of boost library, we need a 'help' generator to generate help information without typing it manually. In addition, better solution to verify keys in options.

For example:

```cpp
Generator gen;
char *help = gen.make("Usage for xxx")
("h,help", "description to the option")
("o,option", a default value, "description");

cout << help << endl;
```

then we get:

```
Usage for xxx:
-h [ --help ] description to the option
-o [ --option ] arg=default description
```