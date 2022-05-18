
Node *createNode () {
    Node *node = (Node *)malloc(sizeof(Node));
    node->previous = NULL;
    node->next = NULL;

    return node;
}

int hasNext (Node *node) {
    return node->next != NULL;
}