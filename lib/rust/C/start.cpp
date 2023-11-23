/*
    lib/rust/C/start.cpp
    Q@khaa.pk
 */

#include "start.hh"

EXPORT_IMPORT void __stdcall BubbleAfter (int argc, char* argv[], const char *command, COMMANDLINES* head)
{
    COMMANDLINES *lines = head;

    while (lines != NULL)
    {
        PCLA *claps = lines->clap;

        while (claps != NULL)
        {
            cc_tokenizer::csv_parser<cc_tokenizer::String<char>, char> commandParser( command );

            while (commandParser.go_to_next_line() != cc_tokenizer::string_character_traits<char>::eof())
            {                
                while (commandParser.go_to_next_token() != cc_tokenizer::string_character_traits<char>::eof())
                {
                    for (int i = claps->i + 1; i < argc; i++)
                    {
                        if (commandParser.get_current_token().compare(cc_tokenizer::String<char>(argv[i])) == 0)
                        {                     
                            if ((i - claps->i - 1) < claps->argc)
                            {
                                claps->argc = i - claps->i - 1;
                            }
                        }                                                                         
                    }                    
                }   
            }

            claps = claps->next;
        }
            
        lines = lines->next;
    }    
}

/*
    Bubble sort a linked list
 */
EXPORT_IMPORT void __stdcall Bubble (PCLA *head)
{
    if (head == NULL)
    {
        return;
    }
    
    int swapped; // To check if any swaps were made    
    PCLA *lCurrent = NULL; // Local current

    do
    {        
        PCLA *current = head;
        swapped = 0;

        while (current->next != lCurrent) // At the start, ICurrent is NULL
        {
            // Compare the 'i' values of adjacent nodes
            if (current->i > current->next->i)
            {                
                // Swap the nodes such that the next becomes current and vice versa

                PCLA pcla;
                pcla.i = current->i;
                pcla.l = current->l;
                pcla.argc = current->argc;

                current->i = current->next->i;
                current->l = current->next->l;
                current->argc = current->next->argc;

                current->next->i = pcla.i;
                current->next->l = pcla.l;
                current->next->argc = pcla.argc;

                swapped = 1; // Set the swapped flag
            }

            current = current->next;
        }

        lCurrent = current; // Mark the last node checked
    } 
    while (swapped);    
}

void __stdcall Stop(PCLA *head)
{    
    PCLA *current = head;

    //std::cout<< "argc = " << current->argc << std::endl;
    
    while (current != NULL)
    {
        PCLA *lCurrent = current->next;

        //std::cout<< "argc = " << current->argc << std::endl;

        //std::cout<< current->l << " - " << current->t << " - " << current->i << std::endl;

        cc_tokenizer::allocator<char>().deallocate(reinterpret_cast<char*>(current));

        current = lCurrent;
    }    
}

EXPORT_IMPORT void __stdcall StopClapping (COMMANDLINES *lines) 
{
    COMMANDLINES *head = lines;
    
    while (head != NULL)
    {
        COMMANDLINES *line = head->next;
        
        if (line != NULL)
        {
            Stop (line->clap);
        }
        
        cc_tokenizer::allocator<char>().deallocate(reinterpret_cast<char*>(head));

        head = line;
    }
}

