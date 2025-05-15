#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>

typedef struct Node
{
    struct Node *next;
    int value;
} Node;

_Atomic Node *top = NULL; // 栈顶初始化为NULL
int SLIDE = 1000;

void push(Node **top_ptr, Node *n)
{
    n->next = *top_ptr;
    *top_ptr = n;
}

void safe_push(_Atomic Node **top_ptr, Node *n)
{
    Node *old_top;
    do {
        old_top = atomic_load(top_ptr);
        n->next = old_top;
    } while (!atomic_compare_exchange_weak(top_ptr, &old_top, n));
}

Node *pop(Node **top_ptr)
{
    if (*top_ptr == NULL)
    {
        return NULL;
    }
    Node *p = *top_ptr;
    *top_ptr = (*top_ptr)->next;
    return p;
}

Node *safe_pop(_Atomic Node **top_ptr)
{
    Node *old_top;
    do {
        old_top = atomic_load(top_ptr);
        if (old_top == NULL) {
            return NULL;
        }
    } while (!atomic_compare_exchange_weak(top_ptr, &old_top, old_top->next));
    return old_top;
}

void *thread_function(void *arg)
{
    for (int i = 0; i < SLIDE; i++)
    {
        Node *new_node = (Node *)malloc(sizeof(Node));
        new_node->value = *(int *)arg * SLIDE + i;

        if (new_node == NULL)
        {
            perror("Failed to allocate memory for new node");
            pthread_exit(NULL);
        }

        safe_push(&top, new_node); 
        // push(&top, new_node); 
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[10];
    int thread_args[10];

    // 创建线程
    for (int i = 0; i < 10; i++)
    {
        thread_args[i] = i;
        if (pthread_create(&threads[i], NULL, thread_function, (void *)&thread_args[i]))
        {
            perror("Failed to create the thread");
        }
    }

    // 等待线程结束
    for (int i = 0; i < 10; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // print
    Node *current = top;
    int count = 0;
    printf("content of stack:\n");
    while (current != NULL)
    {
        count++;
        // printf("%d\n", current->value);
        current = current->next; // 假设每个节点都有一个指向下一个节点的指针
    }
    printf("count = %d\n", count);

    return 0;
}