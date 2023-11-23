/*
    lib/rust/C/clap.c
    Q@khaa.pk
 */

#include "clap.h"

/*
    Clap, Command Line Argument Processor
 */
 
EXPORT_IMPORT COMMANDLINES* __stdcall Clap (int argc, char* argv[], const char *command) 
{
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

    return (cArgc == INT_MAX ? 0 : (cArgc != 0 ? cArgc - 1 : 0));
} 