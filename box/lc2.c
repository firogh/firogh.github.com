/*
 * lc2
 */
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 *  
 */
/*
 *
 * Failed due to overflow
 */
struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
    int  weight = 10, sum = 0, v1, v2, val;
    struct ListNode *head, *tail, *node;
    
    node = malloc(sizeof(struct ListNode));
    node->val = 0;
    node->next = NULL;
    
    head =  node;
    sum = l1->val +l2->val;
    l1 = l1->next;
    l2= l2->next;
    while (l1 || l2) {
        if (!l1)
            v1 = 0;
        else {
            v1 = l1->val * weight;
            l1= l1->next;
        }
        
        if (!l2)
            v2 = 0;
        else {
            v2 = l2->val * weight;
            l2 = l2->next;
        }

        weight *= 10;
        sum += v1 + v2;
    }
    
    head->val = sum % 10;
    sum /= 10;
    tail = head;
    while ( sum ) {
        val = sum % 10;
        sum /= 10;
        node = malloc(sizeof(struct ListNode));
        node->val =val;
        node->next = NULL;
        tail->next = node;
        tail = node;
    }
    
    return head;
}

/*
 *
 * Success
 */
struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
    long  v1, v2, carry = 0;
    struct ListNode *head = NULL, *tail = NULL, *node;
    
    while (l1 || l2 || carry) {
        if (!l1)
            v1 = 0;
        else {
            v1 = l1->val;
            l1 = l1->next;
        }
        
        if (!l2)
            v2 = 0;
        else {
            v2 = l2->val;
            l2 = l2->next;
        }

	node = malloc(sizeof(struct ListNode));
	node->val = (v1 + v2 + carry) % 10;
	carry = (v1 + v2 + carry) / 10;
	node->next = NULL;
	if (tail)
		tail ->next = node;
	tail = node;
	if (!head)
		head =  node;
    }

    return head;
}
