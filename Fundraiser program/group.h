/**
    @file group.c
    @author Sachi Vyas (smvyas)
    A program that: The prototype for group.c
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/** Initial size for the member and sale item representation */
#define INITIAL_SIZE 5
/** Maximum length for the name of an item or member */
#define MAX_NAME_LEN 30
/** Maximum length for an ID */
#define MAX_ID_LEN 8
/** Multiply by 2 to increase array size during resizing */
#define DOUBLE_SIZE 2

/** Struct for the items */
struct ItemStruct {
    int itemId;
    char nameOfItem[MAX_NAME_LEN + 1];
    int cost;
    int numSold;
};
typedef struct ItemStruct Item;

/** Struct for the helping us deal with how many of each item are sold */
struct SaleItemStruct {
    Item *item;  
    int quantity;
};
typedef struct SaleItemStruct SaleItem;

/** Struct for the members */
struct MemberStruct {
    char memberId[MAX_ID_LEN + 1];
    char name[MAX_NAME_LEN + 1];
    SaleItem **soldItems;
    int soldItemCount; 
    int soldItemCap;     
};
typedef struct MemberStruct Member;

/** Struct for the group */
struct GroupStruct {
    int iCount;
    Item **iList;
    int iCap;
    int mCount;
    Member **mList;
    int mCap;
};
typedef struct GroupStruct Group;
/**
    Dynamically allocates storage for the Group, initializes its fields (to store the two resizable arrays) 
    and returns a pointer to the new Group.
    @return g the allocated group with initialized storage
 */
Group *makeGroup();
/**
    Function frees the memory used to store the given Group, including freeing space for all the Items, 
    Members, and Member SaleItem lists, freeing the resizable arrays of pointers and freeing space for the Group struct 
    itself.
    @param *group the group to free, or empty the allocated memory of
 */
void freeGroup( Group *group );
/**
    Function reads all the items from an item file with the given name. It makes an instance 
    of the Item struct for an item in the file and stores a pointer to that Item in the resizable item array in group.
    @param *filename the pointer to a file to read in
    @param *group allows us to access the actual group variable or object that is being pointed at
 */
void readItems( char const *filename, Group *group );
/**
    Function sorts the items in the given group. It uses the qsort() function together with the function 
    pointer parameter to order the items.
    @param *filename the pointer to a file to read in
    @param *group allows us to access the actual group variable or object that is being pointed at
 */
void readMembers( char const *filename, Group *group );
/**
    Function sorts the items in the given group. It uses the qsort() function together with 
    the function pointer parameter to order the items.
    @param *group the pointer to a group to sort the items in
    @param *compare is a pointer to a comparison function to help us sort items
 */
void sortItems( Group *group, int (* compare) (void const *va, void const *vb ));
/**
    This function sorts the members in the given group. It uses the qsort() function together with the 
    function pointer parameter to order the members.
    @param *group the pointer to a group to sort the members in
    @param *compare is a pointer to a comparison function to help us sort members
 */
void sortMembers( Group *group, int (* compare) (void const *va, void const *vb ));
/**
    This function prints all or some of the items.
    @param *group the pointer to a group to list the items from
    @param *test is pointer to test function that takes a const *item and char const *str and checks if the *item meets the criteria
    @param *str is pointer to a string that we are trying to look for in the item
 */
void listItems( Group *group, bool (*test)( Item const *item, char const *str ), char const *str );
/**
    This function prints all or some of the members.
    @param *group the pointer to a group to list the members from
    @param *test is pointer to test function that takes a const *member and char const *str and checks if the *item meets the criteria
    @param *str is pointer to a string that we are trying to look for in the *member
 */
void listMembers( Group *group, bool (*test)( Member const *member, char const *str ), char const *str );
