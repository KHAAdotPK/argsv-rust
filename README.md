# argsv-rust

#### argsv for Rust: Simplifying Command Line Argument Processing with Elegance

In the words of the C/C++ Users Journal from November 2004, "Command-line options represent the 'face' of the program, and tools for options parsing should be convenient and easy to use."

Welcome to `argsv-rust` - an earnest endeavor to process command line arguments with both ease and style.

## Key Features

- **Versatile Command Handling:** `argsv-rust` excels in managing both short and long commands, providing a flexible and user-friendly interface.

- **Handling Multiple Instances:** It seamlessly handles scenarios where multiple instances of the same short or long command are encountered, ensuring robust command line parsing.

- **Command Name Variations:** `argsv-rust` supports a variety of command name variations. For instance, a single command can be invoked using different names such as "?", "/?", "h", "-h", "help", "--help". The flexibility allows users to choose the naming convention that suits them best.

- **Help Text Processing:** Beyond basic parsing, `argsv-rust` goes a step further by enabling the processing and parsing of the help text associated with each command line option. This enhances the overall clarity and usability of your command line interface.

## Example Application

To expedite your understanding of how to harness the power of `argsv-rust`, an example application is included in this repository. Take a moment to explore and dissect this practical illustration, which serves as a guide on effectively working with `argsv-rust` for streamlined command line argument processing.

Your journey into the realm of elegant and efficient command line argument processing begins with `argsv-rust`. Embrace simplicity without compromising on style.

## Dependencies

Ensure you have the following dependencies cloned into the `./lib` directory before building `argsv-rust`:

- [parser](https://github.com/KHAAdotPK/parser.git)
- [string](https://github.com/KHAAdotPK/string.git)
- [csv](https://github.com/KHAAdotPK/csv.git)
- [ala_exception](https://github.com/KHAAdotPK/ala_exception.git)

## Build Instructions

Follow these steps to build `argsv-rust`:

1. **Clone Dependencies:** Clone all the dependencies mentioned above into the `./lib` directory.

    ```bash
    git clone https://github.com/KHAAdotPK/parser.git ./lib/parser
    git clone https://github.com/KHAAdotPK/string.git ./lib/string
    git clone https://github.com/KHAAdotPK/csv.git ./lib/csv
    git clone https://github.com/KHAAdotPK/ala_exception.git ./lib/ala_exception
    ```

2. **CMake Build:** Execute CMake on the `CMakeLists.txt` file to build the required libraries (`clap.lib`, `clap.dll`, `start.lib`, and `start.dll`). 

    ```bash
    cmake ./CMakeLists.txt
    ```

3. **Copy Libraries:** Copy the newly built files (`clap.lib`, `clap.dll`, `start.lib`, and `start.dll`) into the root directory (`./`).

4. **Cargo Build:** Finally, execute `cargo build` to build `argsv-rust`. 

    ```bash
    cargo build
    ```

Please note that the build process is still a work in progress and will be continuously improved for a more seamless experience.

Feel free to contribute and report issues, making `argsv-rust` a robust and reliable choice for your command line argument processing needs. Thank you for using `argsv-rust`!