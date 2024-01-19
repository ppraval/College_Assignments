#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct paitentOP
{
    char* name;
    int priority;
    char* time; 
};

typedef struct paitentOP *paitentOP;

struct OPqueue
{
    paitentOP arr[10000];
    int n;
};

typedef struct OPqueue *OPqueue;

paitentOP createOP(char* name, int priority, char* time)
{
    paitentOP p = (paitentOP)malloc(sizeof(struct paitentOP));
    p->name = name;
    p->priority = priority;
    p->time = time;
    return p;
}

OPqueue createQueue()
{
    OPqueue new_q = (OPqueue)malloc(sizeof(struct OPqueue));
    new_q->n = 0;
    return new_q;
}

void max_heapify(OPqueue q, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if(left < q->n && q->arr[left]->priority >= q->arr[largest]->priority)
    {
        if(q->arr[left]->priority == q->arr[largest]->priority)
        {
            if(strcmp(q->arr[largest]->time, q->arr[left]->time) > 0)
            {
                largest = left;
            }
        }
        else
        {
            largest = left;
        }
    }
    if(right < q->n && q->arr[right]->priority >= q->arr[largest]->priority)
    {
        if(q->arr[right]->priority == q->arr[largest]->priority)
        {
            if(strcmp(q->arr[largest]->time, q->arr[right]->time) > 0)
            {
                largest = right;
            }
        }
        else
        {
            largest = right;
        }
    }
    if(largest != i)
    {
        paitentOP temp = q->arr[i];
        q->arr[i] = q->arr[largest];
        q->arr[largest] = temp;
        max_heapify(q, largest);
    }
}

void build_max_heap(OPqueue q)
{
    for(int i = q->n / 2 - 1; i >= 0; i--)
    {
        max_heapify(q, i);
    }
}

void change_value(OPqueue q, int i)
{
    int parent = (i - 1) / 2;
    if(parent > 0 && q->arr[i]->priority > q->arr[parent]->priority)
    {
        paitentOP temp = q->arr[i];
        q->arr[i] = q->arr[parent];
        q->arr[parent] = temp;
        i = parent;
    }
}

void admitPaitent(OPqueue q, paitentOP p)
{
    q->arr[(q->n)++] = p;
    change_value(q, (q->n) - 1);
    build_max_heap(q);
}

void treatNextPatient(OPqueue q)
{
    if(q->n == 0)
    {
        return;
    }
    paitentOP temp = q->arr[0];
    q->arr[0] = q->arr[q->n - 1];
    q->arr[q->n] = temp;
    (q->n)--;
    build_max_heap(q);
}

void checkNextPatient(OPqueue q)
{
    if(q->n == 0)
    {
        printf("-1\n");
        return;
    }
    printf("%s %d %s\n", q->arr[0]->name, q->arr[0]->priority, q->arr[0]->time);
}

void paitentInfo(OPqueue q)
{
    struct OPqueue temp = *q;
    int n = temp.n;
    for(int i = 0; i < n; i++)
    {
        checkNextPatient(&temp);
        treatNextPatient(&temp);
    }
}

void updateConditionSeverity(OPqueue q, char* name, char* time, int new_proirity)
{
    for(int i = 0; i < q->n; i++)
    {
        if(strcmp(q->arr[i]->name, name) == 0 && strcmp(q->arr[i]->time, time) == 0)
        {
            q->arr[i]->priority = new_proirity;
            break;
        }
    }
    build_max_heap(q);
}

void dischargePaitent(OPqueue q, char* name, char* time)
{
    for(int i = 0; i < q->n; i++)
    {
        if(strcmp(q->arr[i]->name, name) == 0 && strcmp(q->arr[i]->time, time) == 0)
        {
            paitentOP temp = q->arr[i];
            q->arr[i] = q->arr[q->n - 1];
            q->arr[q->n] = temp;
            (q->n)--;
            break;
        }
    }
    build_max_heap(q);
}

int main()
{   
    OPqueue q = createQueue();
    char ch;
    char* name;
    char* time;
    int val;
    while(1)
    {
        name = (char*)malloc(sizeof(char)*100);
        time = (char*)malloc(sizeof(char)*100);
        scanf("%c", &ch);
        if(ch == 'a')
        {
            scanf("%s%d%s", name, &val, time);
            paitentOP p = createOP(name, val, time);
            admitPaitent(q, p);
        }
        else if(ch == 't')
        {
            checkNextPatient(q);
            treatNextPatient(q);
        }
        else if(ch == 'c')
        {
            checkNextPatient(q);
        }
        else if(ch == 'd')
        {
            scanf("%s%s", name, time);
            dischargePaitent(q , name, time);
        }
        else if(ch == 'u')
        {
            scanf("%s%s%d", name, time, &val);
            updateConditionSeverity(q, name, time, val);
        }
        else if(ch == 'p')
        {
            paitentInfo(q);
        }
        else if(ch == 'e')
        {
            break;
        }
    }
    return 1;   
}