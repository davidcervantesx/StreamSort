// PID: 730327199
// I pledge the COMP211 honor code.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** get_all_chars( size_t *num_blocks, size_t *num_strings);
char** find_strings( char **all_chars, size_t num_blocks, size_t num_strings);
size_t add_strings_in_block( char **all_strings, char *block, size_t string_index);
void sort_strings( char **strings, size_t num_strings);
void print_strings( char **strings, size_t num_strings);

void validate_allocation( void *ptr);
int already_sorted( char **strings, size_t num_strings);
int my_strcmp( const void *str1, const void *str2);


int main(){
    size_t num_blocks = 0;
    size_t num_strings = 0; 
    char **all_chars = get_all_chars( &num_blocks, &num_strings);

    char **all_strings = find_strings( all_chars, num_blocks, num_strings); 

    sort_strings( all_strings, num_strings);

    print_strings( all_strings, num_strings); 

    // free resources 
    for( size_t i = 0; i < num_blocks; i++){
        free( all_chars[ i]);
    }
    free( all_chars);
    free( all_strings);

    return EXIT_SUCCESS;
}


/* returns a dynamically allocated array of dynamically allocated arrays to char.
 *      each inner array represents a block of memory that holds multiple null
 *          terminated strings. Two null characters signal the end of the block
 *      The outer array holds pointer to each of these blocks.
 * *Each index of the outer array must eventually be freed.
 * *The outer array must also be freed. 
 */
char** get_all_chars( size_t *num_blocks, size_t *num_strings){
    // how much to (arithmetically) grow total_blocks by
    const int TOTAL_BLOCKS_GROWTH = 2;

    // initialize the outer array (described above function)
    size_t total_blocks = 2;
    char **memory_blocks = malloc( total_blocks * sizeof( char*));
    validate_allocation( memory_blocks);

    // character growth factor numerator and denominator.
    const int GF_NUM = 4;
    const int GF_DEN = 3; 

    // initialize the first memory block
    size_t total_chars = 20;
    char *current_block = malloc( total_chars * sizeof( char));
    validate_allocation( current_block);
    size_t pos_current_block = 0; // index in outer array
    memory_blocks[ pos_current_block] = current_block;

    // positional info about where we are/just were within the memory blocks
    char *current_char = current_block;
    size_t pos_current_char = 0; // index in current_block
    char *prev_string = NULL;

    size_t strings_in_block = 0;
    size_t total_strings = 0;

    while( fgets( current_char, total_chars - pos_current_char, stdin)){  
        prev_string = current_char;
        current_char = strchr( current_char, '\0') + 1;
        pos_current_char = current_char - current_block;

        ++strings_in_block;
        ++total_strings;

        if( pos_current_char == total_chars){ 
            // current max reached,
            
            int string_finished = (current_block[ pos_current_char - 2] == '\n');

            --strings_in_block;
            --total_strings;            
            if( strings_in_block == 0){
                // the incomplete string
                // starts at the begginning of the block
                prev_string = current_block;
            }

            ++pos_current_block;
            if( pos_current_block == total_blocks){
                // out of memory blocks.
                total_blocks += TOTAL_BLOCKS_GROWTH; // Grows logarithmically with input,
                    // theirfore arithmetic growth is enough to amortize to O(1).
                memory_blocks = realloc( memory_blocks, total_blocks * sizeof( char*)); 
                validate_allocation( memory_blocks);
            }

            // create new memory block
            total_chars = (total_chars * GF_NUM) / GF_DEN; 
            current_block = malloc( total_chars * sizeof( char));
            validate_allocation( current_block); 
            memory_blocks[ pos_current_block] = current_block;

            // move unfinished string into new block
            current_char = current_block;
            strcpy( current_char, prev_string);

            strings_in_block = 0;

            // update positional info
            pos_current_char = strchr( prev_string, '\0') - prev_string;
            *prev_string = '\0'; // '\0' used to find end of block.
            current_char = current_block + pos_current_char;
            if( string_finished){
                //should not overwrite '\0'
                ++pos_current_char;
                ++current_char;
                //should count as a string
                ++total_strings;
                ++strings_in_block;
            }
        }
    }
    *current_char = '\0'; // '\0' used to find end of block

    *num_strings = total_strings;
    *num_blocks = pos_current_block + 1;
    return memory_blocks;
}

/* returns dynamically allocated array of strings, 
 *      one for each string in all_chars.
 * *must eventually free returned array.
 */
char** find_strings( char **all_chars, size_t num_blocks, size_t num_strings){
    char **all_strings = malloc( num_strings * sizeof( char*)); 
    validate_allocation( all_strings);

    size_t string_index = 0;
    for( size_t i = 0; i < num_blocks; i++){
        string_index = add_strings_in_block( all_strings, all_chars[ i], string_index);
    }

    return all_strings;
}

size_t add_strings_in_block( char **all_strings, char *block, size_t string_index){
    char *beg_next_string = block;
    for( ; *beg_next_string != '\0'; string_index++){
        all_strings[ string_index] = beg_next_string;
        beg_next_string = strchr( beg_next_string, '\0') + 1;
    }
    return string_index;
}

void sort_strings( char** strings, size_t num_strings){
    if( !already_sorted( strings, num_strings)){
        qsort( 
            strings, 
            num_strings, 
            sizeof( char*),
            my_strcmp
        );
    }


}

void print_strings( char **strings, size_t num_strings){
    for( size_t i = 0; i < num_strings; i++){
        printf( "%s", strings[ i]);
    } 
}

// immediately exits program if ptr is null.
void validate_allocation( void *ptr){
    if( ptr == NULL){
        fprintf( stderr, "out of memory");
        exit( EXIT_FAILURE);
    }
}

int already_sorted( char **strings, size_t num_strings){
    for( size_t i = 1; i < num_strings; i++){
        if( strcmp( strings[ i-1], strings[ i]) > 0){
           //strings[ i-1] was greater.
           return 0;
        }
    }
   return 1;
} 

// used as campare function parameter in qsort.
int my_strcmp( const void *str1, const void *str2){
    return strcmp( * (char **) str1, * (char **) str2);
}






