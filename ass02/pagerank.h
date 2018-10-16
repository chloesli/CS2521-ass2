typedef struct List *Li;

typedef struct ListNode {
    char *value;
    struct ListNode *next;
    struct ListNode *prev;
} ListNode;

typedef struct List{
    int nvalues;
    struct ListNode *first;
    struct ListNode *last;
} List;


// function declarations 
static ListNode *MakeNode(char *val);
Li ListofUrls();
