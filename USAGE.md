---
###### This commit adds an initial version of the USAGE.md file, providing basic information on how to use the crate. Please note that the usage details are not yet complete, and a more comprehensive example will be added in future commits.
---
## Usage Guide for argsv-rust
- Imports necessary functions and a macro from the argsv crate.
```rust
+ use argsv::{start, find_arg, stop, help, help_line, common_argc, process_argument};
```
- Placeholder for actual command lines. Replace "..." with your specific command lines.
```rust
+ let command_lines = "..."; // Replace with your actual command lines
```
> For example...
```rust
let command_lines = "h -h help --help ? /? (Displays help screen)\n\
                     v -v version --version /v (Displays version number)\n\
                     t -t traverse --traverse /t (Traverses PNG file structure and displays it)\n\
                     d -d delete --delete /d (Deletes the named chunk from the PNG file)\n\
                     verbose --verbose (Displays detailed information or feedback about the execution of another command)";
```
> Boiler plate code to collect command line arguments into a vector of strings and converts command line arguments to C-style arguments for compatibility with the start function.
```rust
// Get the command-line arguments as an iterator
+ let args: Vec<String> = std::env::args().collect();
// Create a Vec<CString> from the Vec<String>
+ let c_args: Vec<std::ffi::CString> = args.iter().map(|s| std::ffi::CString::new(s.as_str()).expect("Failed to create CString")).collect();
// Get the equivalent of argv in C. Create a Vec<*const c_char> from the Vec<CString>.
+ let c_argv: Vec<*const std::os::raw::c_char> = c_args.iter().map(|s| s.as_ptr()).collect();
// Get the C equivalent of argc.    
+ let argc: i32 = c_args.len() as std::os::raw::c_int;
```
- Initiates argument processing using the start function.
```rust
+ let head = start (argc, c_argv, command_lines);
```
> Get the number of common arguments. When there are no common arguments then it returns 1 (for the name of the program) 
```rust
println!("COMMON_ARGC = {}", common_argc(head));
```
> Checks for the presence of the help argument and prints help information if found.
```rust
+ let arg_help = find_arg (head, command_lines, "h");
+ if !arg_help.is_null() || argc < 1 {
+    help (head, command_lines);
+    stop (head); 
+    return;
+ }
```
- Stops argument processing.
```rust
+ stop (head); 
```
### `argsv::process_argument` Macro
This crate introduces the process_argument macro, offering a versatile mechanism for handling command line arguments. The macro allows users to provide a closure expression, denoted by $closure_as_func, to specify the implementation details for processing a particular command line argument.
### Macro Parameters:
- `$closure_as_func`: The closure to be executed to process the respective command line argument.
- `$head`: A mutable pointer to an instance of COMMANDLINES returned by `argsv::start()`.
- `$command`: An instance of type `str`, raw text from which a COMMANDLINES instance is created.
- `$arg`: A mutable pointer to an instance of COMMANDLINES, representing a single line returned by `argsv::find_arg()`.
- `$args`: Equivalent to `argv[]` in C/C++.
- `$rvr`: A mutable reference to an instance used to return a value. This reference can be utilized to return values from the closure. Note: Consider adjusting the macro to make the closure not return any value and use `$rvr` exclusively for returning values.
- `$cla`: A single token used to search. In this macro, it is supplied to `argsv::help_line()` to obtain the help text for the type of command. Here, 'cla' stands for "command line argument."
- `$verbose`: A boolean type. If true, makes the macro more verbose.
### `Example` usage(how to use `process_argument` macro)
```rust
/*
    Start argument collection and processing.
 */
+ let head = start (argc, c_argv, command_lines);
+ let arg_traverse: *mut argsv::COMMANDLINES = find_arg (head, command_lines, "t");
/*
    Rust macros don't allow you to directly pass a function as an argument. However, you can pass function-like items like closures as arguments. 
    Closures can be used in place of functions in many scenarios.
 */
+ let mut png: Png = Png::new(Vec::new());
+ let arg_traverse_closure = |name: &str, returned_value_reference: &mut Png, verbose: bool| arg_traverse_closure_func(name, returned_value_reference, verbose);    
+ process_argument!(arg_traverse_closure, head, command_lines, arg_traverse, args,  &mut png, "/t", !arg_verbose.is_null());
/*
    Stop argument processing.
 */
+ stop (head);
```
### `Example` of user provided `closure`(how to write a `closure` to process a single command line option, here that comamnd is "traverse"):
```rust
pub fn arg_traverse_closure_func(file_name: &str, _returned_value_reference: &mut Png, verbose: bool) -> Png {
    println!("Traversing file {}.", file_name);
    let content = read_write::read(file_name);
    if verbose {
        println!("File size: {} bytes", content.get_content_length());
    }
    let png = Png::new(content.get_content_vec());
    println!("Number of chunks = {}", png.chunks.len());
    let mut i: usize = 0;
    let mut iter = png.chunks.iter();
    loop {
        if !(i < png.chunks.len()) {
            break;
        }
        let chunk: &Chunk = iter.next().unwrap();
        println! ("Length = {}",  unsafe { big_endian_read_u32 (chunk.length.clone().as_mut_ptr()) });        
        println! ("Type = [ {} {} {} {} ], {}", chunk.type_name[0], chunk.type_name[1], chunk.type_name[2], chunk.type_name[3], str::from_utf8(&chunk.type_name).unwrap());
        i = i + 1;        
    }
    return png;
}
```