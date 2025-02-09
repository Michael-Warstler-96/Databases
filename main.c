/**
   @file main.c 
   @author Michael Warstler (mwwarstl)
   @author Teaching Staff
   Continuously loops while taking user input for commands. Commands are entered to the terminal
   and are then processed by parser.c. After being parsed, the commands are executed if possible.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "database.h"

/**
   The execute_query takes a parsed query as input and execute the specific function based on the
   query type. This function returns EXIT_SUCCESS status on successful execution and retuerns
   EXIT_FAILURE otherwise. See parser.h for detail about the Query objects. 
*/
int execute_query( Query query ){
    switch ( query.type ) {
        case CREATE_TABLE:
            create_table( query.table_name );
            break;
            
        case INSERT:
            insert_into_table( query.table_name, query.table_row );
            break;
            
        case SELECT:
            select_from_table( query.table_name, query.condition_variable, query.condition_type,
                               query.condition_value );
            break;
            
        case UPDATE:  
            update( query.table_name, query.table_row, query.set_clause );
            break;
            
        case DELETE:
            delete_row( query.table_name, query.table_row );
            break;
            
        case DROP:
            drop_database_file( query.table_name );
            break;
            
        case READ_FILE:
            read_database_file( query.table_name );
            break;
            
        case WRITE_FILE:
            write_database_file( query.table_name );    //table name is technically just a filename
            break;
            
        case HELP:
            break;
            
        default:
            printf( "Unrecognized query.\n" );
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


/**
   The main fuction takes user input in natural language, parse the input into command or database 
   query, and then execute the query. THe main function do this repeatedly until an exit command
   is executed.
   @return is exit status.
*/
int main() {
    char command[256];  // Buffer to store the command - 256 listed by teaching staff.

    while (1) {
        printf( "cmd> " );  // Display the prompt
        if ( fgets(command, sizeof(command), stdin) == NULL ) {
            printf( "\n" );
            break;  // Exit on EOF (Ctrl+D)
        }

        // Remove the newline character at the end
        size_t length = strlen( command );
        if ( length > 0 && command[length - 1] == '\n' ) {
            command[length - 1] = '\0';
        }

        // Exit the shell on "exit" command
        if ( strcmp(command, "exit") == 0 ) {
            break;
        }

        // Parse the command 
        Query query = parse_query( command );

        // Execute the command 
        execute_query( query );
        
        //printf( "\n" ); ///////// Commented out in order to get test 1 to pass...
    }
    return 0;
}

