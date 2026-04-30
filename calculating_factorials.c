#include <stdio.h>
#include <stdlib.h>

// THE NUMBER WHOSE FACTORIAL WE WISH TO COMPUTE
const int FACTORIAL_TO_COMPUTE = 42;

/**
 * Main idea:
 * Store numbers using linked lists to bypass standard issues of basic data type overflows.
 * To keep it simple, 1 digit per node (otherwise, say k (>= 2) digit data containing nodes, on multiplication, may be a bit more convoluted to consider overflow)
 * (although technically, we can most likely find a commmon ground to deal with, so that we can multiply 32-bit integers without overflowing and having more than a single digit)
 * I am doing 1 digit per node particularly as a demonstration for theoretical understanding, rather than an efficient implementation, as the techniques there shall stay the same, after choosing a proper number of digits for the threshold.
 * 
 * NOTE: Since we don't know the length of the number in terms of number of digits, the storage is reverse. That is the head refers to the last digit (units place) and the tail refers to the largest digit (first digit, basically like say if 1234, then the largest digit is 1)
 */

/**
 * As recommended in the Assignment Question, I am utilising Circular Doubly Connected Linked List.
 * Hence the standard Doubly Connected Linked List node is defined
 */
struct Node {
	int data;
	struct Node* prev;
	struct Node* next;
};

/**
 * @details Initialises a node to start off with
 * Since this is a circular doubly linked list, the pointers next and prev both map to itself.
 */
struct Node* createNode(int data) {
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->data = data;
	newNode->prev = newNode;
	newNode->next = newNode;
	return newNode;
}

/**
 * @details Inserts a new node to the start of the number (i.e. not the ones digit, but rather the most significant digit, i.e. the largest when expanded into 10^k expansion)
 * Since this is a circular list, we just add it as the prev of the head, and the next of the previous (head->prev)
 */
void insertTail(struct Node** head, int data) {
	struct Node* newNode = createNode(data);

	// If list is empty, this node becomes the head
	if (*head == NULL) {
		*head = newNode;
		return;
	}

	// In a circular list, the tail is always prev to the head
	struct Node* tail = (*head)->prev;

	// Link the new node into the circular doubly linked list
	newNode->next = *head;
	newNode->prev = tail;
	tail->next = newNode;
	(*head)->prev = newNode;
}

/**
 * @details Multiplication here works therefore, works similar to what we did in school.
 * So, we multiply our current number with the new multiplier
 * 
 * NOTE: This works on the assumption that we are specially making this for their usage in factorials, 
 * where small numbers blow up really fast. Then, the multiplier will not be too big for us to handle. 
 */
void multiply(struct Node** head, int multiplier) {
	int carry = 0;
	struct Node* temp = *head; // start at the units place

	// Task 1: Loop through every single digit we currently have
	do {
		int prod = (temp->data * multiplier) + carry;
		temp->data = prod % 10; // The remainder becomes the new digit in this node (so 24 -> keep 4)
		carry = prod / 10;      // The quotient is carried over to the next node (so 24 -> carry 2)
		temp = temp->next;
	} while (temp != *head); // we stop once we've done an overall complete round.

	// Task 2: If there's still a carry left over, append it as new nodes at the tail
    // basically, if we still have a carry left after checking all the digits, we need to add new digits to the start of our number (to keep the digits thing going.)
	while (carry > 0) {
		insertTail(head, carry % 10);
		carry = carry / 10;
	}
}

/**
 * @details To print the result, since the data is stored in a linked list, in reverse order (using circular doubly linked list properties)
 */
void printFactorial(struct Node* head) {
	if (head == NULL) return;

	// Jump instantly to the largest place value
	struct Node* tail = head->prev; // Tail is the first digit we're working with.
	struct Node* temp = tail;

	// Print backwards from tail down to head, printing 1 digit at a time.
	do {
		printf("%d", temp->data);
		temp = temp->prev;
	} while (temp != tail); // we stop once we've done an overall complete round.
	printf("\n");
}

/**
 * @details For good practices, this function frees the allocated memory after the work is done and dusted.
 */
void freeList(struct Node** head) {
	if (*head == NULL) return;
	struct Node* curr = *head;
	struct Node* next;
	struct Node* tail = (*head)->prev;

	// preventing infinite loop during cleanup by breaking the circle.
	tail->next = NULL; 

	while (curr != NULL) {
		next = curr->next;
		free(curr);
		curr = next;
	}
	*head = NULL;
}

int main() {
	int n = FACTORIAL_TO_COMPUTE;
	struct Node* head = NULL;
	insertTail(&head, 1);

	for (int i = 2; i <= n; i++) {
		multiply(&head, i);
	}

	printf("The full decimal expansion of %d! is:\n", n);
	printFactorial(head);

	// VERY VERY VERY IMPORTANT: Clean up memory before exiting
	freeList(&head);
	return 0;
}