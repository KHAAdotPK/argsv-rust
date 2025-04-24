# argsv-rust

#### argsv for Rust: Simplifying Command Line Argument Processing with Elegance

In the words of the C/C++ Users Journal from November 2004, "Command-line options represent the 'face' of the program, and tools for options parsing should be convenient and easy to use."

Welcome to `argsv-rust` - an earnest endeavor to process command line arguments with both ease and style.

## Key Features

- **Versatile Command Handling:** `argsv-rust` excels in managing both short and long commands, providing a flexible and user-friendly interface.

- **Handling Multiple Instances:** It seamlessly handles scenarios where multiple instances of the same short or long command are encountered, ensuring robust command line parsing.

- **Command Name Variations:** `argsv-rust` supports a variety of command name variations. For instance, a single command can be invoked using different names such as "?", "/?", "h", "-h", "help", "--help". The flexibility allows users to choose the naming convention that suits them best.

- **Help Text Processing:** Beyond basic parsing, `argsv-rust` goes a step further by enabling the processing and parsing of the help text associated with each command line option. This enhances the overall clarity and usability of your command line interface.

## Example Application (Under Development)

To expedite your understanding of how to harness the power of argsv-rust, an example application is currently under development and will soon be available in this repository. In the meantime, users can follow the guidelines provided in the [USAGE.md](./USAGE.md) document for effective utilization of argsv-rust in streamlined command line argument processing.

Your journey into the realm of elegant and efficient command line argument processing begins with argsv-rust. Embrace simplicity without compromising on style.

## Dependencies

Ensure you have the following dependencies cloned into the `./argsv-rust/lib` directory before building `argsv-rust`:

- [parser](https://github.com/KHAAdotPK/parser.git)
- [string](https://github.com/KHAAdotPK/string.git)
- [csv](https://github.com/KHAAdotPK/csv.git)
- [ala_exception](https://github.com/KHAAdotPK/ala_exception.git)

## Build Instructions

The `argsv-rust` crate uses a custom `build.rs` script that compiles the required native C/C++ code automatically when it is used as a dependency in another Rust project. To build it:

1. **Clone Dependencies:** Clone all the required C/C++ libraries into the `./argsv-rust/lib` directory of this crate.

    ```bash
    cd argsv-rust/lib

    git clone https://github.com/KHAAdotPK/parser.git
    git clone https://github.com/KHAAdotPK/string.git
    git clone https://github.com/KHAAdotPK/csv.git
    git clone https://github.com/KHAAdotPK/ala_exception.git
    ```
To simplify the cloning process, a helper script is provided:

### Quick Setup

Navigate to the `argsv-rust/lib` directory and run the `PULL.cmd` script. This script will automatically clone all required repositories into the correct location.

```bash
cd argsv-rust/lib
PULL.cmd
```

This step is required only once, or whenever you need to update the dependencies.   

2. **Use as Dependency:** Add `argsv-rust` as a dependency in the `Cargo.toml` file of your Rust project.

    ```toml
    [dependencies]
    argsv-rust = { path = "./path/to/argsv-rust" }
    ```

3. **Build the Project:** Now, simply run `cargo build` **from the root of your project** (the one using `argsv-rust` as a dependency):

    ```bash
    cargo build
    ```

    This will:
    - Compile the native libraries inside `argsv-rust` using `build.rs`
    - Place the resulting `.dll` and `.lib` files in the directory where your `Cargo.toml` is of this crate is located 
    - Complete the full build automatically

You no longer need to manually run CMake or copy library files — the process is now fully automated.

## License

This project is governed by a license, the details of which can be located in the accompanying file named 'LICENSE.' Please refer to this file for comprehensive information.
