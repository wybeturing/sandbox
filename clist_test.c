/*
 * clist_test.c
 *
 * Automated test code for CLists
 */

#include "clist.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Some known testdata, for testing
const char *testdata[] = {"Zero",     "One",      "Two",      "Three",   "Four",    "Five",
                          "Six",      "Seven",    "Eight",    "Nine",    "Ten",     "Eleven",
                          "Twelve",   "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen",
                          "Eighteen", "Nineteen", "Twenty"};

const char *testdata_sorted[] = {
    "Eight", "Eighteen", "Eleven", "Fifteen", "Five",      "Four", "Fourteen",
    "Nine",  "Nineteen", "One",    "Seven",   "Seventeen", "Six",  "Sixteen",
    "Ten",   "Thirteen", "Three",  "Twelve",  "Twenty",    "Two",  "Zero"};

static const int num_testdata = sizeof(testdata) / sizeof(testdata[0]);

// Checks that value is true; if not, prints a failure message and
// returns 0 from this function
#define test_assert(value)                                               \
    {                                                                    \
        if (!(value)) {                                                  \
            printf("FAIL %s[%d]: %s\n", __FUNCTION__, __LINE__, #value); \
            return 0;                                                    \
        }                                                                \
    }

// Checks that value == INVALID_RETURN; if not, prints a failure
// message and returns 0 from this function
#define test_invalid(value)                                              \
    {                                                                    \
        if (value != INVALID_RETURN) {                                   \
            printf("FAIL %s[%d]: %s\n", __FUNCTION__, __LINE__, #value); \
            return 0;                                                    \
        }                                                                \
    }

// Checks that value == expected; if not, prints a failure message and
// returns 0 from this function
#define test_compare(value, expected)                                                              \
    {                                                                                              \
        const char *v = (value);                                                                   \
        const char *e = (expected);                                                                \
        if (strcmp(v, e) != 0) {                                                                   \
            printf("FAIL %s[%d] %s: expected '%s', got '%s'\n", __FUNCTION__, __LINE__, #value, e, \
                   v);                                                                             \
            return 0;                                                                              \
        }                                                                                          \
    }

// Function to print elements during iteration
void print_callback(int pos, CListElementType element, void *cb_data) {
    printf("Position: %d, Element: %s\n", pos, element);
}

/*
 * Tests the CL_new, CL_push, CL_pop, and CL_free functions
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_cl_push_pop() {
    CList list = CL_new();

    test_assert(CL_length(list) == 0);
    test_invalid(CL_pop(list));

    CL_push(list, testdata[0]);

    test_assert(CL_length(list) == 1);
    test_compare(CL_pop(list), testdata[0]);
    test_assert(CL_length(list) == 0);
    test_invalid(CL_pop(list));

    for (int i = 0; i < num_testdata; i++) {
        CL_push(list, testdata[i]);
        test_assert(CL_length(list) == i + 1);
    }

    for (int i = num_testdata - 1; i >= 0; i--) {
        test_compare(CL_pop(list), testdata[i]);
        test_assert(CL_length(list) == i);
    }

    CL_free(list);

    return 1;
}

int test_cl_append() {
    CList list = CL_new();

    // Append all the items
    for (int i = 0; i < num_testdata; i++) {
        CL_append(list, testdata[i]);
        test_assert(CL_length(list) == i + 1);
    }

    // ensure that the list is in the right order
    for (int i = 0; i < num_testdata; i++) test_compare(CL_nth(list, i), testdata[i]);

    CL_free(list);
    return 1;
}

/*
 * Tests the CL_nth function
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_cl_nth() {
    CList list = CL_new();

    // Empty list -- ensure that CL_nth returns NULL for -2, -1, 0, 1, and 2
    test_invalid(CL_nth(list, -2));
    test_invalid(CL_nth(list, -1));
    test_invalid(CL_nth(list, 0));
    test_invalid(CL_nth(list, 1));
    test_invalid(CL_nth(list, 2));

    // Add one item, then perform same tests
    CL_push(list, testdata[2]);
    test_assert(CL_length(list) == 1);
    test_invalid(CL_nth(list, -3));
    test_invalid(CL_nth(list, -2));
    test_compare(CL_nth(list, -1), testdata[2]);
    test_compare(CL_nth(list, 0), testdata[2]);
    test_invalid(CL_nth(list, 1));
    test_invalid(CL_nth(list, 2));

    // Push two more items, perform similar tests
    CL_push(list, testdata[1]);
    CL_push(list, testdata[0]);
    test_assert(CL_length(list) == 3);
    test_invalid(CL_nth(list, -5));
    test_invalid(CL_nth(list, -4));
    test_compare(CL_nth(list, -3), testdata[0]);
    test_compare(CL_nth(list, -2), testdata[1]);
    test_compare(CL_nth(list, -1), testdata[2]);
    test_compare(CL_nth(list, 0), testdata[0]);
    test_compare(CL_nth(list, 1), testdata[1]);
    test_compare(CL_nth(list, 2), testdata[2]);
    test_invalid(CL_nth(list, 3));
    test_invalid(CL_nth(list, 4));

    CL_free(list);

    return 1;
}

/*
 * Tests the CL_free function
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */

int test_cl_free() {
    CList list = CL_new();

    for (int i = 0; i < 5; ++i) {
        CL_append(list, testdata[i]);
    }

    printf("Before freeing:\n");
    CL_foreach(list, print_callback, NULL);

    CL_free(list);

    return 1;
}

/*
 * Tests the CL_insert function
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */

int test_cl_insert() {
    CList list = CL_new();

    // Empty list -- test insertion into an empty list
    test_assert(!CL_insert(list, testdata[0], 3));
    test_assert(!CL_insert(list, testdata[0], -2));
    test_assert(!CL_insert(list, testdata[1], 5));
    test_assert(CL_length(list) == 0);
    test_assert(CL_insert(list, testdata[0], -1));
    test_assert(CL_length(list) == 1);

    // Non-empty list, inserting at the top
    test_assert(CL_insert(list, testdata[1], -2));
    test_assert(CL_length(list) == 2);
    for (int i = 0; i < 2; i++) test_compare(CL_nth(list, i), testdata[1 - i]);

    // testing for inserting a new item inside a list
    CList second_list = CL_new();
    for (int i = 0; i <= 3; i++) test_assert(CL_insert(second_list, testdata[i], i));
    test_assert(CL_length(second_list) == 4);
    test_assert(CL_insert(second_list, testdata[5], -1));
    test_assert(CL_length(second_list) == 5);
    test_assert(CL_insert(second_list, testdata[4], -2));
    test_assert(CL_length(second_list) == 6);
    for (int i = 0; i < 6; i++) test_compare(CL_nth(second_list, i), testdata[i]);

    CL_free(list);
    CL_free(second_list);

    return 1;
}

/*
 * Tests the CL_remove function
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */

int test_cl_remove() {
    CList list = CL_new();

    // Empty list -- test removal from an empty list
    test_invalid(CL_remove(list, 0));
    test_invalid(CL_remove(list, 6));
    test_invalid(CL_remove(list, -9));

    // Removing from the edges, with zero
    test_assert(CL_insert(list, testdata[0], 0));
    test_assert(CL_length(list) == 1);
    test_compare(CL_remove(list, 0), testdata[0]);
    test_assert(CL_length(list) == 0);

    // Removing from the edges, with the negative number
    for (int i = 0; i <= 3; i++) test_assert(CL_insert(list, testdata[i], i));
    test_assert(CL_length(list) == 4);
    test_compare(CL_remove(list, -4), testdata[0]);
    test_assert(CL_length(list) == 3);

    // Testing invalid indexes on a non-empty list
    test_invalid(CL_remove(list, 13));
    test_invalid(CL_remove(list, -7));
    test_invalid(CL_remove(list, 10));

    // Testing removing from inside, showing that it is removed
    test_assert(CL_insert(list, testdata[0], 0));
    test_assert(CL_length(list) == 4);
    test_compare(CL_remove(list, 2), testdata[2]);
    test_assert(CL_length(list) == 3);

    // Testing removing from inside, showing that order is correct
    test_assert(CL_insert(list, testdata[2], 2));
    test_assert(CL_insert(list, testdata[5], 3));
    test_assert(CL_length(list) == 5);
    test_compare(CL_remove(list, 3), testdata[5]);
    test_assert(CL_length(list) == 4);
    for (int i = 0; i <= 3; i++) test_compare(CL_nth(list, i), testdata[i]);

    CL_free(list);

    return 1;
}

/*
 * Tests the cl_copy function
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */

int test_cl_copy() {
    CList list = CL_new();

    // copy an empty list
    CList copy = CL_copy(list);

    test_assert(CL_length(list) == 0);
    test_assert(CL_length(copy) == 0);

    // test the case for the empty list
    for (int i = 0; i < num_testdata; i++) {
        CL_append(list, testdata[i]);
        test_assert(CL_length(list) == i + 1);
    }

    CL_free(copy);

    copy = CL_copy(list);
    // Assert that the length is correct
    test_assert(CL_length(list) == num_testdata);
    test_assert(CL_length(copy) == num_testdata);

    // Assert that the elements are copied correctly
    for (int i = 0; i < num_testdata; i++) test_compare(CL_nth(copy, i), CL_nth(list, i));

    CL_free(list);
    CL_free(copy);
    return 1;
}

/*
 * Tests the cl_inserted_sorted function
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */

int test_cl_inserted_sorted() {
    CList list = CL_new();

    for (int i = 0; i < num_testdata; i += 2) CL_append(list, testdata_sorted[i]);

    for (int i = 1; i < num_testdata; i += 2) CL_insert_sorted(list, testdata_sorted[i]);

    test_assert(CL_length(list) == num_testdata);

    for (int i = 0; i < num_testdata; i++) test_compare(CL_nth(list, i), testdata_sorted[i]);

    // Attempting the case when the list is empty
    CL_free(list);

    return 1;
}

/*
 * Tests the cl_join
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */

int test_cl_join() {
    CList list = CL_new();
    CList list_join = CL_new();

    // Joining two empty lists should leave us with an empty list
    CL_join(list, list_join);
    test_assert(CL_length(list) == 0);
    test_assert(CL_length(list_join) == 0);

    // Populating both lists and then testing the join

    for (int i = 0; i < 7; ++i) CL_append(list, testdata[i]);

    for (int i = 7; i < 14; ++i) CL_append(list_join, testdata[i]);
    test_assert(CL_length(list) == 7);
    test_assert(CL_length(list_join) == 7);
    CL_join(list, list_join);
    test_assert(CL_length(list) == 14);
    test_assert(CL_length(list_join) == 0);

    // Testing that the values are the same
    for (int i = 0; i < 14; ++i) test_compare(CL_nth(list, i), testdata[i]);

    // Trying to join an empty list to a list should not change anything
    CL_join(list, list_join);
    test_assert(CL_length(list) == 14);
    test_assert(CL_length(list_join) == 0);

    // Testing that the values are the same
    for (int i = 0; i < 14; ++i) test_compare(CL_nth(list, i), testdata[i]);

    CL_free(list);

    CL_free(list_join);
    return 1;
}

/*
 * Tests the cl_reverse
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */

int test_cl_reverse() {
    CList list = CL_new();

    // Reversing an empty list should leave the list empty
    CL_reverse(list);
    test_assert(CL_length(list) == 0);

    // Reversing a list with one item should leave the list unchanged

    CL_append(list, testdata[0]);
    CL_reverse(list);
    test_assert(CL_length(list) == 1);
    test_compare(CL_nth(list, 0), testdata[0]);

    // Reversing a list with items should in fact reverse the list and maintain the length

    for (int i = 1; i < 8; ++i) CL_append(list, testdata[i]);
    CL_reverse(list);
    for (int i = 0; i < 8; ++i) test_compare(CL_nth(list, i), testdata[7 - i]);
    test_assert(CL_length(list) == 8);

    // Reversing a reversed list should ensure the original state
    CL_reverse(list);
    for (int i = 0; i < 8; ++i) test_compare(CL_nth(list, i), testdata[i]);

    CL_free(list);
    return 1;
}

/*
 * Tests the cl_foreach
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */

int test_cl_foreach() {
    CList list = CL_new();

    // CL_foreach on an empty list should do nothing
    CL_foreach(list, print_callback, NULL);

    // Should print for a list with items
    for (int i = 0; i < 5; ++i) {
        CL_append(list, testdata[i]);
    }

    printf("Iterating through the list:\n");
    CL_foreach(list, print_callback, NULL);

    CL_free(list);

    return 1;
}

/*
 * A demonstration of how to use a CList, which also doubles as a
 * test case.
 *
 * Returns: 1 if all tests pass, 0 otherwise.
 */
int sample_clist_usage() {
    CList list = CL_new();

    // new lists have length 0
    test_assert(CL_length(list) == 0);

    CL_push(list, "alpha");    // push 'alpha' onto front of list
    CL_push(list, "bravo");    // push 'bravo' onto front of list
    CL_push(list, "charlie");  // push 'charlie' onto front of list

    // list is now charlie, bravo, alpha

    CL_print(list);  // print out the list: charlie, bravo, alpha

    test_assert(CL_length(list) == 3);

    // pop the element off the front of the list, which should be 'charlie'
    test_compare(CL_pop(list), "charlie");
    test_assert(CL_length(list) == 2);

    CL_insert(list, "delta", 2);     // insert 'delta' at position 2
    CL_append(list, "echo");         // append 'echo' at end of list
    CL_insert(list, "foxtrot", -2);  // insert 'foxtrot' one before end

    // list is now: bravo, alpha, delta, foxtrot, echo
    test_assert(CL_length(list) == 5);

    // retrieve the 3rd element, numbering from 0, so it should be foxtrot
    test_compare(CL_nth(list, 3), "foxtrot");

    // list hasn't changed
    test_assert(CL_length(list) == 5);

    // now REMOVE the third element
    test_compare(CL_remove(list, 3), "foxtrot");
    test_assert(CL_length(list) == 4);

    // list is now: bravo, alpha, delta, echo

    // make a copy of the list
    CList list_copy = CL_copy(list);

    test_assert(CL_length(list_copy) == 4);

    // reverse the copy, so it is now echo, delta, alpha, bravo
    CL_reverse(list_copy);

    // remove the first item from the copy
    test_compare(CL_remove(list_copy, 0), "echo");
    test_assert(CL_length(list_copy) == 3);

    // original list should be unchanged
    test_assert(CL_length(list) == 4);

    // join the two lists; note this operation empties list_copy
    CL_join(list, list_copy);

    // list is now: bravo, alpha, delta, echo, delta, alpha, bravo
    // list_copy should be empty
    test_assert(CL_length(list) == 7);
    test_compare(CL_nth(list, 3), "echo");
    test_assert(CL_length(list_copy) == 0);

    // Lists must be destroyed
    CL_free(list);
    CL_free(list_copy);

    return 1;
}

int main() {
    int passed = 0;
    int num_tests = 0;

    num_tests++;
    passed += test_cl_push_pop();
    num_tests++;
    passed += test_cl_append();
    num_tests++;
    passed += test_cl_nth();
    num_tests++;
    passed += test_cl_insert();
    num_tests++;
    passed += test_cl_remove();
    num_tests++;
    passed += test_cl_copy();
    num_tests++;
    passed += test_cl_inserted_sorted();
    num_tests++;
    passed += test_cl_reverse();
    num_tests++;
    passed += test_cl_foreach();
    num_tests++;
    passed += test_cl_free();

    num_tests++;
    passed += sample_clist_usage();

    printf("Passed %d/%d test cases\n", passed, num_tests);
    fflush(stdout);
    return 0;
}
