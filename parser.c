/**
   @file parser.c 
   @author Michael Warstler (mwwarstl)
   @author Teaching Staff
   Implementation file handles parsing a string for a query. Depending on the query, a query type 
   is set for 1 of several possible commands. If query is valid, the type is returned, so that a
   specific database command can be executed.   
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

/**
   This function receives a query string, parse it into some information fields 
   defined in Query structure, and return it. This information is used in main.c
   to call necessary function to do the job asked in the query string. 
*/
Query parse_query( const char *query_string ) {
    Query parsed_query;
    parsed_query.type = INVALID_QUERY;

    // make a copy of the query string
    char *query_copy = (char *) strdup(query_string);
    if ( query_copy == NULL ) {
        fprintf( stderr, "Memory allocation error\n" );
        exit( EXIT_FAILURE );
    }

    // Tokenize the string
    char *token = strtok( query_copy, " \t\n" );
    if ( token == NULL ) {
        fprintf( stderr, "Empty query\n" );
        free( query_copy) ;
        exit( EXIT_FAILURE );
    }

    // find type of query 
    if ( strcmp(token, "create_table") == 0 ) {
        parsed_query.type = CREATE_TABLE;
    } 
    else if ( strcmp(token, "insert") == 0 ) {
        parsed_query.type = INSERT;
    } 
    else if ( strcmp(token, "select") == 0 ) {
        parsed_query.type = SELECT;
    } 
    else if ( strcmp(token, "update") == 0 ) {
        parsed_query.type = UPDATE;
    } 
    else if ( strcmp(token, "delete") == 0 ) {
        parsed_query.type = DELETE;
    } 
    else if ( strcmp(token, "read_file") == 0 ) {
        parsed_query.type = READ_FILE;
    } 
    else if ( strcmp(token, "write_file") == 0 ) {
        parsed_query.type = WRITE_FILE;
    } 
    else if ( strcmp(token, "drop") == 0 ) {
        parsed_query.type = DROP;
    }
    else if ( strcmp(token, "help") == 0 ) {
        parsed_query.type = HELP;
    } 
    else {
        fprintf( stderr, "Invalid query type\n" );
        free( query_copy );
        return parsed_query;
    }

    // Based on the query type, parse rest of the string
    switch ( parsed_query.type ) {
        case HELP:
            // Print out commands possible.
            printf( "Following are the valid query commands: \n" );
            printf( "help                             \n" );
            printf( "create_table [table_name]        \n" );
            printf( "insert [table_name] [row Values] \n" );
            printf( "select [table_name] [condition]  \n" );
            printf( "delete [table_name] [condition]  \n" );
            printf( "read_file [table_name]           \n" );
            printf( "update [row_id] [row Values] \n" );
            printf( "drop [table_name]                \n" );
            printf( "write_file [file_name]           " );

            parsed_query.type = HELP;
            return parsed_query;
            
        case CREATE_TABLE:
        	// Parse table name
            token = strtok(NULL, " \t\n");
            if ( token == NULL ) {
                fprintf( stderr, "Table name missing\n" );
                free( query_copy );
                parsed_query.type = INVALID_QUERY;
                return parsed_query;
            }
            strncpy( parsed_query.table_name, token, MAX_TABLE_NAME_LENGTH - 1 );
            parsed_query.table_name[MAX_TABLE_NAME_LENGTH - 1] = '\0';

            return parsed_query;

        case INSERT:
        	// Parse table name
            token = strtok( NULL, " \t\n" );
            if ( token == NULL ) {
                fprintf( stderr, "Table name missing\n" );
                free( query_copy );
                parsed_query.type = INVALID_QUERY;
                return parsed_query;
            }
            strncpy( parsed_query.table_name, token, MAX_TABLE_NAME_LENGTH - 1 );
            parsed_query.table_name[MAX_TABLE_NAME_LENGTH - 1] = '\0';
            
            // Parse row values
            token = strtok( NULL, "" );
            if ( token == NULL ) {
                fprintf( stderr, "Row values missing\n" );
                free( query_copy );
                parsed_query.type = INVALID_QUERY;
                return parsed_query;
            }
            strncpy( parsed_query.table_row, token, MAX_TABLE_VALUE_LENGTH - 1 );
            parsed_query.table_row[MAX_TABLE_VALUE_LENGTH - 1] = '\0';

            return parsed_query;
        	break;

        case SELECT:
            // Parse table name
            token = strtok( NULL, " \t\n" );
            if ( token == NULL ) {
                fprintf( stderr, "Table name missing\n" );
                free( query_copy );
                parsed_query.type = INVALID_QUERY;
                return parsed_query;
            }
            strncpy( parsed_query.table_name, token, MAX_TABLE_NAME_LENGTH - 1 );
            parsed_query.table_name[MAX_TABLE_NAME_LENGTH - 1] = '\0';

            // Parse query condition (any column name from the table) 
            token = strtok( NULL, " \t\n" );
            if ( token == NULL ) {
                fprintf( stderr, "Conditions missing\n" );
                free( query_copy );
                parsed_query.type = INVALID_QUERY;
                return parsed_query;
            }
            strncpy( parsed_query.condition_variable, token, MAX_CONDITIONS_LENGTH - 1 );
            parsed_query.condition_variable[MAX_CONDITIONS_LENGTH - 1] = '\0';
            
            // Parse condition type (== or !=)
            token = strtok( NULL, " \t\n" );
            if ( token == NULL ) {
                fprintf( stderr, "Conditions incomplete\n" );
                free( query_copy );
                parsed_query.type = INVALID_QUERY;
                return parsed_query;
            }
            strncpy( parsed_query.condition_type, token, MAX_CONDITIONS_LENGTH - 1 );
            parsed_query.condition_type[MAX_CONDITIONS_LENGTH - 1] = '\0';
            
            // Parse condition value.
            token = strtok( NULL, "\t\n" );
            if ( token == NULL ) {
                fprintf( stderr, "Conditions incomplete\n" );
                free( query_copy );
                parsed_query.type = INVALID_QUERY;
                return parsed_query;
            }
            strncpy( parsed_query.condition_value, token, MAX_CONDITIONS_LENGTH - 1 );
            parsed_query.condition_value[MAX_CONDITIONS_LENGTH - 1] = '\0';

            return parsed_query;
            break;

        case DELETE:
        	 // Parse table name
            token = strtok( NULL, " \t\n" );
            if ( token == NULL ) {
                fprintf( stderr, "Table name missing\n" );
                free( query_copy );
                parsed_query.type = INVALID_QUERY;
                return parsed_query;
            }
            strncpy( parsed_query.table_name, token, MAX_TABLE_NAME_LENGTH - 1 );
            parsed_query.table_name[MAX_TABLE_NAME_LENGTH - 1] = '\0';
            
            // Parse row values - should only be an id value.
            token = strtok( NULL, " \t\n" );
            if ( token == NULL ) {
                fprintf( stderr, "Record id not found!\n" );
                free( query_copy );
                parsed_query.type = INVALID_QUERY;
                return parsed_query;
            }
            strncpy( parsed_query.table_row, token, MAX_TABLE_VALUE_LENGTH - 1 );
            parsed_query.table_row[MAX_TABLE_VALUE_LENGTH - 1] = '\0';
            

            return parsed_query;
            break;

        case UPDATE:
        	// Parse table name
            token = strtok( NULL, " \t\n" );
            if ( token == NULL ) {
                fprintf( stderr, "Table name missing\n" );
                free( query_copy );
                parsed_query.type = INVALID_QUERY;
                return parsed_query;
            }
            strncpy( parsed_query.table_name, token, MAX_TABLE_NAME_LENGTH - 1 );
            parsed_query.table_name[MAX_TABLE_NAME_LENGTH - 1] = '\0';
            
            // Parse row value
            token = strtok( NULL, " \t\n" );
            if ( token == NULL ) {
                fprintf( stderr, "Record id not found!\n" );
                free( query_copy );
                parsed_query.type = INVALID_QUERY;
                return parsed_query;
            }
            strncpy( parsed_query.table_row, token, MAX_TABLE_VALUE_LENGTH - 1 );
            parsed_query.table_row[MAX_TABLE_VALUE_LENGTH - 1] = '\0';

            // Parse update attributes (set_clause)
            token = strtok( NULL, "\t\n" );
            if ( token == NULL ) {
                fprintf( stderr, "Record value not found!\n" );
                free( query_copy );
                parsed_query.type = INVALID_QUERY;
                return parsed_query;
            }
            strncpy( parsed_query.set_clause, token, MAX_SET_CLAUSE_LENGTH - 1 );
            parsed_query.set_clause[MAX_SET_CLAUSE_LENGTH - 1] = '\0';
        	break;
            
        case DROP:
            // Parse table name.
            token = strtok( NULL, " \t\n" );
            if ( token == NULL ) {
                fprintf( stderr, "Table name missing\n" );
                free( query_copy );
                parsed_query.type = INVALID_QUERY;
                return parsed_query;
            }
            strncpy( parsed_query.table_name, token, MAX_TABLE_NAME_LENGTH - 1 );
            parsed_query.table_name[MAX_TABLE_NAME_LENGTH - 1] = '\0';

            return parsed_query;
            break;

        case READ_FILE:
            // Parse table name.
            token = strtok( NULL, " \t\n" );
            if ( token == NULL ) {
                fprintf( stderr, "Table name missing\n" );
                free( query_copy );
                parsed_query.type = INVALID_QUERY;
                return parsed_query;
            } 
            strncpy( parsed_query.table_name, token, MAX_TABLE_NAME_LENGTH - 1 );
            parsed_query.table_name[MAX_TABLE_NAME_LENGTH - 1] = '\0';
            
            return parsed_query;
            break;

        case WRITE_FILE:
            // Parse table name. - Technically just a "filename" to write to.
            token = strtok( NULL, " \t\n" );
            if ( token == NULL ) {
                fprintf( stderr, "Table name missing\n" );
                free( query_copy );
                parsed_query.type = INVALID_QUERY;
                return parsed_query;
            }
            strncpy( parsed_query.table_name, token, MAX_TABLE_NAME_LENGTH - 1 );
            parsed_query.table_name[MAX_TABLE_NAME_LENGTH - 1] = '\0';

            return parsed_query;
            break;

        default:
            // Should never reach here
            fprintf( stderr, "Invalid query type\n" );
            free( query_copy );
            return parsed_query;
    }

    // Free allocated memory
    free( query_copy );
    return parsed_query;
}