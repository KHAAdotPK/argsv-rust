- Imports necessary functions and a macro from the argsv crate.
```DIFF
+ use argsv::{start, find_arg, stop, help, help_line, common_argc, process_argument};
```
- Placeholder for actual command lines. Replace "..." with your specific command lines.
```DIFF
+ let command_lines = "..."; // Replace with your actual command lines
```
> For example...
```DIFF
let command_lines = "h -h help --help ? /? (Displays help screen)\n\
                     v -v version --version /v (Displays version number)\n\
                     t -t traverse --traverse /t (Traverses PNG file structure and displays it)\n\
                     d -d delete --delete /d (Deletes the named chunk from the PNG file)\n\
                     verbose --verbose (Displays detailed information or feedback about the execution of another command)";
```
> Boiler plate code to collect command line arguments into a vector of strings and converts command line arguments to C-style arguments for compatibility with the start function.
```DIFF
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
```DIFF
+ let head = start (argc, c_argv, command_lines);
```
> Checks for the presence of the help argument and prints help information if found.
```DIFF
+ let arg_help = find_arg (head, command_lines, "h");
+ if !arg_help.is_null() || argc < 1 {
+    help (head, command_lines);
+    stop (head); 
+    return;
+ }
```
- Stops argument processing.
```DIFF
+ stop (head); 
```