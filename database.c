/**
   @file database.c 
   @author Michael Warstler (mwwarstl)
   @author Teaching Staff
   Implementation file for a handling a database. Contains functions for creating a table, checking
   if a table exist, inserting into a table, reading a singular table, selecting from a table,
   writing a database to a file, updating a line in a table, deleting a line in a table, and 
   dropping/removing an entire table.
*/
#include <unistd.h>
#include "database.h"

/** Number of databases defined in database.h */
#define DATABASE_SIZE 11

/** The path for a tables folder */
char *folder = "./tables"; 

/** Creates a table. */
int create_table( const char *table_name ){
	char filepath[MAX_STR_LENGTH];                                                                  
    snprintf( filepath, sizeof(filepath), "%s/%s", folder, table_name );

    // Check if the file exists, return EXIT_FAILURE if exists 
    if ( access(filepath, F_OK) != -1 ) {
    	printf( "Table '%s' already exists.\n", table_name );
        return EXIT_FAILURE; 
    } 
    else {
    	FILE *file = fopen( filepath, "w" );
        if ( file != NULL ) {
            printf( "Table '%s' created successfully.\n", table_name );
            fclose(file);
        } else {
            printf( "Failed to create '%s' table.\n", table_name );
        }
        return EXIT_SUCCESS; 
    }
}

/** This function is defined to to check if a table is already exist. */
int table_exist( const char *table_name ){
	char filepath[MAX_STR_LENGTH];
    snprintf( filepath, sizeof(filepath), "%s/%s", folder, table_name );

    // Check if the file exists and print error if it doesn't.
    if ( access(filepath, F_OK) != -1 ) {
        return EXIT_SUCCESS; 
    }
    else {
        printf( "Table %s does not exist!\n", table_name );
        printf( "Run: create_table %s", table_name );
        return EXIT_FAILURE; 
    }
}

/** This function is defined to insert a record into a table. */
int insert_into_table( const char *table_name, const char *table_row ){
	if ( table_exist(table_name) == EXIT_SUCCESS )
	{
		char filepath[MAX_STR_LENGTH];
    	snprintf(filepath, sizeof(filepath), "%s/%s", folder, table_name);

    	// Open file, handle file opening error, write file, print success or error message
        FILE *fp = fopen( filepath, "a" );
        if ( fp == NULL ) {
            printf( "The data insertion failed!\n" ); 
            return EXIT_FAILURE;
        }
        else {
            // Add contents of table_row to end of current table/file.
            fprintf( fp, "%s\n", table_row );
            printf( "Data inserted successfully!\n" );
            fclose( fp );   // close file when finished.
        }
	}
	return EXIT_SUCCESS;
}

/** This function is defined to read a table. */
int read_database_file( const char *table_name ) {
    // Check for NULL error.
    if ( table_name == NULL ) {
        printf( "Table name missing!\n" );
        return EXIT_FAILURE;
    }
    
    // Set up filepath to read from.
    char filepath[MAX_STR_LENGTH];
    snprintf( filepath, sizeof(filepath), "%s/%s", folder, table_name );

    // Check if the file exists and print error if it doesn't.
    FILE *file = fopen( filepath, "r" );
    if ( file != NULL ) {
        // read in table and print to console line by line.
        char line[MAX_STR_LENGTH];
        while ( fgets(line, sizeof(line), file) ) {
            printf( line );
        }
        fclose( file );
        return EXIT_SUCCESS; 
    }
    else {
        printf( "Table %s not found!\n", table_name );
        return EXIT_FAILURE; 
    }
}

