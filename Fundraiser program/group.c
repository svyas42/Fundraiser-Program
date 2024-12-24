/**
    @file group.c
    @author Sachi Vyas (smvyas)
    A program that: Helps us reads the item and member file and produce output according to the command lines in the input
    files.
 */
#include "input.h"
#include "group.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
/** Number of items to read from each line from the items file */
#define VAR_ITEMS 3
/** Number of things to read from each line from the members file */
#define VAR_MEMBERS 2
/**
    Dynamically allocates storage for the Group, initializes its fields (to store the two resizable arrays) 
    and returns a pointer to the new Group.
    @return g the allocated group with initialized storage
 */
Group *makeGroup() 
{
    Group *g = (Group*)malloc(sizeof(Group));

    g -> iList = (Item **)malloc(INITIAL_SIZE * sizeof(Item *));
    if (g -> iList == NULL) {
        fprintf(stderr, "Memory allocation issue.\n");
        exit(EXIT_FAILURE);
    }
    g -> iCount = 0;
    g -> iCap = INITIAL_SIZE;

    g -> mList = (Member **)malloc(INITIAL_SIZE * sizeof(Member *));
    if (g -> mList == NULL) {
        fprintf(stderr, "Memory allocation issue.\n");
        exit(EXIT_FAILURE);
    }
    g -> mCount = 0;
    g -> mCap = INITIAL_SIZE;
    return g;
}

/**
    Function frees the memory used to store the given Group, including freeing space for all the Items, 
    Members, and Member SaleItem lists, freeing the resizable arrays of pointers and freeing space for the Group struct 
    itself.
    @param *group the group to free, or empty the allocated memory of
 */
void freeGroup( Group *group ) 
{
    free(group -> iList);
    free(group -> mList);
    free(group);
}

/**
    Function reads all the items from an item file with the given name. It makes an instance 
    of the Item struct for an item in the file and stores a pointer to that Item in the resizable item array in group.
    @param *filename the pointer to a file to read in
    @param *group allows us to access the actual group variable or object that is being pointed at
 */
void readItems( char const *filename, Group *group )
{
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Can't open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    //creating a pointer in the file   
    char *l = NULL;
    
    while ((l = readLine(fp)) != NULL) {
        Item *newItem = malloc(sizeof(Item));
        newItem -> numSold = 0;
        if (newItem == NULL) {
            fprintf(stderr, "Invalid items file: %s\n", filename);
            free(l);
            fclose(fp);
            exit(EXIT_FAILURE);
        }

        //reading in the file
        if (sscanf(l, "%d %d %30[^\n]", &newItem -> itemId, &newItem -> cost, newItem -> nameOfItem) != VAR_ITEMS) {
            fprintf(stderr, "Invalid item file: %s\n", filename);
            free(newItem);
            freeGroup(group);
            fclose(fp);
            exit(EXIT_FAILURE);
        }

        //checking if the id and cost are valid
        if (newItem -> itemId <= 0 || newItem -> cost <= 0) {
            fprintf(stderr, "Invalid item file: %s\n", filename);
            free(newItem);
            //free(l);
            freeGroup(group);
            fclose(fp);
            exit(EXIT_FAILURE);
        }

        //invalid name of item
        if (strlen(newItem -> nameOfItem) > MAX_NAME_LEN) {
            fprintf(stderr, "Invalid item file: %s\n", filename);
            free(newItem);
            freeGroup(group);
            fclose(fp);
            exit(EXIT_FAILURE);
        }

        //check if two or more items have the same id
        for (int i = 0; i < group -> iCount; i++) {
            if (group -> iList[i] -> itemId == newItem -> itemId) {
                fprintf(stderr, "Invalid item file: %s\n", filename);
                // free(newItem);
                // free(l);
                freeGroup(group);
                fclose(fp);
                exit(EXIT_FAILURE);
            }
        }
        //resize the array if needed
        if (group -> iCount >= group -> iCap) {
            group -> iCap *= DOUBLE_SIZE;
            Item **newListItem = realloc(group -> iList, group -> iCap * sizeof(Item *));
            if (newListItem == NULL) {
                fprintf(stderr, "Invalid item file: %s\n", filename);
                // free(newItem);
                // free(l);
                freeGroup(group);
                fclose(fp);
                exit(EXIT_FAILURE);
            }
            group -> iList = newListItem;
        }
        group -> iList[group -> iCount++] = newItem;
        free(l);
    }
    fclose(fp);
}

/**
    Function sorts the items in the given group. It uses the qsort() function together with the function 
    pointer parameter to order the items.
    @param *filename the pointer to a file to read in
    @param *group allows us to access the actual group variable or object that is being pointed at
 */
