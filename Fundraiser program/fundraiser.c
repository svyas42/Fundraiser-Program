/**
    @file fundraiser.c
    @author Sachi Vyas (smvyas)
    A program that: Helps us process the elements in the command line and print
    the output accordingly.
 */
#include "input.h"
#include "group.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/** Minimum number of commands on the command line */
#define MIN_ARGS 3
/** Length of the word */
#define LENGTH 4
/**
    Checks if a string is contained in the item
    @param *item a pointer to an item that we are currently looking at
    @param *str a pointer to a str to compare
    @return true if *str is equal to the name else returns false
 */
bool testItemNameEquals(Item const *item, char const *str) {
    if (str == NULL || *str == '\0') {
        return true;
    }
    return strstr(item -> nameOfItem, str) != NULL;
}
/**
    Checks if a string is contained in the member pointer
    @param *member a pointer to a member that we are currently looking at
    @param *str a pointer to a str to compare
    @return true if *str is equal to the name else returns false
 */
bool testMemberNameEquals(Member const *member, char const *str) {
    if (str == NULL || *str == '\0') {
        return true;
    }
    return strstr(member -> name, str) != NULL;
}
/**
    Compares two member id's
    @param *va a pointer to a member id that we are currently looking at
    @param *vb a pointer to a compare *va with
    @return int returns 0 if *va and *vb are equal each other else returns 1
 */
int compareMemberID(void const *va, void const *vb) {
    Member *const *m1 = va;
    Member *const *m2 = vb;
    return strcmp((*m1) -> memberId, (*m2) -> memberId);
}
/**
    Compares two item id's
    @param *va a pointer to an item id that we are currently looking at
    @param *vb a pointer to a compare *va with
    @return int returns 0 if *va and *vb are equal each other else returns 1
 */
int compareItemsID(void const *va, void const *vb) {
    Item *const *i1 = va;
    Item *const *i2 = vb;
    if ((*i1) -> itemId < (*i2) -> itemId) {
        return -1;
    }
    if ((*i1) -> itemId > (*i2) -> itemId) {
        return 1;
    }
    return 0;
}
/**
    Checks if two item names are equal
    @param *va a pointer to an item name that we are currently looking at
    @param *vb a pointer to an item name to compare with
    @return 0 if *va is equal to *vb else returns false
 */
int compareItemsByName(const void *va, const void *vb) {
    const Item *i1 = *(const Item **)va; 
    const Item *i2 = *(const Item **)vb; 
    int compareNames = strcmp(i1 -> nameOfItem, i2 -> nameOfItem);
    if (compareNames != 0) {
        return compareNames;
    }
    if (i1 -> itemId < i2 -> itemId) {
        return -1;
    }
    if (i1 -> itemId > i2 -> itemId) {
        return 1;
    }
    return 0;
}
/**
    Checks if two member names are equal
    @param *va a pointer to a member name that we are currently looking at
    @param *vb a pointer to a member name to compare with
    @return 0 if *va is equal to *vb else returns false
 */
int compareMembersByName(const void *va, const void *vb) {
    const Member *m1 = *(const Member **)va; 
    const Member *m2 = *(const Member **)vb;

    // First, compare by name
    int compareNames = strcmp(m1 -> name, m2 -> name);
    if (compareNames != 0) {
        return compareNames;
    }
    return strcmp(m1 -> memberId, m2 -> memberId);
}
/**
    Helps us compare the total cost of items sold by a member
    @param *group a pointer to the group that we are currently looking at
    @param totalCostArray an array containing the total cost of each item sold by the member
    @param totalItemsSoldByMember an array containing the number of each item sold by the member
 */
void compareTotalCost(Group *group, int totalCostArray[], int totalItemsSoldByMember[]) {
    for (int i = 0; i < group -> mCount - 1; i++) {
        for (int k = 0; k < group -> mCount - 1 - i; k++) {
            if (totalCostArray[k] < totalCostArray[k + 1]) {
                //getting the current member at which the pointer is pointing to
                Member *currentMember = group -> mList[k];
                //assigning the currentMember to the next idx since its cost sold is more
                group -> mList[k] = group -> mList[k + 1];
                group -> mList[k + 1] = currentMember;

                //now swapping the total items sold by member
                int currentItemsSoldByMember = totalItemsSoldByMember[k];
                totalItemsSoldByMember[k] = totalItemsSoldByMember[k + 1];
                totalItemsSoldByMember[k + 1] = currentItemsSoldByMember;

                //swapping members with the highest cost of items
                int currentCost = totalCostArray[k];
                totalCostArray[k] = totalCostArray[k + 1];
                totalCostArray[k + 1] = currentCost;
            }
        }
    }
}
/**
    Returns an integer based on if the program successfully executed
    @param argc the number of arguments in the command line
    @param argv the array to put the arguments in
    @return 1 or 0 based on if the program ran successfully or not
 */
