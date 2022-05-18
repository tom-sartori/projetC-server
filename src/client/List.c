#include "Node.c"

List *createList() {
    List *list = (List*)malloc(sizeof(List));
    list->head = createNode();
    return list;
}

int isEmpty(List *list) {
    return list->head->next == NULL ? 1 : 0;
}

void add(List *list, Client client) {
    // Lock mutex.
    pthread_mutex_lock(&mutex);

    Node *node = createNode();
    node->client = client;

    node->previous = list->head;
    node->next = list->head->next;

    if (isEmpty(list)) {
        list->head->next = node;
    }
    else {
        list->head->next->previous = node;
        list->head->next = node;
    }
    // Unlock mutex.
    pthread_mutex_unlock(&mutex);
}

Node *next(Node *node){
    return node->next;
}

Node *previous (Node *node) {
    return node->previous;
}

void delete (List *list, Client *client) {
    if (isEmpty(list)) {
        exit(EXIT_FAILURE);
    }

    // Lock mutex.
    pthread_mutex_lock(&mutex);

    int isDeleted = 0;

    Node *current = next(list->head);
    while (!isDeleted && current != NULL) {
        if (strcmp(current->client.username, client->username) == 0) {
            current->previous->next = current->next;
            if (hasNext(current)) {
                current->next->previous = current->previous;
            }

            free(current);
            isDeleted = 1;
        }
        else {
            current = next(current);
        }
    }
    // Unlock mutex.
    pthread_mutex_unlock(&mutex);
}

/**
 * Check if a client's username is contained in the list.
 *
 * @param list : Linked list of clients.
 * @param username
 * @return 1 if the a user has the username in params or NULL.
 */
Client *contains (List *list, char *username) {
    if (isEmpty(list)) {
        return 0;
    }

    // Lock mutex.
    pthread_mutex_lock(&mutex);

    Node *current = next(list->head);
    while (current != NULL) {
            if (strcmp(username, current->client.username) == 0) {
                // Unlock mutex.
                pthread_mutex_unlock(&mutex);
                return &current->client;
            }
            current = next(current);
    }
    // Unlock mutex.
    pthread_mutex_unlock(&mutex);
    return NULL;
}