void readMembers( char const *filename, Group *group ) 
{
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Can't open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    char *l = NULL;
    while((l = readLine(fp)) != NULL) {
        Member *newMember = malloc(sizeof(Member));
        newMember -> soldItemCount = 0;
        if (newMember == NULL) {
            fprintf(stderr, "Invalid member file: %s\n", filename);
            // free(l);
            freeGroup(group);
            fclose(fp);
            exit(EXIT_FAILURE);
        }
        newMember -> soldItems = malloc(INITIAL_SIZE * sizeof(SaleItem));
        if (newMember -> soldItems == NULL) {
            fprintf(stderr, "Invalid member file: %s\n", filename);
            free(newMember);
            // free(l);
            freeGroup(group);
            fclose(fp);
            exit(EXIT_FAILURE);
        }

        newMember -> soldItemCount = 0;
        newMember -> soldItemCap = INITIAL_SIZE;
        if (sscanf(l, "%8s %[^\n]", newMember -> memberId, newMember -> name) != VAR_MEMBERS) {
            fprintf(stderr, "Invalid member file: %s\n", filename);
            // free(newMember);
            // free(l);
            freeGroup(group);
            fclose(fp);
            exit(EXIT_FAILURE);
        }
        
        //invalid inputs
        if (strlen(newMember -> memberId) > MAX_ID_LEN || strlen(newMember -> name) > MAX_NAME_LEN) {
            fprintf(stderr, "Invalid member file: %s\n", filename);
            // free(newMember);
            // free(l);
            freeGroup(group);
            fclose(fp);
            exit(EXIT_FAILURE);
        }

        //check for duplicate ID
        for (int i = 0; i < group -> mCount; i++) {
            if (strcmp(group -> mList[i] -> memberId, newMember -> memberId) == 0) {
                fprintf(stderr, "Invalid member file: %s\n", filename);
                // free(newMember);
                // free(l);
                freeGroup(group);
                exit(EXIT_FAILURE);
                fclose(fp);
               
            }
        }

        //resize array if needed
        if (group -> mCount >= group -> mCap) {
            group->mCap *= DOUBLE_SIZE;
            Member **newListMember = realloc(group -> mList, group -> mCap * sizeof(Member *));
            group->mList = newListMember;
            if (newListMember == NULL) {
                fprintf(stderr, "Invalid member file: %s\n", filename);
                // free(newMember);
                // free(l);
                freeGroup(group);
                fclose(fp);
                exit(EXIT_FAILURE);
            }
            group -> mList = newListMember;
        }
        group -> mList[group -> mCount++] = newMember;
        free(l);

    }
    fclose(fp);
}

/**
    Function sorts the items in the given group. It uses the qsort() function together with 
    the function pointer parameter to order the items.
    @param *group the pointer to a group to sort the items in
    @param *compare is a pointer to a comparison function to help us sort items
 */
void sortItems( Group *group, int (* compare) (void const *va, void const *vb )) 
{
    if (group -> iCount > 0) {
        qsort(group -> iList, group -> iCount, sizeof(Item *), compare);
    }
}

/**
    This function sorts the members in the given group. It uses the qsort() function together with the 
    function pointer parameter to order the members.
    @param *group the pointer to a group to sort the members in
    @param *compare is a pointer to a comparison function to help us sort members
 */
void sortMembers( Group *group, int (* compare) (void const *va, void const *vb )) 
{
    if (group -> mCount > 0) {
        qsort(group -> mList, group -> mCount, sizeof(Member *), compare);
    }
}

/**
    This function prints all or some of the items.
    @param *group the pointer to a group to list the items from
    @param *test is pointer to test function that takes a const *item and char const *str and checks if the *item meets the criteria
    @param *str is pointer to a string that we are trying to look for in the item
 */
void listItems( Group *group, bool (*test)( Item const *item, char const *str ), char const *str ) 
{
    int totalItemsSold = 0;
    int numSold = 0;
    int total = 0;
    int totalTable = 0;
    for (int i = 0; i < group -> iCount; i++) {
        Item *item = group -> iList[i];
        if (test != NULL && !test(item, str)) {
            continue;
        }
        numSold = item -> numSold;
        total = item -> cost * numSold;
        printf("%3d %-30s %6d %6d %6d\n", item -> itemId, item -> nameOfItem, item -> cost, numSold, total);
        totalItemsSold += numSold;
        totalTable += total;

    }
    printf("%3s %-30s %6s %4d %6d\n\n", "TOTAL", "", "", totalItemsSold, totalTable);
}

/**
    This function prints all or some of the members.
    @param *group the pointer to a group to list the members from
    @param *test is pointer to test function that takes a const *member and char const *str and checks if the *item meets the criteria
    @param *str is pointer to a string that we are trying to look for in the *member
 */
void listMembers( Group *group, bool (*test)( Member const *member, char const *str ), char const *str ) 
{
    
    int totalItemsSold = 0;
    int totalCost = 0;
    for (int i = 0; i < group -> mCount; i++) {
        Member *m = group -> mList[i];
        if (test != NULL && !test(m, str)) {
            continue;
        }
        int soldItems = 0;
        int totalMemberCost = 0;

        for (int j = 0; j < m -> soldItemCount; j++) {
            SaleItem *s = m -> soldItems[j];
            soldItems += s -> quantity;
            totalMemberCost += s -> quantity * s -> item -> cost;
        }
        printf("%-8s %-30s %6d %6d\n", m -> memberId, m -> name, soldItems, totalMemberCost);

        totalItemsSold += soldItems;
        totalCost += totalMemberCost;

    }
    printf("%-8s %-30s %6d %6d\n\n", "TOTAL", "", totalItemsSold, totalCost);

}