/**
   This function is defined to find a row(s) based on a condition. Some database structs share 
   similar fields and can be "selected" in the same logic branch.
*/
int select_from_table( const char *table_name, const char *condition_var, const char *condition,
                       const char *condition_val ) {
    // Open the file for reading
    FILE *file;
    char filepath[MAX_STR_LENGTH];
    snprintf( filepath, sizeof( filepath ), "%s/%s", folder, table_name );
    file = fopen( filepath, "r" );
    if ( file == NULL ) { 
        perror("Table not exist!");
        return EXIT_FAILURE;
    }
    
    // Variables to hold data count and line string regardless of the database type.
    int data_count = 0; 
    char line[MAX_STR_LENGTH];
    
    // Check for database of type book.
	if ( strcmp( table_name, "book" ) == 0 ) {
		// Set up array of Book structs.
        Book *book_data = ( Book * )malloc( MAX_RECORDS * sizeof( Book ) );
		if ( book_data == NULL ) { 
            printf( "Memory allocation failed\n" ); 
            return EXIT_FAILURE; 
        }

        // Read and process each line. Insert to array of Books.
	    while ( fgets(line, sizeof( line ), file) ) { 
            // Remove newline character if present
	        if ( line[strlen( line ) - 1] == '\n' ) { 
                line[strlen( line ) - 1] = '\0';
            }
			// Process the line
	        char *token = strtok(line, " ");
	        book_data[data_count].id = atoi(token);
	        token = strtok( NULL, "\"" );
	        strcpy( book_data[data_count].title, token );
	        token = strtok( NULL, " " );
	        book_data[data_count].category_id = atoi( token );
	        ++data_count;
		}
        
        // If condition_var is "id"...
		if ( strcmp( condition_var, "id" ) == 0 ) {
			int value = atoi( condition_val );
	        // If id matches, print to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( book_data[i].id == value ) { 
                        printf( "%d %s %d\n", book_data[i].id, book_data[i].title,
						                     book_data[i].category_id );
					}
				}
			} 
            // If id does not match, print to console.
            else if ( strcmp(condition, "!=" ) == 0 ) {     
                for ( int i = 0; i < data_count; ++i ) {
					if ( book_data[i].id != value ) {
						printf( "%d %s %d\n", book_data[i].id, book_data[i].title,
						                     book_data[i].category_id );
					}
				}
            }
            else {
                printf( "conditions invalid\n" );
            }
        } 
        // If condition_var is "title"...
        else if ( strcmp( condition_var, "title" ) == 0 ) {
			// If title matches, print it to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
                    if ( strcmp( book_data[i].title, condition_val ) == 0 ) {
						printf( "%d %s %d\n", book_data[i].id, book_data[i].title,
						                     book_data[i].category_id );
					}
				}
			} 
            // If title does not match, print it to console.
            else if ( strcmp( condition, "!=" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( strcmp( book_data[i].title, condition_val) != 0 ) {
						printf( "%d %s %d\n", book_data[i].id, book_data[i].title,
						                     book_data[i].category_id );
					}
				}
			}
            else {
                printf( "conditions invalid\n" );
            }            
		}
        // If condition_var is category_id
        else if ( strcmp( condition_var, "category_id" ) == 0 ) { 
            int value = atoi( condition_val );
            // If category_id matches, print it to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
                    if ( book_data[i].category_id == value ) {
						printf( "%d %s %d\n", book_data[i].id, book_data[i].title,
						                     book_data[i].category_id );
					}
				}
			} 
            // If category_id does not match, print it to console.
            else if ( strcmp( condition, "!=" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( book_data[i].category_id != value ) {
						printf( "%d %s %d\n", book_data[i].id, book_data[i].title,
						                     book_data[i].category_id );
					}
				}
			}
            else {
                printf( "conditions invalid\n" );
            }            
		}
        // Invalid condition_var for a book.
        else {
			printf( "conditions invalid\n" );
		}
        free( book_data );
	}
    
    // Table name of category, author, and publisher are all identified with just id and name. 
    else if ( strcmp( table_name, "category" ) == 0 || strcmp( table_name, "author" ) == 0 ||
              strcmp( table_name, "publisher" ) == 0 ) {
                  
		// Set up array of Category structs.(Technically same as author and publisher)
        Category *category_data = ( Category * )malloc( MAX_RECORDS * sizeof( Category ) );
		if ( category_data == NULL ) {
            printf( "Memory allocation failed\n" );
            return EXIT_FAILURE;
        }
        
        // Read and process each line. Insert to array of categories.
	    while ( fgets(line, sizeof( line ), file) ) { 
            // Remove newline character if present
	        if ( line[strlen( line ) - 1] == '\n' ) {
                line[strlen( line ) - 1] = '\0';
            }
			// Process the line (here, just print it) 
	        char *token = strtok(line, " ");
	        category_data[data_count].id = atoi(token);
	        token = strtok( NULL, "\"" );
	        strcpy( category_data[data_count].name, token );
	        ++data_count;
		}
        
        // If condition_var is "id"...
		if ( strcmp( condition_var, "id" ) == 0 ) {
			int value = atoi( condition_val );
	        // If id matches, print to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( category_data[i].id == value ) { 
                        printf( "%d %s\n", category_data[i].id, category_data[i].name );
					}
				}
			} 
            // If id does not match, print to console.
            else if (strcmp(condition, "!=" ) == 0) {     
                for ( int i = 0; i < data_count; ++i ) {
					if ( category_data[i].id != value ) {
						printf( "%d %s\n", category_data[i].id, category_data[i].name );
					}
				}
            }
            else {
                printf( "conditions invalid\n" );
            }
        } 
        // If condition_var is "name"...
        else if ( strcmp( condition_var, "name" ) == 0 ) {
			// If name matches, print it to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
                    if ( strcmp( category_data[i].name, condition_val ) == 0 ) {
						printf( "%d %s\n", category_data[i].id, category_data[i].name );
					}
				}
			} 
            // If name does not match, print it to console.
            else if ( strcmp( condition, "!=" ) == 0) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( strcmp( category_data[i].name, condition_val) != 0 ) {
						printf( "%d %s\n", category_data[i].id, category_data[i].name );
					}
				}
			}
            else {
                printf( "conditions invalid\n" );
            }            
		}
        // Invalid condition_var for a category/author/publisher.
        else {
			printf( "conditions invalid\n" );
		}
        free( category_data );
	}
    
    // Table is type book_author
    else if ( strcmp( table_name, "book_author" ) == 0 ) {
        // Set up array of book_author structs.
        Book_author *book_author_data = ( Book_author * )malloc( MAX_RECORDS * sizeof( Book_author ) );
		if ( book_author_data == NULL ) {
            printf( "Memory allocation failed\n" );
            return EXIT_FAILURE;
        }
        
        // Read and process each line. Insert to array of Book_authors.
	    while ( fgets(line, sizeof( line ), file) ) { 
            // Remove newline character if present
	        if ( line[strlen( line ) - 1] == '\n' ) { 
                line[strlen( line ) - 1] = '\0';
            }
			// Process the line
	        char *token = strtok(line, " ");
	        book_author_data[data_count].book_id = atoi(token);
	        token = strtok( NULL, " " );
	        book_author_data[data_count].author_id = atoi( token );
	        ++data_count;
		}
        
        // If condition_var is "book_id"...
		if ( strcmp( condition_var, "book_id" ) == 0 ) {
			int value = atoi( condition_val );
	        // If book_id matches, print to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( book_author_data[i].book_id == value ) { 
                        printf( "%d %d\n", book_author_data[i].book_id, 
						                   book_author_data[i].author_id );
					}
				}
			} 
            // If book_id does not match, print to console.
            else if ( strcmp(condition, "!=" ) == 0) {     
                for ( int i = 0; i < data_count; ++i ) {
					if ( book_author_data[i].book_id != value ) {
						printf( "%d %d\n", book_author_data[i].book_id,
						                     book_author_data[i].author_id );
					}
				}
            }
            else {
                printf( "conditions invalid\n" );
            }
        }
        // If condition_var is author_id
        else if ( strcmp( condition_var, "author_id" ) == 0 ) { 
            int value = atoi( condition_val );
            // If category_id matches, print it to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
                    if ( book_author_data[i].author_id == value ) {
						printf( "%d %d\n", book_author_data[i].book_id, 
						                   book_author_data[i].author_id );
					}
				}
			} 
            // If author_id does not match, print it to console.
            else if ( strcmp( condition, "!=" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( book_author_data[i].author_id != value ) {
						printf( "%d %d\n", book_author_data[i].book_id,
						                   book_author_data[i].author_id );
					}
				}
			}
            else {
                printf( "conditions invalid\n" );
            }            
		}
        // Invalid condition_var for a book_author.
        else {
			printf( "conditions invalid\n" );
		}
        free( book_author_data );
    }
    
    // Table is type book_copy
    else if ( strcmp( table_name, "book_copy" ) == 0 ) {
        // Set up array of Book_copy structs.
        Book_copy *book_copy_data = ( Book_copy * )malloc( MAX_RECORDS * sizeof( Book_copy ) );
		if ( book_copy_data == NULL ) {
            printf( "Memory allocation failed\n" );
            return EXIT_FAILURE;
        }
        
        // Read and process each line. Insert to array of book_copy_data.
	    while ( fgets(line, sizeof( line ), file) ) { 
            // Remove newline character if present
	        if ( line[strlen( line ) - 1] == '\n' ) { 
                line[strlen( line ) - 1] = '\0';
            }
			// Process the line
	        char *token = strtok(line, " ");
            book_copy_data[data_count].id = atoi( token );
            token = strtok( NULL, " " );
	        book_copy_data[data_count].book_id = atoi( token );
	        token = strtok( NULL, " " );
	        book_copy_data[data_count].publisher_id = atoi( token );
            token = strtok( NULL, " " );
	        book_copy_data[data_count].year_published = atoi( token );
	        ++data_count;
		}
        
        // If condition_var is "id"
		if ( strcmp( condition_var, "id" ) == 0 ) {
			int value = atoi( condition_val );
	        // If id matches, print to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( book_copy_data[i].id == value ) { 
                        printf( "%d %d %d %d\n", book_copy_data[i].id, book_copy_data[i].book_id,
						        book_copy_data[i].publisher_id, book_copy_data[i].year_published );
					}
				}
			} 
            // If id does not match, print to console.
            else if ( strcmp(condition, "!=" ) == 0 ) {     
                for ( int i = 0; i < data_count; ++i ) {
					if ( book_copy_data[i].id != value ) {
                        printf( "%d %d %d %d\n", book_copy_data[i].id, book_copy_data[i].book_id,
						        book_copy_data[i].publisher_id, book_copy_data[i].year_published );
					}
				}
            }
            else {
                printf( "conditions invalid\n" );
            }
        } 
        // If condition_var is "book_id"...
		else if ( strcmp( condition_var, "book_id" ) == 0 ) {
			int value = atoi( condition_val );
	        // If book_id matches, print to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( book_copy_data[i].book_id == value ) { 
                        printf( "%d %d %d %d\n", book_copy_data[i].id, book_copy_data[i].book_id,
						        book_copy_data[i].publisher_id, book_copy_data[i].year_published );
					}
				}
			} 
            // If book_id does not match, print to console.
            else if ( strcmp(condition, "!=" ) == 0) {     
                for ( int i = 0; i < data_count; ++i ) {
					if ( book_copy_data[i].book_id != value ) {
                        printf( "%d %d %d %d\n", book_copy_data[i].id, book_copy_data[i].book_id,
						        book_copy_data[i].publisher_id, book_copy_data[i].year_published );
					}
				}
            }
            else {
                printf( "conditions invalid\n" );
            }
        }
        // If condition_var is publisher_id
        else if ( strcmp( condition_var, "publisher_id" ) == 0 ) { 
            int value = atoi( condition_val );
            // If publisher_id matches, print it to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
                    if ( book_copy_data[i].publisher_id == value ) {
                        printf( "%d %d %d %d\n", book_copy_data[i].id, book_copy_data[i].book_id,
						        book_copy_data[i].publisher_id, book_copy_data[i].year_published );
					}
				}
			} 
            // If publisher_id does not match, print it to console.
            else if ( strcmp( condition, "!=" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( book_copy_data[i].publisher_id != value ) {
                        printf( "%d %d %d %d\n", book_copy_data[i].id, book_copy_data[i].book_id,
						        book_copy_data[i].publisher_id, book_copy_data[i].year_published );
					}
				}
			}
            else {
                printf( "conditions invalid\n" );
            }            
		}
        // If condition_var is year_published.
        else if ( strcmp( condition_var, "year_published" ) == 0 ) { 
            int value = atoi( condition_val );
            // If year_published matches, print it to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
                    if ( book_copy_data[i].year_published == value ) {
                        printf( "%d %d %d %d\n", book_copy_data[i].id, book_copy_data[i].book_id,
						        book_copy_data[i].publisher_id, book_copy_data[i].year_published );
					}
				}
			} 
            // If year_published does not match, print it to console.
            else if ( strcmp( condition, "!=" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( book_copy_data[i].year_published != value ) {
                        printf( "%d %d %d %d\n", book_copy_data[i].id, book_copy_data[i].book_id,
						        book_copy_data[i].publisher_id, book_copy_data[i].year_published );
					}
				}
			}
            else {
                printf( "conditions invalid\n" );
            }            
		}
        // Invalid condition_var for a book_copy.
        else {
			printf( "conditions invalid\n" );
		}
        free( book_copy_data );
    }
    
    // Table is type member_account
    else if ( strcmp( table_name, "member_account" ) == 0 ) {
        // Set up array of Member_account structs.
        Member_account *member_account_data = ( Member_account * )malloc( MAX_RECORDS * sizeof( Member_account ) );
		if ( member_account_data == NULL ) { 
            printf( "Memory allocation failed\n" ); 
            return EXIT_FAILURE; 
        }

        // Read and process each line. Insert to array of member_accounts.
	    while ( fgets(line, sizeof( line ), file) ) { 
            // Remove newline character if present
	        if ( line[strlen( line ) - 1] == '\n' ) { 
                line[strlen( line ) - 1] = '\0';
            }
			// Process the line
	        char *token = strtok(line, " ");
	        member_account_data[data_count].id = atoi(token);
	        token = strtok( NULL, "\"" );
	        strcpy( member_account_data[data_count].first_name, token );
	        token = strtok( NULL, "\" " );
	        strcpy( member_account_data[data_count].last_name, token );
            token = strtok( NULL, "\" " );
	        strcpy( member_account_data[data_count].email, token );
	        ++data_count;
		}
        
        // If condition_var is "id"...
		if ( strcmp( condition_var, "id" ) == 0 ) {
			int value = atoi( condition_val );
	        // If id matches, print to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( member_account_data[i].id == value ) { 
                        printf( "%d %s %s %s\n", member_account_data[i].id, member_account_data[i].first_name,
						         member_account_data[i].last_name, member_account_data[i].email );
					}
				}
			} 
            // If id does not match, print to console.
            else if ( strcmp(condition, "!=" ) == 0 ) {     
                for ( int i = 0; i < data_count; ++i ) {
					if ( member_account_data[i].id != value ) {
                        printf( "%d %s %s %s\n", member_account_data[i].id, member_account_data[i].first_name,
						         member_account_data[i].last_name, member_account_data[i].email );
					}
				}
            }
            else {
                printf( "conditions invalid\n" );
            }
        } 
        // If condition_var is "first_name"...
        else if ( strcmp( condition_var, "first_name" ) == 0 ) {
			// If first_name matches, print it to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
                    if ( strcmp( member_account_data[i].first_name, condition_val ) == 0 ) {
                        printf( "%d %s %s %s\n", member_account_data[i].id, member_account_data[i].first_name,
						         member_account_data[i].last_name, member_account_data[i].email );
					}
				}
			} 
            // If first_name does not match, print it to console.
            else if ( strcmp( condition, "!=" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( strcmp( member_account_data[i].first_name, condition_val) != 0 ) {
                        printf( "%d %s %s %s\n", member_account_data[i].id, member_account_data[i].first_name,
						         member_account_data[i].last_name, member_account_data[i].email );
					}
				}
			}
            else {
                printf( "conditions invalid\n" );
            }            
		}
        // If condition_var is "last_name"...
        else if ( strcmp( condition_var, "last_name" ) == 0 ) {
			// If last_name matches, print it to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
                    if ( strcmp( member_account_data[i].last_name, condition_val ) == 0 ) {
                        printf( "%d %s %s %s\n", member_account_data[i].id, member_account_data[i].first_name,
						         member_account_data[i].last_name, member_account_data[i].email );
					}
				}
			} 
            // If last_name does not match, print it to console.
            else if ( strcmp( condition, "!=" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( strcmp( member_account_data[i].last_name, condition_val) != 0 ) {
                        printf( "%d %s %s %s\n", member_account_data[i].id, member_account_data[i].first_name,
						         member_account_data[i].last_name, member_account_data[i].email );
					}
				}
			}
            else {
                printf( "conditions invalid\n" );
            }            
		}
        // If condition_var is "email"...
        else if ( strcmp( condition_var, "email" ) == 0 ) {
			// If email matches, print it to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
                    if ( strcmp( member_account_data[i].email, condition_val ) == 0 ) {
                        printf( "%d %s %s %s\n", member_account_data[i].id, member_account_data[i].first_name,
						         member_account_data[i].last_name, member_account_data[i].email );
					}
				}
			} 
            // If email does not match, print it to console.
            else if ( strcmp( condition, "!=" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( strcmp( member_account_data[i].email, condition_val) != 0 ) {
                        printf( "%d %s %s %s\n", member_account_data[i].id, member_account_data[i].first_name,
						         member_account_data[i].last_name, member_account_data[i].email );
					}
				}
			}
            else {
                printf( "conditions invalid\n" );
            }            
		}
        // Invalid condition_var for a book.
        else {
			printf( "conditions invalid\n" );
		}
        free( member_account_data );
    }
    
    // Table is type checkout
    else if ( strcmp( table_name, "checkout" ) == 0 ) {
        // Set up array of checkout structs.
        Checkout *checkout_data = ( Checkout * )malloc( MAX_RECORDS * sizeof( Checkout ) );
		if ( checkout_data == NULL ) {
            printf( "Memory allocation failed\n" );
            return EXIT_FAILURE;
        }
        
        // Read and process each line. Insert to array of checkouts.
	    while ( fgets(line, sizeof( line ), file) ) { 
            // Remove newline character if present
	        if ( line[strlen( line ) - 1] == '\n' ) { 
                line[strlen( line ) - 1] = '\0';
            }
			
			// Process the line
	        char *token = strtok(line, " ");
	        checkout_data[data_count].id = atoi(token);
	        token = strtok( NULL, "-" );
	        checkout_data[data_count].checkout_date.day = atoi( token );
            token = strtok( NULL, "-" );
            checkout_data[data_count].checkout_date.month = atoi( token );
            token = strtok( NULL, " " );
            checkout_data[data_count].checkout_date.year = atoi( token );
            token = strtok( NULL, "-" );
	        checkout_data[data_count].return_date.day = atoi( token );
            token = strtok( NULL, "-" );
            checkout_data[data_count].return_date.month = atoi( token );
            token = strtok( NULL, " " );
            checkout_data[data_count].return_date.year = atoi( token );
            token = strtok( NULL, " " );
            checkout_data[data_count].book_copy_id = atoi( token );
            token = strtok( NULL, " " );
            checkout_data[data_count].member_id = atoi( token );
            token = strtok( NULL, " " );
            checkout_data[data_count].is_returned = atoi( token );
	        ++data_count;
		}
        
        // If condition_var is "id"...
		if ( strcmp( condition_var, "id" ) == 0 ) {
			int value = atoi( condition_val );
	        // If id matches, print to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( checkout_data[i].id == value ) { 
                        printf( "%d %02d-%02d-%4d %02d-%02d-%4d %d %d %d\n", checkout_data[i].id, checkout_data[i].checkout_date.day,
						         checkout_data[i].checkout_date.month, checkout_data[i].checkout_date.year,
                                 checkout_data[i].return_date.day, checkout_data[i].return_date.month,
                                 checkout_data[i].return_date.year, checkout_data[i].book_copy_id,
                                 checkout_data[i].member_id, checkout_data[i].is_returned );
					}
				}
			} 
            // If id does not match, print to console.
            else if ( strcmp(condition, "!=" ) == 0 ) {     
                for ( int i = 0; i < data_count; ++i ) {
					if ( checkout_data[i].id != value ) {
                        printf( "%d %02d-%02d-%4d %02d-%02d-%4d %d %d %d\n", checkout_data[i].id, checkout_data[i].checkout_date.day,
						         checkout_data[i].checkout_date.month, checkout_data[i].checkout_date.year,
                                 checkout_data[i].return_date.day, checkout_data[i].return_date.month,
                                 checkout_data[i].return_date.year, checkout_data[i].book_copy_id,
                                 checkout_data[i].member_id, checkout_data[i].is_returned );
					}
				}
            }
            else {
                printf( "conditions invalid\n" );
            }
        }
        // If condition_var is "checkout_date"
		else if ( strcmp( condition_var, "checkout_date" ) == 0 ) {
            // Tokenize the condition_val to get a day-month-year.
            strcpy( line, condition_val );
            char *token = strtok(line, "-");
            int day = atoi( token );
            token = strtok( NULL, "-" );
            int month = atoi( token );
            token = strtok( NULL, " " );
            int year = atoi( token );

	        // If checkout_date matches, print to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( checkout_data[i].checkout_date.day == day && 
                         checkout_data[i].checkout_date.month == month &&
                         checkout_data[i].checkout_date.year == year ) { 
                        printf( "%d %02d-%02d-%4d %02d-%02d-%4d %d %d %d\n", checkout_data[i].id, checkout_data[i].checkout_date.day,
						         checkout_data[i].checkout_date.month, checkout_data[i].checkout_date.year,
                                 checkout_data[i].return_date.day, checkout_data[i].return_date.month,
                                 checkout_data[i].return_date.year, checkout_data[i].book_copy_id,
                                 checkout_data[i].member_id, checkout_data[i].is_returned );
					}
				}
			} 
            // If checkout_date does not match, print to console.
            else if ( strcmp(condition, "!=" ) == 0 ) {     
                for ( int i = 0; i < data_count; ++i ) {
					if ( checkout_data[i].checkout_date.day != day || 
                         checkout_data[i].checkout_date.month != month ||
                         checkout_data[i].checkout_date.year != year ) {
                        printf( "%d %02d-%02d-%4d %02d-%02d-%4d %d %d %d\n", checkout_data[i].id, checkout_data[i].checkout_date.day,
						         checkout_data[i].checkout_date.month, checkout_data[i].checkout_date.year,
                                 checkout_data[i].return_date.day, checkout_data[i].return_date.month,
                                 checkout_data[i].return_date.year, checkout_data[i].book_copy_id,
                                 checkout_data[i].member_id, checkout_data[i].is_returned );
					}
				}
            }
            else {
                printf( "conditions invalid\n" );
            }
        }
        // If condition_var is "return_date"
		else if ( strcmp( condition_var, "return_date" ) == 0 ) {
            // Tokenize the condition_val to get a day-month-year.
            strcpy( line, condition_val );
            char *token = strtok(line, "-");
            int day = atoi( token );
            token = strtok( NULL, "-" );
            int month = atoi( token );
            token = strtok( NULL, " " );
            int year = atoi( token );

	        // If return_date matches, print to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( checkout_data[i].return_date.day == day && 
                         checkout_data[i].return_date.month == month &&
                         checkout_data[i].return_date.year == year ) { 
                        printf( "%d %02d-%02d-%4d %02d-%02d-%4d %d %d %d\n", checkout_data[i].id, checkout_data[i].checkout_date.day,
						         checkout_data[i].checkout_date.month, checkout_data[i].checkout_date.year,
                                 checkout_data[i].return_date.day, checkout_data[i].return_date.month,
                                 checkout_data[i].return_date.year, checkout_data[i].book_copy_id,
                                 checkout_data[i].member_id, checkout_data[i].is_returned );
					}
				}
			} 
            // If return_date does not match, print to console.
            else if ( strcmp(condition, "!=" ) == 0 ) {     
                for ( int i = 0; i < data_count; ++i ) {
					if ( checkout_data[i].return_date.day != day || 
                         checkout_data[i].return_date.month != month ||
                         checkout_data[i].return_date.year != year ) {
                        printf( "%d %02d-%02d-%4d %02d-%02d-%4d %d %d %d\n", checkout_data[i].id, checkout_data[i].checkout_date.day,
						         checkout_data[i].checkout_date.month, checkout_data[i].checkout_date.year,
                                 checkout_data[i].return_date.day, checkout_data[i].return_date.month,
                                 checkout_data[i].return_date.year, checkout_data[i].book_copy_id,
                                 checkout_data[i].member_id, checkout_data[i].is_returned );
					}
				}
            }
            else {
                printf( "conditions invalid\n" );
            }
        }
        // If condition_var is "book_copy_id"...
		else if ( strcmp( condition_var, "book_copy_id" ) == 0 ) {
			int value = atoi( condition_val );
	        // If book_copy_id matches, print to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( checkout_data[i].book_copy_id == value ) { 
                        printf( "%d %02d-%02d-%4d %02d-%02d-%4d %d %d %d\n", checkout_data[i].id, checkout_data[i].checkout_date.day,
						         checkout_data[i].checkout_date.month, checkout_data[i].checkout_date.year,
                                 checkout_data[i].return_date.day, checkout_data[i].return_date.month,
                                 checkout_data[i].return_date.year, checkout_data[i].book_copy_id,
                                 checkout_data[i].member_id, checkout_data[i].is_returned );
					}
				}
			} 
            // If book_copy_id does not match, print to console.
            else if ( strcmp(condition, "!=" ) == 0 ) {     
                for ( int i = 0; i < data_count; ++i ) {
					if ( checkout_data[i].book_copy_id != value ) {
                        printf( "%d %02d-%02d-%4d %02d-%02d-%4d %d %d %d\n", checkout_data[i].id, checkout_data[i].checkout_date.day,
						         checkout_data[i].checkout_date.month, checkout_data[i].checkout_date.year,
                                 checkout_data[i].return_date.day, checkout_data[i].return_date.month,
                                 checkout_data[i].return_date.year, checkout_data[i].book_copy_id,
                                 checkout_data[i].member_id, checkout_data[i].is_returned );
					}
				}
            }
            else {
                printf( "conditions invalid\n" );
            }
        }
        // If condition_var is "member_id"...
		else if ( strcmp( condition_var, "member_id" ) == 0 ) {
			int value = atoi( condition_val );
	        // If member_id matches, print to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( checkout_data[i].member_id == value ) { 
                        printf( "%d %02d-%02d-%4d %02d-%02d-%4d %d %d %d\n", checkout_data[i].id, checkout_data[i].checkout_date.day,
						         checkout_data[i].checkout_date.month, checkout_data[i].checkout_date.year,
                                 checkout_data[i].return_date.day, checkout_data[i].return_date.month,
                                 checkout_data[i].return_date.year, checkout_data[i].book_copy_id,
                                 checkout_data[i].member_id, checkout_data[i].is_returned );
					}
				}
			} 
            // If member_id does not match, print to console.
            else if ( strcmp(condition, "!=" ) == 0 ) {     
                for ( int i = 0; i < data_count; ++i ) {
					if ( checkout_data[i].member_id != value ) {
                        printf( "%d %02d-%02d-%4d %02d-%02d-%4d %d %d %d\n", checkout_data[i].id, checkout_data[i].checkout_date.day,
						         checkout_data[i].checkout_date.month, checkout_data[i].checkout_date.year,
                                 checkout_data[i].return_date.day, checkout_data[i].return_date.month,
                                 checkout_data[i].return_date.year, checkout_data[i].book_copy_id,
                                 checkout_data[i].member_id, checkout_data[i].is_returned );
					}
				}
            }
            else {
                printf( "conditions invalid\n" );
            }
        }
        // If condition_var is "is_returned"...
		else if ( strcmp( condition_var, "is_returned" ) == 0 ) {
			int value = atoi( condition_val );
            // Convert to just 0 or 1 depending on user input
            value = value > 0 ? 1 : 0;
            
	        // If is_returned matches, print to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( checkout_data[i].is_returned == value ) { 
                        printf( "%d %02d-%02d-%4d %02d-%02d-%4d %d %d %d\n", checkout_data[i].id, checkout_data[i].checkout_date.day,
						         checkout_data[i].checkout_date.month, checkout_data[i].checkout_date.year,
                                 checkout_data[i].return_date.day, checkout_data[i].return_date.month,
                                 checkout_data[i].return_date.year, checkout_data[i].book_copy_id,
                                 checkout_data[i].member_id, checkout_data[i].is_returned );
					}
				}
			} 
            // If is_returned does not match, print to console.
            else if ( strcmp(condition, "!=" ) == 0 ) {     
                for ( int i = 0; i < data_count; ++i ) {
					if ( checkout_data[i].is_returned != value ) {
                        printf( "%d %02d-%02d-%4d %02d-%02d-%4d %d %d %d\n", checkout_data[i].id, checkout_data[i].checkout_date.day,
						         checkout_data[i].checkout_date.month, checkout_data[i].checkout_date.year,
                                 checkout_data[i].return_date.day, checkout_data[i].return_date.month,
                                 checkout_data[i].return_date.year, checkout_data[i].book_copy_id,
                                 checkout_data[i].member_id, checkout_data[i].is_returned );
					}
				}
            }
            else {
                printf( "conditions invalid\n" );
            }
        }
        // Invalid condition_var for a checkout.
        else {
			printf( "conditions invalid\n" );
		}
        free( checkout_data );
    }
    
    // Table is type hold
    else if ( strcmp( table_name, "hold" ) == 0 ) {
        // Set up array of hold structs.
        Hold *hold_data = ( Hold * )malloc( MAX_RECORDS * sizeof( Hold ) );
		if ( hold_data == NULL ) {
            printf( "Memory allocation failed\n" );
            return EXIT_FAILURE;
        }
        
        // Read and process each line. Insert to array of holds.
	    while ( fgets(line, sizeof( line ), file) ) { 
            // Remove newline character if present
	        if ( line[strlen( line ) - 1] == '\n' ) { 
                line[strlen( line ) - 1] = '\0';
            }
			
			// Process the line
	        char *token = strtok(line, " ");
	        hold_data[data_count].id = atoi(token);
	        token = strtok( NULL, "-" );
	        hold_data[data_count].checkout_date.day = atoi( token );
            token = strtok( NULL, "-" );
            hold_data[data_count].checkout_date.month = atoi( token );
            token = strtok( NULL, " " );
            hold_data[data_count].checkout_date.year = atoi( token );
            token = strtok( NULL, "-" );
	        hold_data[data_count].return_date.day = atoi( token );
            token = strtok( NULL, "-" );
            hold_data[data_count].return_date.month = atoi( token );
            token = strtok( NULL, " " );
            hold_data[data_count].return_date.year = atoi( token );
            token = strtok( NULL, " " );
            hold_data[data_count].book_copy_id = atoi( token );
            token = strtok( NULL, " " );
            hold_data[data_count].member_id = atoi( token );
	        ++data_count;
		}
        
        // If condition_var is "id"...
		if ( strcmp( condition_var, "id" ) == 0 ) {
			int value = atoi( condition_val );
	        // If id matches, print to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( hold_data[i].id == value ) { 
                        printf( "%d %02d-%02d-%4d %02d-%02d-%4d %d %d\n", hold_data[i].id, hold_data[i].checkout_date.day,
						         hold_data[i].checkout_date.month, hold_data[i].checkout_date.year,
                                 hold_data[i].return_date.day, hold_data[i].return_date.month,
                                 hold_data[i].return_date.year, hold_data[i].book_copy_id,
                                 hold_data[i].member_id );
					}
				}
			} 
            // If id does not match, print to console.
            else if ( strcmp(condition, "!=" ) == 0 ) {     
                for ( int i = 0; i < data_count; ++i ) {
					if ( hold_data[i].id != value ) {
                        printf( "%d %02d-%02d-%4d %02d-%02d-%4d %d %d\n", hold_data[i].id, hold_data[i].checkout_date.day,
						         hold_data[i].checkout_date.month, hold_data[i].checkout_date.year,
                                 hold_data[i].return_date.day, hold_data[i].return_date.month,
                                 hold_data[i].return_date.year, hold_data[i].book_copy_id,
                                 hold_data[i].member_id );
					}
				}
            }
            else {
                printf( "conditions invalid\n" );
            }
        }
        // If condition_var is "checkout_date"
		else if ( strcmp( condition_var, "checkout_date" ) == 0 ) {
            // Tokenize the condition_val to get a day-month-year.
            strcpy( line, condition_val );
            char *token = strtok(line, "-");
            int day = atoi( token );
            token = strtok( NULL, "-" );
            int month = atoi( token );
            token = strtok( NULL, " " );
            int year = atoi( token );

	        // If checkout_date matches, print to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( hold_data[i].checkout_date.day == day && 
                         hold_data[i].checkout_date.month == month &&
                         hold_data[i].checkout_date.year == year ) { 
                        printf( "%d %02d-%02d-%4d %02d-%02d-%4d %d %d\n", hold_data[i].id, hold_data[i].checkout_date.day,
						         hold_data[i].checkout_date.month, hold_data[i].checkout_date.year,
                                 hold_data[i].return_date.day, hold_data[i].return_date.month,
                                 hold_data[i].return_date.year, hold_data[i].book_copy_id,
                                 hold_data[i].member_id );
					}
				}
			} 
            // If checkout_date does not match, print to console.
            else if ( strcmp(condition, "!=" ) == 0 ) {     
                for ( int i = 0; i < data_count; ++i ) {
					if ( hold_data[i].checkout_date.day != day || 
                         hold_data[i].checkout_date.month != month ||
                         hold_data[i].checkout_date.year != year ) {
                        printf( "%d %02d-%02d-%4d %02d-%02d-%4d %d %d\n", hold_data[i].id, hold_data[i].checkout_date.day,
						         hold_data[i].checkout_date.month, hold_data[i].checkout_date.year,
                                 hold_data[i].return_date.day, hold_data[i].return_date.month,
                                 hold_data[i].return_date.year, hold_data[i].book_copy_id,
                                 hold_data[i].member_id );
					}
				}
            }
            else {
                printf( "conditions invalid\n" );
            }
        }
        // If condition_var is "return_date"
		else if ( strcmp( condition_var, "return_date" ) == 0 ) {
            // Tokenize the condition_val to get a day-month-year.
            strcpy( line, condition_val );
            char *token = strtok(line, "-");
            int day = atoi( token );
            token = strtok( NULL, "-" );
            int month = atoi( token );
            token = strtok( NULL, " " );
            int year = atoi( token );

	        // If return_date matches, print to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( hold_data[i].return_date.day == day && 
                         hold_data[i].return_date.month == month &&
                         hold_data[i].return_date.year == year ) { 
                        printf( "%d %02d-%02d-%4d %02d-%02d-%4d %d %d\n", hold_data[i].id, hold_data[i].checkout_date.day,
						         hold_data[i].checkout_date.month, hold_data[i].checkout_date.year,
                                 hold_data[i].return_date.day, hold_data[i].return_date.month,
                                 hold_data[i].return_date.year, hold_data[i].book_copy_id,
                                 hold_data[i].member_id );
					}
				}
			} 
            // If return_date does not match, print to console.
            else if ( strcmp(condition, "!=" ) == 0 ) {     
                for ( int i = 0; i < data_count; ++i ) {
					if ( hold_data[i].return_date.day != day || 
                         hold_data[i].return_date.month != month ||
                         hold_data[i].return_date.year != year ) {
                        printf( "%d %02d-%02d-%4d %02d-%02d-%4d %d %d\n", hold_data[i].id, hold_data[i].checkout_date.day,
						         hold_data[i].checkout_date.month, hold_data[i].checkout_date.year,
                                 hold_data[i].return_date.day, hold_data[i].return_date.month,
                                 hold_data[i].return_date.year, hold_data[i].book_copy_id,
                                 hold_data[i].member_id );
					}
				}
            }
            else {
                printf( "conditions invalid\n" );
            }
        }
        // If condition_var is "book_copy_id"...
		else if ( strcmp( condition_var, "book_copy_id" ) == 0 ) {
			int value = atoi( condition_val );
	        // If book_copy_id matches, print to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( hold_data[i].book_copy_id == value ) { 
                        printf( "%d %02d-%02d-%4d %02d-%02d-%4d %d %d\n", hold_data[i].id, hold_data[i].checkout_date.day,
						         hold_data[i].checkout_date.month, hold_data[i].checkout_date.year,
                                 hold_data[i].return_date.day, hold_data[i].return_date.month,
                                 hold_data[i].return_date.year, hold_data[i].book_copy_id,
                                 hold_data[i].member_id );
					}
				}
			} 
            // If book_copy_id does not match, print to console.
            else if ( strcmp(condition, "!=" ) == 0 ) {     
                for ( int i = 0; i < data_count; ++i ) {
					if ( hold_data[i].book_copy_id != value ) {
                        printf( "%d %02d-%02d-%4d %02d-%02d-%4d %d %d\n", hold_data[i].id, hold_data[i].checkout_date.day,
						         hold_data[i].checkout_date.month, hold_data[i].checkout_date.year,
                                 hold_data[i].return_date.day, hold_data[i].return_date.month,
                                 hold_data[i].return_date.year, hold_data[i].book_copy_id,
                                 hold_data[i].member_id );
					}
				}
            }
            else {
                printf( "conditions invalid\n" );
            }
        }
        // If condition_var is "member_id"...
		else if ( strcmp( condition_var, "member_id" ) == 0 ) {
			int value = atoi( condition_val );
	        // If member_id matches, print to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( hold_data[i].member_id == value ) { 
                        printf( "%d %02d-%02d-%4d %02d-%02d-%4d %d %d\n", hold_data[i].id, hold_data[i].checkout_date.day,
						         hold_data[i].checkout_date.month, hold_data[i].checkout_date.year,
                                 hold_data[i].return_date.day, hold_data[i].return_date.month,
                                 hold_data[i].return_date.year, hold_data[i].book_copy_id,
                                 hold_data[i].member_id );
					}
				}
			} 
            // If member_id does not match, print to console.
            else if ( strcmp(condition, "!=" ) == 0 ) {     
                for ( int i = 0; i < data_count; ++i ) {
					if ( hold_data[i].member_id != value ) {
                        printf( "%d %02d-%02d-%4d %02d-%02d-%4d %d %d\n", hold_data[i].id, hold_data[i].checkout_date.day,
						         hold_data[i].checkout_date.month, hold_data[i].checkout_date.year,
                                 hold_data[i].return_date.day, hold_data[i].return_date.month,
                                 hold_data[i].return_date.year, hold_data[i].book_copy_id,
                                 hold_data[i].member_id );
					}
				}
            }
            else {
                printf( "conditions invalid\n" );
            }
        }
        // Invalid hold_data for a hold.
        else {
			printf( "conditions invalid\n" );
		}
        free( hold_data );   
    }
    
    // Table is type waitlist
    else if ( strcmp( table_name, "waitlist" ) == 0 ) {
        // Set up array of waitlist structs.
        Waitlist *waitlist_data = ( Waitlist * )malloc( MAX_RECORDS * sizeof( Waitlist ) );
		if ( waitlist_data == NULL ) {
            printf( "Memory allocation failed\n" );
            return EXIT_FAILURE;
        }
        
        // Read and process each line. Insert to array of waitlists.
	    while ( fgets(line, sizeof( line ), file) ) { 
            // Remove newline character if present
	        if ( line[strlen( line ) - 1] == '\n' ) { 
                line[strlen( line ) - 1] = '\0';
            }
			
			// Process the line
	        char *token = strtok(line, " ");
	        waitlist_data[data_count].book_id = atoi(token);
	        token = strtok( NULL, " " );
	        waitlist_data[data_count].member_id = atoi( token );
	        ++data_count;
		}
        
        // If condition_var is "book_id"...
		if ( strcmp( condition_var, "book_id" ) == 0 ) {
			int value = atoi( condition_val );
	        // If book_id matches, print to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( waitlist_data[i].book_id == value ) { 
                        printf( "%d %d\n", waitlist_data[i].book_id, 
						                   waitlist_data[i].member_id );
					}
				}
			} 
            // If book_id does not match, print to console.
            else if ( strcmp( condition, "!=" ) == 0) {     
                for ( int i = 0; i < data_count; ++i ) {
					if ( waitlist_data[i].book_id != value ) {
						printf( "%d %d\n", waitlist_data[i].book_id,
						                   waitlist_data[i].member_id );
					}
				}
            }
            else {
                printf( "conditions invalid\n" );
            }
        }
        // If condition_var is member_id
        else if ( strcmp( condition_var, "member_id" ) == 0 ) { 
            int value = atoi( condition_val );
            // If member_id matches, print it to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
                    if ( waitlist_data[i].member_id == value ) {
						printf( "%d %d\n", waitlist_data[i].book_id, 
						                   waitlist_data[i].member_id );
					}
				}
			} 
            // If member_id does not match, print it to console.
            else if ( strcmp( condition, "!=" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( waitlist_data[i].member_id != value ) {
						printf( "%d %d\n", waitlist_data[i].book_id,
						                   waitlist_data[i].member_id );
					}
				}
			}
            else {
                printf( "conditions invalid\n" );
            }            
		}
        // Invalid condition_var for a waitlist.
        else {
			printf( "conditions invalid\n" );
		}
        free( waitlist_data );
    }
    
    // Table is type notification
    else if ( strcmp( table_name, "notification" ) == 0 ) {
        // Set up array of notification structs.
        Notification *notification_data = ( Notification * )malloc( MAX_RECORDS * sizeof( Notification ) );
		if ( notification_data == NULL ) {
            printf( "Memory allocation failed\n" );
            return EXIT_FAILURE;
        }
        
        // Read and process each line. Insert to array of holds.
	    while ( fgets(line, sizeof( line ), file) ) { 
            // Remove newline character if present
	        if ( line[strlen( line ) - 1] == '\n' ) { 
                line[strlen( line ) - 1] = '\0';
            }
			
			// Process the line
	        char *token = strtok(line, " ");
	        notification_data[data_count].id = atoi(token);
	        token = strtok( NULL, "-" );
	        notification_data[data_count].sent_at.day = atoi( token );
            token = strtok( NULL, "-" );
            notification_data[data_count].sent_at.month = atoi( token );
            token = strtok( NULL, " " );
            notification_data[data_count].sent_at.year = atoi( token );
            token = strtok( NULL, " " );
            notification_data[data_count].member_id = atoi( token );
            token = strtok( NULL, "\"" );
            strcpy( notification_data[data_count].message, token );
	        ++data_count;
		}
        
        // If condition_var is "id"...
		if ( strcmp( condition_var, "id" ) == 0 ) {
			int value = atoi( condition_val );
	        // If id matches, print to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( notification_data[i].id == value ) { 
                        printf( "%d %02d-%02d-%4d %d %s\n", notification_data[i].id, notification_data[i].sent_at.day,
						         notification_data[i].sent_at.month, notification_data[i].sent_at.year,
                                 notification_data[i].member_id, notification_data[i].message );
					}
				}
			} 
            // If id does not match, print to console.
            else if ( strcmp(condition, "!=" ) == 0 ) {     
                for ( int i = 0; i < data_count; ++i ) {
					if ( notification_data[i].id != value ) {
                        printf( "%d %02d-%02d-%4d %d %s\n", notification_data[i].id, notification_data[i].sent_at.day,
						         notification_data[i].sent_at.month, notification_data[i].sent_at.year,
                                 notification_data[i].member_id, notification_data[i].message );
					}
				}
            }
            else {
                printf( "conditions invalid\n" );
            }
        }
        // If condition_var is "sent_at"
		else if ( strcmp( condition_var, "sent_at" ) == 0 ) {
            // Tokenize the condition_val to get a day-month-year.
            strcpy( line, condition_val );
            char *token = strtok(line, "-");
            int day = atoi( token );
            token = strtok( NULL, "-" );
            int month = atoi( token );
            token = strtok( NULL, " " );
            int year = atoi( token );

	        // If sent_at matches, print to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( notification_data[i].sent_at.day == day && 
                         notification_data[i].sent_at.month == month &&
                         notification_data[i].sent_at.year == year ) { 
                        printf( "%d %02d-%02d-%4d %d %s\n", notification_data[i].id, notification_data[i].sent_at.day,
						         notification_data[i].sent_at.month, notification_data[i].sent_at.year,
                                 notification_data[i].member_id, notification_data[i].message );
					}
				}
			} 
            // If sent_at does not match, print to console.
            else if ( strcmp(condition, "!=" ) == 0 ) {     
                for ( int i = 0; i < data_count; ++i ) {
					if ( notification_data[i].sent_at.day != day || 
                         notification_data[i].sent_at.month != month ||
                         notification_data[i].sent_at.year != year ) {
                        printf( "%d %02d-%02d-%4d %d %s\n", notification_data[i].id, notification_data[i].sent_at.day,
						         notification_data[i].sent_at.month, notification_data[i].sent_at.year,
                                 notification_data[i].member_id, notification_data[i].message );
					}
				}
            }
            else {
                printf( "conditions invalid\n" );
            }
        }
        // If condition_var is "member_id"...
		else if ( strcmp( condition_var, "member_id" ) == 0 ) {
			int value = atoi( condition_val );
	        // If member_id matches, print to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( notification_data[i].member_id == value ) { 
                        printf( "%d %02d-%02d-%4d %d %s\n", notification_data[i].id, notification_data[i].sent_at.day,
						         notification_data[i].sent_at.month, notification_data[i].sent_at.year,
                                 notification_data[i].member_id, notification_data[i].message );
					}
				}
			} 
            // If member_id does not match, print to console.
            else if ( strcmp(condition, "!=" ) == 0 ) {     
                for ( int i = 0; i < data_count; ++i ) {
					if ( notification_data[i].member_id != value ) {
                        printf( "%d %02d-%02d-%4d %d %s\n", notification_data[i].id, notification_data[i].sent_at.day,
						         notification_data[i].sent_at.month, notification_data[i].sent_at.year,
                                 notification_data[i].member_id, notification_data[i].message );
					}
				}
            }
            else {
                printf( "conditions invalid\n" );
            }
        }
        // If condition_var is "message"...
        else if ( strcmp( condition_var, "message" ) == 0 ) {
			// If message matches, print it to console.
            if ( strcmp( condition, "==" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
                    if ( strcmp( notification_data[i].message, condition_val ) == 0 ) {
                        printf( "%d %02d-%02d-%4d %d %s\n", notification_data[i].id, notification_data[i].sent_at.day,
						         notification_data[i].sent_at.month, notification_data[i].sent_at.year,
                                 notification_data[i].member_id, notification_data[i].message );
					}
				}
			} 
            // If message does not match, print it to console.
            else if ( strcmp( condition, "!=" ) == 0 ) {
                for ( int i = 0; i < data_count; ++i ) {
					if ( strcmp( notification_data[i].message, condition_val) != 0 ) {
                        printf( "%d %02d-%02d-%4d %d %s\n", notification_data[i].id, notification_data[i].sent_at.day,
						         notification_data[i].sent_at.month, notification_data[i].sent_at.year,
                                 notification_data[i].member_id, notification_data[i].message );
					}
				}
			}
            else {
                printf( "conditions invalid\n" );
            }            
		}
        // Invalid notification_data for a notification.
        else {
			printf( "conditions invalid\n" );
		}
        free( notification_data );   
    }
    
    // Table is not part of defined databases in database.h 
    else {
        printf( "Defined databases for selction include book, category, author, book_author, "
                 "publisher, book_copy, member_account, checkout, hold, waitlist, notification\n" );
    }
    
	return EXIT_SUCCESS;
}

