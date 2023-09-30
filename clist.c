/*
 * clist.c
 *
 * Linked list implementation for ISSE Assignment 5
 *
 * Author: <YOUR NAME HERE!>
 */

#include "clist.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG

struct _cl_node {
        CListElementType element;
        struct _cl_node *next;
};

struct _clist {
        struct _cl_node *head;
        int length;
};

/*
 * Create (malloc) a new _cl_node and populate it with the supplied
 * values
 *
 * Parameters:
 *   element, next  the values for the node to be created
 *
 * Returns: The newly-malloc'd node, or NULL in case of error
 */
static struct _cl_node *_CL_new_node(CListElementType element, struct _cl_node *next) {
        struct _cl_node *new = (struct _cl_node *)malloc(sizeof(struct _cl_node));

        assert(new);

        new->element = element;
        new->next = next;

        return new;
}

// Documented in .h file
CList CL_new() {
        CList list = (CList)malloc(sizeof(struct _clist));
        assert(list);

        list->head = NULL;
        list->length = 0;

        return list;
}

// Documented in .h file
void CL_free(CList list) {
        // free the members of the list
        struct _cl_node *iter = list->head;
        while (iter) {
                struct _cl_node *temp = iter;
                iter = iter->next;
                free(temp);
        }
        // free the list itself
        free(list);
}

// Documented in .h file
int CL_length(CList list) {
        assert(list);
#ifdef DEBUG
        // In production code, we simply return the stored value for
        // length. However, as a defensive programming method to prevent
        // bugs in our code, in DEBUG mode we walk the list and ensure the
        // number of elements on the list is equal to the stored length.

        int len = 0;
        for (struct _cl_node *node = list->head; node != NULL; node = node->next) len++;

        assert(len == list->length);
#endif  // DEBUG

        return list->length;
}

// Documented in .h file
void CL_print(CList list) {
        assert(list);
        int num = 0;
        for (struct _cl_node *node = list->head; node != NULL; node = node->next)
                printf("  [%d]: %s\n", num++, node->element);
}

// Documented in .h file
void CL_push(CList list, CListElementType element) {
        assert(list);
        list->head = _CL_new_node(element, list->head);
        list->length++;
}

// Documented in .h file
CListElementType CL_pop(CList list) {
        assert(list);
        struct _cl_node *popped_node = list->head;

        if (popped_node == NULL) return INVALID_RETURN;

        CListElementType ret = popped_node->element;

        // unlink previous head node, then free it
        list->head = popped_node->next;
        free(popped_node);
        // we cannot refer to popped node any longer

        list->length--;

        return ret;
}

// Documented in .h file
void CL_append(CList list, CListElementType element) {
        assert(list);
        struct _cl_node *new_node = _CL_new_node(element, NULL);
        assert(new_node);

        // If the list is empty we just update the head, else we find the end and then update the
        // last pointer
        if (list->head == NULL) {
                list->head = new_node;
        } else {
                struct _cl_node *iter = list->head;
                while (iter->next) {
                        iter = iter->next;
                }
                iter->next = new_node;
        }
        list->length++;
}

// Documented in .h file
CListElementType CL_nth(CList list, int pos) {
        assert(list);
        // Handling the case of an empty list
        if (list->head == NULL) {
                return INVALID_RETURN;
        }
        const int len = CL_length(list);
        if (pos >= -len && pos <= len - 1) {
                struct _cl_node *iter = list->head;
                const int standard_pos = (pos < 0) ? pos + len : pos;
                int current_position = 0;
                while (current_position < standard_pos) {
            if (iter == NULL) return INVALID_RETURN;
                        iter = iter->next;
                        current_position++;
                }
                return iter->element;
        }
        return INVALID_RETURN;
}

// Documented in .h file
bool CL_insert(CList list, CListElementType element, int pos) {
        assert(list);
        // Handle the case where the list is empty
        if (list->head == NULL && pos != 0 && pos != -1) {
                return false;
        }

        const int len = CL_length(list);

        if (pos < -(len + 1) || pos > len) {
                return false;
        } else {
                const int standard_pos = (pos < 0) ? pos + len + 1 : pos;
                struct _cl_node *new_node = _CL_new_node(element, NULL);
                assert(new_node);
                if (standard_pos == 0) {
                        new_node->next = list->head;
                        list->head = new_node;
                } else {
                        struct _cl_node *iter = list->head;
                        int current_position = 0;
                        while (current_position < standard_pos - 1) {
                                iter = iter->next;
                                current_position++;
                        }
                        new_node->next = iter->next;
                        iter->next = new_node;
                }
                list->length++;
        }
        return true;
}

// Documented in .h file
CListElementType CL_remove(CList list, int pos) {
        assert(list);

        // Handle the case where the list is empty
        if (list->head == NULL) {
                return INVALID_RETURN;
        }

        const int len = CL_length(list);

        if (pos < -len || pos > len - 1) {
                return INVALID_RETURN;
        } else {
                const int standard_pos = (pos < 0) ? pos + len : pos;
                CListElementType to_return;

                // Handle the case when we are removing the head item
                if (standard_pos == 0) {
                        struct _cl_node *temp = list->head;
                        list->head = list->head->next;
                        to_return = temp->element;
                        free(temp);
                } else {
                        struct _cl_node *iter = list->head;
                        int current_position = 0;
                        while (current_position < standard_pos - 1) {
                                iter = iter->next;
                                current_position++;
                        }
                        struct _cl_node *temp = iter->next;
                        iter->next = temp->next;
                        to_return = temp->element;
                        free(temp);
                }

                list->length--;
                return to_return;
        }

        return INVALID_RETURN;
}

// Documented in .h file
CList CL_copy(CList list) {
        assert(list);

        CList list_copy = CL_new();

        struct _cl_node *iter = list->head;

        while (iter) {
                CL_append(list_copy, iter->element);
                iter = iter->next;
        }

        return list_copy;
}

// Documented in .h file
int CL_insert_sorted(CList list, CListElementType element) {
        assert(list);

        /* Handle the case where the list is empty */
        if (list->length == 0) {
                CL_append(list, element);
                return 0;
        } else { /* If the list is not empty */
                int index = 0;
                struct _cl_node *iter = list->head;

                while (iter->next != NULL && (strcmp(iter->element, element) < 0)) {
                        iter = iter->next;
                        iter++;
                }

                CL_insert(list, element, index);
                return index;
        }
}

// Documented in .h file
void CL_join(CList list1, CList list2) {
        assert(list1);
        assert(list2);

        while (list2->length) {
                CL_append(list1, CL_pop(list2));
        }
}

// Documented in .h file
void CL_reverse(CList list) {
        assert(list);

        // We use two pointers that sweep across
        struct _cl_node *current = list->head;
        struct _cl_node *prev = NULL;
        struct _cl_node *next = NULL;

        while (current != NULL) {
                // Remember the next node
                next = current->next;
                // Reverse the current direction
                current->next = prev;
                // Progress in the list
                prev = current;
                current = next;
        }
        list->head = prev;
}

// Documented in .h file
void CL_foreach(CList list, CL_foreach_callback callback, void *cb_data) {
        assert(list);

        int pos = 0;
        struct _cl_node *iter = list->head;

        while (iter != NULL) {
                callback(pos, iter->element, cb_data);
                iter = iter->next;
                pos++;
        }
}
