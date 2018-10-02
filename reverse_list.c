#include <stdio.h>

struct List {
    struct Node *head;
};

struct Node {
    int element;
    struct Node *next;
};

void add_to_head_list(struct List *list, struct Node *node)
{
    node->next = list->head;
    list->head = node;
}

void reverse_list(struct List *list, struct List *rlist)
{
    struct Node *node = list->head;
    struct Node *next_node;

    /* traverse list */
    while (node) {
        next_node = node->next;
        add_to_head_list(rlist, node);
        node = next_node;
    }
}

void print_list(struct List *list)
{
    struct Node *node = list->head;

    while(node) {
        printf("%d\n", node->element);
        node = node->next;
    }
}

int main()
{
    struct Node node3 = {5, NULL};
    struct Node node2 = {3, &node3};
    struct Node node1 = {9, &node2};
    struct Node node0 = {1, &node1};

    struct List list = {&node0};
    struct List rlist = {NULL};

    print_list(&list);
    reverse_list(&list, &rlist);
    printf("---------------------------\n");
    print_list(&rlist);

    return 0;
}