/** This function is defined to write entire database into a file. */
int write_database_file( const char *table_name ){
    // File to write to cannot match one of the database names.
    char *databases[] = { "book", "category", "author", "book_author", "publisher", "book_copy",
                          "member_account", "checkout", "hold", "waitlist", "notification" };
    
    // Variable to track for an error condition (param matches existing table, no tables found)
    bool tables_exist = false;
    
    // Temporary File to write output to. Renamed to parameter if there are no errors in loop.
    // *** This method of saving a temp file, renaming, and removing was approved in piazza. ***
    FILE *temp = fopen( "./tables/temp", "w" );
    if ( temp == NULL ) {
        printf( "Unable to create database file\n" );
    }

    // Loop through possible tables and print contents to output file if possible (no errors).
    for ( int i = 0; i < DATABASE_SIZE; i++ ) {
        // Store pathnames in a variable.
	    char filepath[MAX_STR_LENGTH];                                                                  
        snprintf( filepath, sizeof(filepath), "%s/%s", folder, databases[i] );
        
        // Begin attempting to set up files to read in and print out.
        FILE *input = fopen( filepath, "r" );
        
        // If able to read (table exists) check for same param name, otherwise write to temp.
        if ( input != NULL ) {
            if ( strcmp( table_name, databases[i] ) == 0 ) {
                printf( "File already exist!\n" );
                fclose( temp );
                fclose( input );
                remove( "./tables/temp" );
                return EXIT_FAILURE;
            }
            else {
                tables_exist = true;
                // Print current input file's name/header at the start of database output file.
                fprintf( temp, "%s\n\n", databases[i] ); 
                
                // write contents from current input into temp line by line.
                char line[MAX_STR_LENGTH];
                while ( fgets(line, sizeof(line), input) ) {
                    fprintf( temp, line );
                }
                fprintf( temp, "\n" );
            }
            // Close the current input file. 
            fclose( input );
        }
    }
    
    // Rename temp file to parameter table/file name. Close the stream.
    char renamePath[MAX_STR_LENGTH];
    snprintf( renamePath, sizeof(renamePath), "%s/%s", folder, table_name );
    rename( "./tables/temp", renamePath );
    fclose( temp );
    
    // If no tables were previously written/found (output is empty), delete latest file, return fail.
    if ( !tables_exist ) {
        printf( "No table found!" );
        remove( renamePath );
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/** Updates data on matching table-->row with attributes parameter. */
int update( const char *table_name, const char *table_row, const char *attributes ) {
    // Set up filepath to read from.
    char filepath[MAX_STR_LENGTH];
    snprintf( filepath, sizeof(filepath), "%s/%s", folder, table_name );
    
    // Check if the file exists and print error if it doesn't.
    FILE *fileIn = fopen( filepath, "r" );
    FILE *temp = fopen( "./tables/temp", "w" );
    if ( fileIn != NULL && temp != NULL && table_exist( table_name ) == EXIT_SUCCESS ) {
        // Read in each line of a table. Check if row param matches line row.
        char idValue[ID_LENGTH];
        char line[MAX_STR_LENGTH];
        bool rowFound = false;
        while ( fgets(line, sizeof(line), fileIn) ) {
            // Scan the line for the immediate id value.
            sscanf( line, "%[0-9]", idValue );
            // Print out line from input to temp file if row/id does not match parameter.
            if ( strcmp( idValue, table_row ) != 0 ) {
                fprintf( temp, line );
            }
            else {
                // match was found. Print out line/row id to temp, then print updated attributes.
                rowFound = true;
                fprintf( temp, "%s %s\n", idValue, attributes );
            }
        }
        
        // Close input file.
        fclose( fileIn );
        
        // If matching row not found, print error, close files, delete temp, and return failure.
        if ( !rowFound ) {
            printf( "Record not found!\n" );
            fclose( temp );
            remove( "./tables/temp" );
            return EXIT_FAILURE;
        }
        
        // Close previous file, delete it, rename temp file to original file name, close renamed file.
        printf( "Record updated successfully!\n" );
        remove ( filepath );
        rename( "./tables/temp", filepath );
        fclose( temp );
        return EXIT_SUCCESS; 
    }
    else {
        printf( "Table %s not found!\n", table_name );
        return EXIT_FAILURE; 
    }
}

/** Deletes a single row from a table if found. */
int delete_row( const char *table_name, const char *table_row ) {
    // Check for invalid row
    if ( table_row == NULL ) {
        printf( "Record id not found!\n" );
        return EXIT_FAILURE;
    }
    
    // Set up filepath given table name.
    char filepath[MAX_STR_LENGTH];
    snprintf( filepath, sizeof(filepath), "%s/%s", folder, table_name );

    // Check if the file exists and print error if it doesn't. Set up temporary output file.
    FILE *fileIn = fopen( filepath, "r" );
    FILE *temp = fopen( "./tables/temp", "w" );
    if ( fileIn != NULL && temp != NULL && table_exist( table_name ) == EXIT_SUCCESS ) {
        // Read in each line of a table. Check if row param matches line row.
        char idValue[ID_LENGTH];
        char line[MAX_STR_LENGTH];
        bool rowFound = false;
        while ( fgets(line, sizeof(line), fileIn) ) {
            // Scan the line for the immediate id value.
            sscanf( line, "%[0-9]", idValue );
            // Only print out line to temp file if row/id does not match parameter.
            if ( strcmp( idValue, table_row ) != 0 ) {
                fprintf( temp, line );
            }
            else {
                rowFound = true;    // match was found, does not get printed to temp file.
            }
        }
        
        // Close input file.
        fclose( fileIn );
        
        // If matching row not found, print error, close files, delete temp, and return failure.
        if ( !rowFound ) {
            printf( "Record id not found!\n" );
            fclose( temp );
            remove( "./tables/temp" );
            return EXIT_FAILURE;
        }
        
        // Close previous file, delete it, rename temp file to original file name, close renamed file.
        printf( "Record deleted successfully!\n" );
        remove ( filepath );
        rename( "./tables/temp", filepath );
        fclose( temp );
        return EXIT_SUCCESS; 
    }
    else {
        printf( "Table %s does not exist!\n", table_name );
        return EXIT_FAILURE; 
    }
}

/** Deletes an entire table matching the parameter name. */
int drop_database_file( const char *table_name ) {
    char filepath[MAX_STR_LENGTH];
    snprintf( filepath, sizeof(filepath), "%s/%s", folder, table_name );
    
    // Check if the file exists and print error if it doesn't.
    if ( access( filepath, F_OK ) != -1 ) {
        // File exist at filepath, delete it.
        remove( filepath );
        printf( "Table dropped successfully!\n" );
        return EXIT_SUCCESS; 
    }
    else {
        printf( "Table %s does not exist!!\n", table_name );
        return EXIT_FAILURE; 
    }
}