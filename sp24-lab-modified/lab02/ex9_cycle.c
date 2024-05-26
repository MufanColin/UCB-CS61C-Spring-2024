#include <stddef.h>
#include "ex9_cycle.h"

int ll_has_cycle(node *head) {
    /* TODO: Implement ll_has_cycle */
    node *fast_ptr = head, *slow_ptr = head;
    while (fast_ptr) {
        fast_ptr = fast_ptr->next;
        if (fast_ptr) {
            fast_ptr = fast_ptr->next;
        } else {
            break;
        }
        slow_ptr = slow_ptr->next;
        if (slow_ptr == fast_ptr) {
            return 1;
        }
    }
    return 0;
}
