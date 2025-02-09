/**
   @file database.h 
   @author Michael Warstler (mwwarstl)
   @author Teaching Staff
   Header file for a handling a database. Contains functions for creating a table, checking
   if a table exist, inserting into a table, reading a singular table, selecting from a table,
   writing a database to a file, updating a line in a table, deleting a line in a table, and 
   dropping/removing an entire table.
*/
#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/** Max number of characters for a title */
#define MAX_TITLE_LENGTH 255
/** Max number of characters for an author */
#define MAX_AUTHOR_LENGTH 255
/** Max number of characters for a category */
#define MAX_CATEGORY_LENGTH 255
/** Max number of characters for a message */
#define MESSAGE_LENGTH 1000
/** Max number of characters for an id */
#define ID_LENGTH 10
/** Max number of records for a table/file */
#define MAX_RECORDS 10000
/** Max number of characters in a string */
#define MAX_STR_LENGTH 2048

/** This variable holds the location for database folder */
extern char *folder;

/** The following structures define the tables for the database.  */

/** A book struct contains an id, title, and categoty id. */
typedef struct {
    int id;
    char title[MAX_TITLE_LENGTH];
    int category_id;
} Book;

/** A Category struct contains an id and a name. */
typedef struct {
    int id;
    char name[MAX_CATEGORY_LENGTH];
} Category;

/** Author struct contains an id and a name. */
typedef struct {
    int id;
    char name[MAX_AUTHOR_LENGTH];
} Author;

/** Book_author struct, contains book id and author id. */
typedef struct {
    int book_id;
    int author_id;
} Book_author;

/** Publisher struct contains an id and name. */
typedef struct {
    int id;
    char name[MAX_AUTHOR_LENGTH];
} Publisher;

/** Book_copy struct contains an id, book id, publisher id, and year published. */
typedef struct {
    int id;
    int book_id;
    int publisher_id;
    int year_published;
} Book_copy;

/** Member_account struct contains an id, first & last name, and email. */
typedef struct {
    int id;
    char first_name[MAX_AUTHOR_LENGTH];
    char last_name[MAX_AUTHOR_LENGTH];
    char email[MAX_AUTHOR_LENGTH];
} Member_account;

/** Date struct contains a day, month, and year. Is NOT a database, but is used by others. */
typedef struct {
    int day;
    int month;
    int year;
} Date;

/** 
   Checkout struct contains an id, checkout & return date, book copy id, member id, and return
   status.
*/
typedef struct {
    int id;
    Date checkout_date;
    Date return_date;
    int book_copy_id;
    int member_id;
    bool is_returned;
} Checkout;

/** Hold struct contains an id, checkout & return date, book copy id, and member id. */
typedef struct {
    int id;
    Date checkout_date;
    Date return_date;
    int book_copy_id;
    int member_id;
} Hold;

/** Waitlist struct contains a book and member id. */
typedef struct {
    int book_id;
    int member_id;
} Waitlist;

/** Notification struct contains an id, a sent at Date, member id, and a message. */
typedef struct {
    int id;
    Date sent_at;
    int member_id;
    char message[MESSAGE_LENGTH];
} Notification;

/**
   Creates a table/file with the table_name string parameter.
   @param table_name is string representation of the table's name.
   @return is EXIT_FAILURE if a table already exist under param name, otherwise returns EXIT_SUCCESS
*/
int create_table( const char *table_name );

/**
   Checks if a table/file already exist with parameter name.
   @param table_name is string representing table's name.
   @return is EXIT_SUCCESS if matching table found, EXIT_FAILURE otherwise.
*/
int table_exist( const char *table_name );

/**
   Inserts contents of table_row parameter to the table/file designated by the table_name param.
   @param table_name is string name for table/file.
   @param table_row is string containing table_row information.
   @return is EXIT_FAILURE if table couldn't be opened, otherwise returns EXIT_SUCCESS.
*/
int insert_into_table( const char *table_name, const char *table_row );

/**
   Reads and prints an entire table matching the table_name parameter. If no table is found, then 
   error is printed and exits with failure. Otherwise, reads the records line by line.
   @param table_name is string for table/file's name.
   @return is EXIT_FAILURE if function could not print table, otherwise EXIT_SUCCESS
*/
int read_database_file( const char *table_name );

/**
   Takes a file name to write out the contents of an entire database. Writing filename cannot
   match an existing database table name. An error message is printed if attempting to write a
   database file when no database tables exist. If database tables exist, this function will 
   write the contents of each database table to the specified valid parameter filename.
   @param table_name is string name of the table/file to write entire database contents to.
   @return is EXIT_FAILURE if error occurs, otherwise EXIT_SUCCESS.
*/   
int write_database_file ( const char *table_name );

/**
   Command prints all table rows/records if conditions are met. Table_name parameter determines
   which table to select from. The condition variable is how the user wants to sort selection
   from (i.e sort by id, title, category_id, etc.). The condition is != or ==. The condition value
   is whatever the user wishes to match, or not match, with the condition variable.  
   @param table_name is string for which table to check.
   @param condition_var is the specific variable in the table to select.
   @param condition is the selection condition, either != or ==
   @param condition_val is the value to check the condition with. 
   @return is EXIT_FAILURE if error occurs, otherwise EXIT_SUCCESS
*/
int select_from_table( const char *table_name, const char *condition_var, const char *condition, const char *condition_val );

/**
   Deletes the entire table matching parameter name.
   @param table_name is string representing a table.
   @return is EXIT_FAILURE if table/file could not be deleted, otherwise EXIT_SUCCESS
*/
int drop_database_file( const char *table_name );

/**
   Deletes a singular record from a table with a matching table_name and row id. If no record with 
   matching row id is found, prints an error message. Otherwise prints a success message.
   @param tabl_name is string represnting a table/file.
   @param table_row is string containing table_row information.
   @return is EXIT_FAILURE if table row could not be deleted, otherwise returns EXIT_SUCCESS
*/
int delete_row( const char *table_name, const char *table_row );

/**
   Updates the contents of a table row with the given attributes parameter. Function finds a row 
   from the table parameter that matches the row parameter and replaces contents after the row id
   with the attributes parameter. If no record/row is found, then an error message is printed.
   @param table_name is string representing a table/file.
   @param table_row is string containing table_row (id) information.
   @param attributes is a string of the attributes that will update/replace the original row's data
   @return is EXIT_FAILURE if error occurs, otherwise EXIT_SUCCESS
*/
int update( const char *table_name, const char *table_row, const char *attributes );

#endif //DATABASE_H