int main(int argc, char *argv[]) 
{
    if (argc < MIN_ARGS) {
        fprintf(stderr, "usage: fundraiser item-file member-file\n");
        exit(EXIT_FAILURE);
    }
    Group *group = makeGroup();
    readItems(argv[1], group);
    sortItems(group, compareItemsID);
    readMembers(argv[DOUBLE_SIZE], group);
    sortMembers(group, compareMemberID);
    
    FILE *outfile = stdout;

    char memberId[MAX_ID_LEN + 1];
    int itemId = 0, numItemsSold = 0;
    char *cmd;
    bool quitFound = false;

    while ((cmd = readLine(stdin))) {
        
        if (strncmp(cmd, "quit", LENGTH) == 0) {
            fprintf(outfile, "cmd> quit\n");
            quitFound = true;
            exit(EXIT_SUCCESS);
        }
        else if (strncmp(cmd, "sale", LENGTH) == 0) {
            if (sscanf(cmd, "sale %8s %d %d", memberId, &itemId, &numItemsSold) == MIN_ARGS) {

                fprintf(outfile, "cmd> sale %s %d %d\n", memberId, itemId, numItemsSold);

                int state = 0;
                bool itempresent = false;
                // Find member and item
                for (int i = 0; i < group -> mCount; i++) {
                    if (strcmp(group -> mList[i] -> memberId, memberId) == 0) {
                      
                        Member *m = group->mList[i];
                        state = LENGTH;
                        for (int j = 0; j < group->iCount; j++) {
                            if (group -> iList[j] -> itemId == itemId) {
                                Item *item = group -> iList[j];
                                item -> numSold += numItemsSold;

                                itempresent = false;
                                // Update sold items for the member
                                for (int k = 0; k < m -> soldItemCount; k++) {
                                    SaleItem *sale = m -> soldItems[k];
                                    if (sale -> item == item) {
                                        sale -> quantity += numItemsSold;
                                        itempresent = true;
                                        break;
                                    }
                                }
                                if (!itempresent) {
                                    if (m -> soldItemCount == m -> soldItemCap) {
                                        m -> soldItemCap *= DOUBLE_SIZE;
                                        m -> soldItems = realloc(m -> soldItems, m -> soldItemCap * sizeof(SaleItem *));
                                        if (!m -> soldItems) {
                                            exit(EXIT_FAILURE);
                                        }
                                    }
                                    // Create a new SaleItem
                                    SaleItem *newSale = malloc(sizeof(SaleItem *));
                                    newSale -> item = item;
                                    newSale -> quantity = numItemsSold;

                                    // Add the new SaleItem to the member's soldItems array
                                    m -> soldItems[m -> soldItemCount++] = newSale;
                                }
                                break;
                            }
                        }
                      
                        break;
                    }
                }
                if (state == 0) {
                    fprintf(outfile, "Invalid command\n\n");
                }
                else {
                    printf("%c", '\n');
                }
                
            }
            else {
                fprintf(outfile, "cmd> sale %s %d %d\n", memberId, itemId, numItemsSold);
                fprintf(outfile, "Invalid command\n");
            }
            
        }        
        else if (strcmp(cmd, "list items") == 0) {
            fprintf(outfile, "cmd> list items\n");
            fprintf(outfile, "%-3s %-30s %6s %6s %6s\n", "ID", "Name", "Cost", "Sold", "Total");
            qsort(group -> iList, group -> iCount, sizeof(Item *), compareItemsID);
            listItems(group, testItemNameEquals, NULL);
        }
        else if (strcmp(cmd, "list item names") == 0) {
            fprintf(outfile, "cmd> list item names\n");
            fprintf(outfile, "%-3s %-30s %6s %6s %6s\n", "ID", "Name", "Cost", "Sold", "Total");
            qsort(group -> iList, group -> iCount, sizeof(Item *), compareItemsByName);
            listItems(group, testItemNameEquals, NULL);
            
        }
        else if (strcmp(cmd, "list members") == 0) {
            fprintf(outfile, "cmd> list members\n");
            fprintf(outfile, "%-8s %-30s %6s %6s\n", "ID", "Name", "Sold", "Total");
            qsort(group -> mList, group -> mCount, sizeof(Member *), compareMemberID);
            listMembers(group, testMemberNameEquals, NULL);

        }
        else if (strcmp(cmd, "list member names") == 0) {
            fprintf(outfile, "cmd> list member names\n");
            fprintf(outfile, "%-8s %-30s %6s %6s\n", "ID", "Name", "Sold", "Total");
            qsort(group -> mList, group -> mCount, sizeof(Member *), compareMembersByName);
            listMembers(group, testMemberNameEquals, NULL);
            
        }
        else if (strcmp(cmd, "list topsellers") == 0) {
            fprintf(outfile, "cmd> list topsellers\n");
            fprintf(outfile, "%-8s %-30s %6s %6s\n", "ID", "Name", "Sold", "Total");
            int totalItemsSold[group -> mCount];
            int totalCost[group -> mCount];
            
            for (int i = 0; i < group -> mCount; i++) {
                Member *m = group -> mList[i];
                totalItemsSold[i] = 0;
                totalCost[i] = 0;

                // Calculate total items sold and total cost for this member
                for (int j = 0; j < m -> soldItemCount; j++) {
                    SaleItem *sale = m -> soldItems[j];
                    totalItemsSold[i] += sale -> quantity;
                    totalCost[i] += sale -> quantity * sale -> item -> cost;
                }
            }

            compareTotalCost(group, totalCost, totalItemsSold);
            listMembers(group, testMemberNameEquals, NULL);
        }
        
        else if (strstr(cmd, "list member") != NULL) {
            
            char memberId[MAX_ID_LEN + 1];
            if (sscanf(cmd, "list member %s", memberId) != 1) {
                fprintf(outfile, "Invalid command\n");
                continue;
            }

            int state = 0;
            //Finding the member with the given Id
            Member *m = NULL;
            for (int i = 0; i < group -> mCount; i++) {
                if (strcmp(group -> mList[i] -> memberId, memberId) == 0) {
                    m = group -> mList[i];
                    state = 1;
                    break;
                }
            }
            if (state == 0) {
                fprintf(outfile, "cmd> %s\n", cmd);
                fprintf(outfile, "Invalid command\n\n");
            }
            else if (state == 1) {
                int totalSold = 0, totalCost = 0;
                fprintf(outfile, "cmd> list member %s\n", memberId);
                fprintf(outfile, "%-3s %-30s %6s %6s %6s\n", "ID", "Name", "Cost", "Sold", "Total");

                int itemCount = 0;
                SaleItem *currentSales[MAX_NAME_LEN + 1];

                // Get the sold items for this member
                for (int i = 0; i < m -> soldItemCount; i++) {
                    SaleItem *sale = m -> soldItems[i];
                    bool found = false;
                    for (int j = 0; j < itemCount; j++) {
                        if (currentSales[j] -> item -> itemId == sale -> item -> itemId) {
                            currentSales[j] -> quantity += sale -> quantity;
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        currentSales[itemCount] = sale;
                        itemCount++;
                    }
                }
                // Sort by item ID
                for (int i = 0; i < itemCount - 1; i++) {
                    for (int j = 0; j < itemCount - 1 - i; j++) {
                        if (currentSales[j] -> item -> itemId > currentSales[j + 1] -> item -> itemId) {
                            SaleItem *current = currentSales[j];
                            currentSales[j] = currentSales[j + 1];
                            currentSales[j + 1] = current;
                        }
                    }
                }
                for (int i = 0; i < itemCount; i++) {         
                    Item *item = currentSales[i] -> item;
                    int sold = currentSales[i] -> quantity;
                    int cost = sold * item -> cost;
                    totalSold += sold;
                    totalCost += cost;
                    fprintf(outfile, "%-3d %-30s %6d %6d %6d\n", item -> itemId, item -> nameOfItem, item -> cost, sold, cost);
                }

                // Print the totals
                fprintf(outfile, "%3s %-30s %6s %4d %6d\n\n", "TOTAL", "", "", totalSold, totalCost);
                
            }
        }

        else if (strstr(cmd, "search item") != NULL) {
            char searchStr[MAX_ID_LEN + 1];
            if (sscanf(cmd, "search item %30s", searchStr) == 1) {
                fprintf(outfile, "cmd> %s\n", cmd);
                fprintf(outfile, "%-3s %-30s %6s %6s %6s\n", "ID", "Name", "Cost", "Sold", "Total");
                listItems(group, testItemNameEquals, searchStr);
            } 
            else {
                fprintf(outfile, "Invalid command\n");
            }
        }
        else if (strstr(cmd, "search member") != NULL) {
            char searchStr[MAX_NAME_LEN + 1]; 
            if (sscanf(cmd, "search member %15s", searchStr) == 1) {
                fprintf(outfile, "cmd> search member %s\n", searchStr);
                fprintf(outfile, "%-8s %-30s %6s %6s\n", "ID", "Name", "Sold", "Total");
                listMembers(group, testMemberNameEquals, searchStr);
            } 
            else {
                fprintf(outfile, "Invalid command\n");
            }
        }
        else {
            fprintf(outfile, "cmd> %s\n", cmd);
            fprintf(outfile, "Invalid command\n\n");
        }
    }
    if (quitFound == false) {
        fprintf(outfile, "cmd> ");
        exit(EXIT_SUCCESS);
        
    }
    fclose(outfile);
    return EXIT_SUCCESS;
}
