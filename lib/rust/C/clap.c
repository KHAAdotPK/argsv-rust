/*
    lib/rust/C/clap.c
    Q@khaa.pk
 */

#include "clap.h"

/*
    Clap, Command Line Argument Processor
 */

/*
    Variable to store the number of arguments
 */
/*
 * BUG: Global state dependency
 * 
 * This static variable creates a hidden dependency between Clap() and Common()
 * functions. Clap() sets this value to argc, and Common() uses it as a fallback
 * when no arguments are found in the command line list.
 * 
 * Issues:
 * 1. If Common() is called before Clap(), the value will be incorrect (0)
 * 2. Multiple calls to Clap() with different argc values will overwrite previous values
 * 3. Thread safety concerns with shared global state
 * 
 * Future fix: Store argc in the COMMANDLINES structure instead of using global state
 */ 
static int n = 0;
 
EXPORT_IMPORT COMMANDLINES* __stdcall Clap (int argc, char* argv[], const char *command) 
{
    n = argc;

    COMMANDLINES *head = StartClapping(argc, argv, command);
        
    return head;
}

EXPORT_IMPORT void __stdcall Stop (COMMANDLINES *head)
{
    StopClapping(head);     
}

EXPORT_IMPORT COMMANDLINES* Find (COMMANDLINES *head, const char *command, const char *arg) 
{    
    return FindArg (head, command, arg);
}

EXPORT_IMPORT void Help (COMMANDLINES *head, const char *command) 
{
    HelpTemplate (head, command);
}

EXPORT_IMPORT void HelpLine (COMMANDLINES *head, const char *command, const char *arg) 
{
    HelpLineTemplate (head, command, arg);
}

/**
 * Common - Finds the minimum argument index across all command line arguments
 * 
 * This function traverses the entire command line structure to find the 
 * smallest index value among all parsed arguments. This represents the
 * position of the first recognized argument in the original argv array.
 *
 * The function handles several cases:
 * 1. When specific arguments are found in the linked list, returns the lowest index
 * 2. When no specific arguments are found but command line arguments exist, 
 *    returns the total argument count (n)
 * 3. When no arguments exist at all, returns n (which would be 0 if no args)
 *
 * Previous implementation:
 * The function originally returned 0 for empty lists, and (min_index - 1) for non-empty
 * lists as seen in the commented code:
 * return (cArgc == INT_MAX ? 0 : (cArgc != 0 ? cArgc - 1 : 0));
 * 
 * Current implementation:
 * Now it uses the global 'n' variable (which stores argc from Clap function) as a
 * fallback when no matching arguments are found, providing more useful information
 * about the actual argument count rather than returning an arbitrary value.
 *
 * @param head Pointer to the start of the COMMANDLINES linked list
 * @return The minimum argument index found, or the total argument count if no specific arguments found
 */
EXPORT_IMPORT int __stdcall Common (COMMANDLINES *head) 
{
    int cArgc = INT_MAX;
    COMMANDLINES *line = head;
        
    while (line != NULL)
    {
        PCLA *clap = line->clap;

        while (clap != NULL)
        {
            if (cArgc > clap->i)
            {
                cArgc = clap->i;
            }

            clap = clap->next;
        }

        line = line->next;
    }
    
    if (cArgc == INT_MAX)
    {
        cArgc = n;
    }

    //return (cArgc == INT_MAX ? 0 : (cArgc != 0 ? cArgc - 1 : 0));

    return cArgc;
}