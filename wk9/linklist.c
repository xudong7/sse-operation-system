#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct Node
{
    struct Node *next;
    int value;
} Node;

Node *top = NULL; // 栈顶初始化为NULL
int SLIDE = 1000;

void push(Node **top_ptr, Node *n)
{
    n->next = *top_ptr;
    *top_ptr = n;
}

void safe_push(Node **top_ptr, Node *n)
{
    pthread_mutex_lock(&lock);
    push(top_ptr, n);
    pthread_mutex_unlock(&lock);
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

        safe_push(&top, new_node); // 可以把这句换成普通的push，感受一下非线程安全的结果
        // push(&top, new_node); 
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[10];
    int thread_args[10];

    // 初始化互斥锁
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

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

    // 销毁互斥锁
    pthread_mutex_destroy(&lock);

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