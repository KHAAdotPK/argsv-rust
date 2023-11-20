/*
    lib/rust/argsv/lib.rs
    Q@khaa.pk
 */

#[repr(C)]
pub struct PCLA {
    l: i32,         // Line number
    t: i32,         // Token number in a line
    i: i32,         // Index into argv
    argc: i32,      // Number of arguments for this command line argument
    
    next: *mut PCLA,    
    prev: *mut PCLA, 
}

impl PCLA {

    pub fn get_line_number (&self) -> i32 {

        return self.l;
    }

    pub fn get_token_number (&self) -> i32 {

        return self.t;
    }

    pub fn get_index_number (&self) -> i32 {

        return self.i;
    }

    pub fn get_argc (&self) -> i32 {

        return self.argc;
    }
    
    pub fn get_next (&self) -> *mut PCLA {

        return self.next;
    }

    pub fn get_prev (&self) -> *mut PCLA {

        return self.prev;
    }

}

#[repr(C)]
pub struct COMMANDLINES {
    n_tokens: i32, // Number of tokens in this line
    help_str_position: usize, // The offset where the help string (the text itself) begins.
    help_str_size: usize, // The size of help string.
    n_claps: i32, // Number instances of this command (the number of times the user has given this command)

    clap: *mut PCLA, // The doubly linked list of diffrent instances of same one command.
                     // The number of links are given in n_claps.

    pub next: *mut COMMANDLINES, // Next command line.
    prev: *mut COMMANDLINES, // Previous command line.
}

impl COMMANDLINES {

    pub fn get_n_tokens (&self) -> i32 {

        return self.n_tokens;
    }

    pub fn get_n_claps (&self) -> i32 {

        return self.n_claps;
    }

    pub fn get_help_str_position (&self) -> usize {

        return self.help_str_position;
    }

    pub fn get_help_str_size (&self) -> usize {

        return self.help_str_size;
    }

    pub fn get_next (&self) -> *mut COMMANDLINES {

        return self.next;
    }

    pub fn get_prev (&self) -> *mut COMMANDLINES {

        return self.prev;
    }

    pub fn get_clap (&self) -> *mut PCLA {

        return self.clap;
    }

}

#[link(name = "clap", kind = "dylib")]
extern "stdcall" { 

    // Forign functions. Declare the C/C++ functions you want to call here.    
    fn Clap(argc: std::os::raw::c_int, argv: *const *const std::os::raw::c_char, arg: *const std::os::raw::c_char) -> *mut COMMANDLINES;
    fn Stop(head: *mut COMMANDLINES);

    fn Find (head: *mut COMMANDLINES, command: *const std::os::raw::c_char, arg: *const std::os::raw::c_char) -> *mut COMMANDLINES;
    fn Help (head: *mut COMMANDLINES, command: *const std::os::raw::c_char);
    fn HelpLine (head: *mut COMMANDLINES,command: *const std::os::raw::c_char, arg: *const std::os::raw::c_char);
    fn Common (head: *mut COMMANDLINES) -> i32;
}

/*
    @argc, same as C/C++ argc
    @argv, same as C/C++ argv
    @command, user provided command line text for all commands
    
    It returns A linked list of instance of "struct COMMANDLINES" and start is intended to interface with a C library
 */
pub fn start (argc: std::os::raw::c_int, argv: Vec<*const std::os::raw::c_char>, command: &str) -> *mut COMMANDLINES {

    // Convert the user-provided command string to a C-style string (CString)
    let c_command = std::ffi::CString::new(command).expect("CString::new failed");

    // Call an unsafe function named Clap, passing argc, argv, and the C-style command string
    let lines: *mut COMMANDLINES = unsafe {
                   
        Clap (argc as std::os::raw::c_int, argv.as_ptr(), c_command.as_ptr())        
    };

    // Return the result (a pointer to a linked list of COMMANDLINES instances)    
    return lines;    
}

/*
    @head, pointer to linked list of all commands

    This function stops the process of argument processing by destrying the linked list.
    The function does not return a value, primary purpose is to perform a side effect (stopping the argument processing by destroying the linked list)
 */
pub fn stop (head: *mut COMMANDLINES) {

    // Call an unsafe function named Stop, passing the pointer to the linked list
    unsafe { Stop (head); };
}

/*
    @head, pointer to linked list of all commands
    @command, user provided command line text for all commands
    @arg, the single command to find

    This function traverses the linked list pointed to by the head and returns the instance of  "struct COMMANDLINES"
 */
