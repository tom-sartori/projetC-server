struct Node{
    struct Node *previous;
    struct Client client;
    struct Node *next;
};
typedef struct Node Node;