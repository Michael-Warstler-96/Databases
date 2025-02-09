/**
   @file parser.h 
   @author Michael Warstler (mwwarstl)
   @author Teaching Staff
   Header file for parser.c. Defines a Query struct which is used to hold data entered by the user.
   The parse_query function is declared as the primary function of reading and sorting user entered
   query. The QueryType enum declares possible enum values for commands.
*/
#ifndef PARSER_H
#define PARSER_H

/** Max number of characters in a string */
#define MAX_STRING_LENGTH 255
/** Max number of characters for a table's name */
#define MAX_TABLE_NAME_LENGTH 255
/** Max number of characters for a condition */
#define MAX_CONDITIONS_LENGTH 1023
/** Max number of characters for a set clause */
#define MAX_SET_CLAUSE_LENGTH 1023
/** Max number of characters for a table's value length */
#define MAX_TABLE_VALUE_LENGTH 2047

/**
   Enumeration values a Query type can be.
*/
typedef enum {
    CREATE_TABLE,
    INSERT,
    SELECT,
    UPDATE,
    DELETE,
    DROP,
    READ_FILE,
    WRITE_FILE,
    INVALID_QUERY, 
    HELP
} QueryType;

/**
   This structure defines datatype for Query. A variable of Query holds necessary information about
   the query. This includes a table's name, a QueryType, condition vartiable, condition type
   (!= or ==), a condition value, and a table's row.
*/
typedef struct {
    QueryType type;                             // holds different type of query
    char table_name[MAX_TABLE_NAME_LENGTH];     // holds table name 

    char condition_variable[MAX_CONDITIONS_LENGTH]; // holds condition variable of a query
    char condition_type[MAX_CONDITIONS_LENGTH];     // holds condition type, either == or !=
    char condition_value[MAX_CONDITIONS_LENGTH];    // holds condition value

    char set_clause[MAX_SET_CLAUSE_LENGTH];         // researved, you may use to hold any other information
    char table_row[MAX_TABLE_VALUE_LENGTH];         // holds record information provided in a query
} Query;

/**
   Parse contents of a string to determine a query type and possible extra arguments.
   @param query_string is a constant string representing the query.
   @return is the Query struct created by the Query string.
*/
Query parse_query( const char *query_string );

#endif //PARSER_H