pub fn find_arg (head: *mut COMMANDLINES, command: &str, arg: &str) -> *mut COMMANDLINES {

    // Convert user-provided command and arg strings to C-style strings (CString)
    let c_command = std::ffi::CString::new(command).expect("CString::new failed");
    let c_arg = std::ffi::CString::new(arg).expect("CString::new failed");

    // Call an unsafe function named Find, passing the head pointer, and C-style command and arg strings
    return unsafe { Find (head, c_command.as_ptr(), c_arg.as_ptr())  };
}

/*
    @head, pointer to linked list of all commands
    @command, user provided command line text

    Emitts help text for all commands  
 */
pub fn help (head: *mut COMMANDLINES, command: &str) {

    // Convert user-provided command string to a C-style string (CString)
    let c_command = std::ffi::CString::new(command).expect("CString::new failed");

    // Call an unsafe function named Help, passing the head pointer and C-style command string
    unsafe { Help (head, c_command.as_ptr()) };
}

/*
    @head, pointer to linked list of all commands
    @command, user provided command line text for all commands
    @arg, the single command to find help for

    Emitts help text for only one command 
 */
pub fn help_line (head: *mut COMMANDLINES, command: &str, arg: &str) {

    let c_command = std::ffi::CString::new(command).expect("CString::new failed");
    let c_arg = std::ffi::CString::new(arg).expect("CString::new failed");

    unsafe { HelpLine (head, c_command.as_ptr(), c_arg.as_ptr())  };
}

/*
    @head, pointer to linked list of all commands

    It returns the number of common arguments to all the user given commands
    In  C/C++ it means all arguments starting at index 1 to index "returned value" 
 */
pub fn common_argc (head: *mut COMMANDLINES) -> i32 {

    return unsafe { Common (head) };
}

/*
    @ $closure_as_func: The closure to be executed to process the respective command line argument.
    @ $head: *mut to an instance of COMMANDLINES returned by argsv::start().
    @ $command: An instance of type str, raw text out of which COMMANDLINES instance is created.
    @ $arg: *mut to an instance of COMMANDLINES, a single line returned by argsv::find_arg().
    @ $args: Equivalent to argv[].
    @ $rvr: Mutable reference to an instance used to return a value.
            TODO: Make the closure not return any value and actually use $rvr to return the value.
                  In that case, $rvr can have any type, not like now when it has to have the same type as the closure's return type.
    @ $cla: Single token to search. In this macro, it is given to argsv::help_line() to get the help text for the type of command.
            'cla' is short for "command line argument".
    @ $verbose: Boolean type. If true, makes the macro more verbose.
 */

/*
    @ $closure_as_func, the closure gets executed to process the repective cla.
    @ $head, *mut to instance of COMMANDLINES which is returned by argsv::start()
    @ $command, it is an instance of type str raw text out of which instance of COMMANDLINES get created
    @ $arg, *mut to instance of COMMANDLINES, instead of all lines it is *mut to one single line. It is returned by argsv::find_arg()
    @ $args, equivalent to argv[]
    @ $rvr, mutable reference to an instance used to return value.
            TODO, make the closure not return any value and actually use the @ $rvr to return the value.
                  In that case the @ $rvr can have any type not like now when it has to have the same type as the return type of the closure.
    @ $cla, single token to search. Here in this macro, it is given to argsv::help_line() to get the help text for type of command.
            cla is short for "command line argument". 
    @ $verbose, boolean type. If true makes the macro more verbose.                  
 */
#[macro_export]
macro_rules! process_argument {
    ($closure_as_func:expr, $head:expr, $command:expr, $arg:expr, $args:expr, $rvr:expr, $cla:expr, $verbose:expr) => {
         
        if !$arg.is_null() {

            let mut claps = unsafe { &*((&*$arg).get_clap()) };

            loop {

                if claps.get_argc() == 0 {
                
                    if common_argc ($head) == 0 {
    
                        help_line ($head, $command, $cla);
                                            
                        stop ($head);
                        
                        return;
                    } 
    
                    // Process cla with common args
                    for i in 1 ..=common_argc ($head) {
                        
                        *$rvr = $closure_as_func(&$args[i as usize], $rvr, $verbose);
                    }
                        
                } else {
    
                    // Process cla with clas argc
                    for i in 1 ..=claps.get_argc() {

                        *$rvr = $closure_as_func(&$args[(claps.get_index_number() + i) as usize], $rvr, $verbose);
                    }    
                }
                
                if (claps.get_next()).is_null() {
    
                    break;                      
                }
                    
                claps = unsafe { &*claps.get_next() };
            } 
        }
    };
}