EXPORT_IMPORT COMMANDLINES* __stdcall StartClapping (int argc, char* argv[], const char *command)
{
    COMMANDLINES *lines = NULL, *line = NULL;
    cc_tokenizer::csv_parser<cc_tokenizer::String<char>, char> commandParser( command );

    while (commandParser.go_to_next_line() != cc_tokenizer::string_character_traits<char>::eof())
    {
        PCLA *clap = NULL;

        if (lines == NULL)
        {
            lines = reinterpret_cast<COMMANDLINES*>(cc_tokenizer::allocator<char>().allocate(sizeof(COMMANDLINES)));            
            lines->next = NULL;
            lines->prev = NULL;

            line = lines;
        }
        else
        {
            line->next = reinterpret_cast<COMMANDLINES*>(cc_tokenizer::allocator<char>().allocate(sizeof(COMMANDLINES)));
            line->next->prev = line;
            line->next->next = NULL;

            line = line->next;
        }

        line->nTokens = 0;
        line->helpStrPosition = cc_tokenizer::String<char>::npos;
        line->helpStrSize = 0;
        line->nClaps = 0;
        line->clap = NULL;

        // bPos (Begining position), ePos (Ending position)
        cc_tokenizer::string_character_traits<char>::size_type bPos = 0, ePos = 0;
        if ((bPos = commandParser.get_current_line().find(HELP_STR_START, 0)) != cc_tokenizer::String<char>::npos)
        {  
            line->helpStrPosition = bPos + HELP_STR_START_SIZE;

            /*
            if ((ePos = commandParser.get_current_line().find(HELP_STR_END, 0)) != cc_tokenizer::String<char>::npos)
            {
                //std::cout<< "ePos - bPos = " << ePos - (bPos + HELP_STR_START_SIZE) << std::endl;
            }
            else
            {   
                ePos = commandParser.get_current_line().size();
                //std::cout<< "commandParser.get_current_line().size() - bPos = " << ePos - (bPos + HELP_STR_START_SIZE) << std::endl;
            }
             */

            if ((ePos = commandParser.get_current_line().find(HELP_STR_END, 0)) == cc_tokenizer::String<char>::npos)
            {
                ePos = commandParser.get_current_line().size();  
            }

            line->helpStrPosition = bPos + HELP_STR_START_SIZE;
            line->helpStrSize =  ePos - (bPos + HELP_STR_START_SIZE);               
        }

        while (commandParser.go_to_next_token() != cc_tokenizer::string_character_traits<char>::eof() && commandParser.get_current_token().find(HELP_STR_START, 0) == cc_tokenizer::String<char>::npos)
        {        
            line->nTokens += 1;
                        
            for (int i = 0; i < argc; i++)
            {
                if (commandParser.get_current_token().compare(cc_tokenizer::String<char>(argv[i])) == 0)                 
                {
                    line->nClaps += 1;

                    if (line->clap == NULL)
                    {
                        line->clap = reinterpret_cast<PCLA*>(cc_tokenizer::allocator<char>().allocate(sizeof(PCLA)));
                        line->clap->next = NULL;
                        line->clap->prev = NULL;

                        clap = line->clap;
                    }
                    else
                    {
                        clap->next = reinterpret_cast<PCLA*>(cc_tokenizer::allocator<char>().allocate(sizeof(PCLA)));
                        clap->next->prev = clap;
                        clap->next->next = NULL;

                        clap = clap->next;
                    }
                    
                    clap->i = i; // Index into argv
                    clap->l = commandParser.get_current_line_number(); // Line number
                    clap->t = commandParser.get_current_token_number(); // Token number                    
                    //clap->argc = 0;
                    clap->argc = argc - clap->i - 1;                    
                }
            }
            
        }

        Bubble (line->clap);        
    }

    BubbleAfter (argc, argv, command, lines);

    return lines;
}

EXPORT_IMPORT COMMANDLINES* __stdcall FindArg(COMMANDLINES *lines, const char *command, const char *arg)
{
    int l = 0;
    COMMANDLINES *ptr = NULL;
    cc_tokenizer::csv_parser<cc_tokenizer::String<char>, char> commandParser( command );

    while (commandParser.go_to_next_line() != cc_tokenizer::string_character_traits<char>::eof() && ptr == NULL)
    {
        l = l + 1;

        while (commandParser.go_to_next_token() != cc_tokenizer::string_character_traits<char>::eof() && commandParser.get_current_token().find(HELP_STR_START, 0) == cc_tokenizer::String<char>::npos && ptr == NULL)
        {            
            if (commandParser.get_current_token().compare(cc_tokenizer::String<char>(arg)) == 0)                 
            {                
                COMMANDLINES *line = lines;

                while (line != NULL && ptr == NULL)
                {
                    if (line->clap != NULL && line->clap->l == l)
                    {                        
                        ptr = line;                        
                    }

                    line = line->next;
                }
            }            
        }        
    }

    return ptr;
}

EXPORT_IMPORT void HelpTemplate (COMMANDLINES *lines, const char *command)
{
    cc_tokenizer::csv_parser<cc_tokenizer::String<char>, char> commandParser( command );
    COMMANDLINES *line = lines;
    while (commandParser.go_to_next_line() != cc_tokenizer::string_character_traits<char>::eof())
    {
        cc_tokenizer::String<char> str (commandParser.get_current_line().c_str() + line->helpStrPosition, line->helpStrSize);

        std::cout<< str.c_str() << " [ ";

        for (int i = 1; i <= line->nTokens; i++)
        {
            std::cout<< commandParser.get_token_by_number(i).c_str() << " ";
        }

        std::cout<< "]" << std::endl;

        line = line->next; 
    }    
}

EXPORT_IMPORT void __stdcall HelpLineTemplate (COMMANDLINES *lines, const char *command, const char *arg)
{    
    bool found = false; 
    COMMANDLINES *line = lines;       
    cc_tokenizer::csv_parser<cc_tokenizer::String<char>, char> commandParser( command );

    while (!found && commandParser.go_to_next_line() != cc_tokenizer::string_character_traits<char>::eof())
    {
        for (int i = 1; i <= line->nTokens && !found; i++)
        {
            if (commandParser.get_token_by_number(i).compare(arg) == 0)
            {
                found = true;
            }
        }    

        if (!found)
        {
            line = line->next;
        }
    }

    if (found)
    {
        std::cout<< "Example usage..." << std::endl;

        cc_tokenizer::String<char> str (commandParser.get_current_line().c_str() + line->helpStrPosition, line->helpStrSize);
        
        std::cout<< str.c_str() << " [ ";

        for (int i = 1; i <= line->nTokens; i++)
        {
            std::cout<< commandParser.get_token_by_number(i).c_str() << " ";
        }

        std::cout<< "]" << std::endl;
    }
}