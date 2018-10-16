// Associative array size
#include <stdio.h>
#include <stdlib.h>

struct list_node {
    struct list_node *next;
    struct list_node *prev;
};

struct list_node *head;

struct cache_node {
    struct list_node list;
    int val;
    int index;
};

typedef struct {
    int size;
    int num;
    struct cache_node *cache;
} LRUCache;

LRUCache* lRUCacheCreate(int capacity) {
    int i;
    LRUCache *lc = malloc(sizeof(LRUCache));
    if (!lc)
        return NULL;
    lc->size = capacity;
    lc->num = 0;
    lc->cache = calloc(100, sizeof(struct cache_node));
    for (i = 0; i < 100; i++)
    {
        lc->cache[i].index =i;
    }
    
    return lc;
}
void print_list()
{
    struct list_node *p;
//    printf("print list\n");
    if (!head){
        printf("Empty\n");
        return;
    }
    if (head == head->next)
        printf("head %d\n", ((struct cache_node *)head)->val);
    else {
        p = head;
        while(1)
        {
            printf("p %d", ((struct cache_node *)p)->val);
            p = p->next;
            if (p == head)
                break;
        }
        printf("\n");
    }
}
void delete_node(struct list_node *node)
{
    struct list_node *prev, *next;
   // print_list();
//    printf("only node: %p %p,%p\n", node, node->next, head);
    if (node->next == node)
    {
     //   printf("only 1 node: %p %p,%p\n", node, node->next, head);
     //   print_list();
        head = NULL;
    } else {
        //print_list();
        if (head == node)
            head = node->next;
        node->next->prev = node->prev;
        prev = node->prev;
        next = node->next;
       // if (!prev)
          //  printf("prev is NULL\n");
        prev->next = next;
    }
    
    node->next = NULL;
    node->prev = NULL;
}

void delete_tail(void)
{
    struct list_node *tail;
     if (!head)
         return;
    
    tail = head->prev;
    delete_node(tail);
    ((struct cache_node *)tail)->val = 0;
}
void insert_head(struct list_node *new)
{
    struct list_node *n = head;
    if (!n) {
        new->next = new;
        new->prev = new;
    } else {

        n->prev->next = new;
        new->prev = n->prev;
        n->prev = new;
        new->next = n;
    }
    head = new;
}

void move_to_head(struct list_node *node)
{
    if (head == node)
        return;
    //printf("node %d", ((struct cache_node *)node)->val);
  // printf("only delete node: %p %p, %p, %p\n", node, node->next, node->prev, head);
    delete_node(node);
    insert_head(node);
}

int lRUCacheGet(LRUCache* obj, int key) 
{
    int val;
    
    if (key < 0 || key >= 100)
        return -1;
    val = obj->cache[key].val;
    if (val) {
        move_to_head(&obj->cache[key].list);
        return val;
    }
    
    return -1;
}

void lRUCachePut(LRUCache* obj, int key, int value) {
  //  printf("LRU put %d %d %d\n", key, value, obj->cache[key].val);
    if (obj->cache[key].val) {
        obj->cache[key].val = value;
 //       printf("LRU put existed value %d %d\n", key, value);
        move_to_head(&obj->cache[key].list);
    } else {
    //    printf("num %d, %d\n", obj->num, obj->size);
        if (obj->num == obj->size){
            delete_tail();
        } else {
            obj->num++;
        }
            obj->cache[key].val = value;
            insert_head(&obj->cache[key].list);
    }
}

void lRUCacheFree(LRUCache* obj) {
    free(obj->cache);
    free(obj);
}

int main(void){
	struct LRUCache* obj = lRUCacheCreate(1);
	int rv;

	lRUCachePut(obj, 2, 1);
	rv = lRUCacheGet(obj, 1);
	printf("%d\n", rv);

	lRUCacheFree(obj);

	return 0;
}
