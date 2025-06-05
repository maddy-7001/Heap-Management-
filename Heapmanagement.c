#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#define HeapSize 10000
char Heap[HeapSize];




typedef struct MetaData
{
    size_t size; // size_t is used specifically in memory allocation,deletion,array sizes,indexing,etc. So basically it stores size of each block.
    char status; //'f' for free || 'a' for allocated
    struct MetaData *next; 
}MetaBlock;




MetaBlock *heap_block_ptr = (void *)Heap;




void Initialize()
{
    
    heap_block_ptr->size = (HeapSize-(sizeof(MetaBlock)));
    printf("Size of Meta Data structure is %zu bytes \n",sizeof(MetaBlock));
    heap_block_ptr->status = 'f';
    heap_block_ptr->next = NULL;
    printf("Heap block initialized \n");
    printf("Address of starting of heap: %p \n",heap_block_ptr);
    printf("Ending address: %p \n",((heap_block_ptr->next)-1));
}




void * Allocate(size_t size_to_be_allocated)
{
    MetaBlock *trail,*lead;
    void *ret_ptr;
    trail = lead = ret_ptr = (void *)Heap;
    if((lead->next == NULL) && (lead->size < size_to_be_allocated))
    {
        printf("No free space left to allocate %zu bytes of memory \n",size_to_be_allocated);
        ret_ptr=NULL;
    }
    else
    {
        while((lead != NULL) && ((lead->size < size_to_be_allocated) || (lead->status == 'a')))
        {
            trail = lead;
            lead = lead->next;
        }
        if(lead->size == size_to_be_allocated)
        {
            lead->status = 'a';
            trail = lead;
            lead = lead->next;
            ret_ptr = lead;
            printf("Heap block having size exactly equal to %zu bytes is allocated \n",size_to_be_allocated);
        }
        else
        {
            MetaBlock *new_block = (void *)(((void *)lead) + (sizeof(MetaBlock)) + (size_to_be_allocated));
            new_block->size = ((lead->size) - (size_to_be_allocated) - (sizeof(MetaBlock)));
            new_block->next = lead->next;
            new_block->status = 'f';
            lead->size = size_to_be_allocated;
            lead->next = new_block;
            lead->status = 'a';
            ret_ptr = new_block;
        }
    }
    return ret_ptr;
}



void Merge()
{
    MetaBlock *merge_ptr, *temp;
    merge_ptr = heap_block_ptr;

    while (merge_ptr->next != NULL)
    {
        if ((merge_ptr->status == 'f') && ((merge_ptr->next)->status == 'f'))
        {
            merge_ptr->size += (merge_ptr->next->size) + sizeof(MetaBlock);
            merge_ptr->next = merge_ptr->next->next;
        }
        else
        {
            temp = merge_ptr;
            merge_ptr = merge_ptr->next;
        }
    }
}





void Free(void *free_ptr)
{
    if((((void *)Heap) <= (free_ptr)) && (((void *)(Heap + 10000)) >= (free_ptr)))
    {
        MetaBlock *ptr = (void *)Heap;
        while(ptr->next != free_ptr)
        {
            ptr = ptr->next;
        }
        ptr->status = 'f';
        Merge();
    }
    else
    {
        printf("Entered block of address to be freed is invalid \n");
    }
}




void DisplayHeap()
{
    MetaBlock *traverse_ptr = (void *)Heap;
    size_t index = 0;

    while (traverse_ptr != NULL)
    {
        printf("%zu - %zu \t", index, index + (traverse_ptr->size) - 1);
        printf("%c \n", traverse_ptr->status);
        index += (traverse_ptr->size);
        traverse_ptr = traverse_ptr->next;
    }
}





int main()
{
    Initialize();
    int *p = (int *)Allocate(40000 * sizeof(int));
    // char *c = (char *)Allocate(200 * sizeof(char));
    // int *i = (int *)Allocate(100 * sizeof(int));
    // printf("Heap memory status before freeing is as follows: \n");
    // DisplayHeap();
    // Free(i);
    // printf("Heap memory status after freeing i is as follows: \n");
    // DisplayHeap();
    // Free(p);
    // printf("Heap memory status after freeing p is as follows: \n");
    // DisplayHeap();
    // Free(c);
    // printf("Heap memory status after freeing c is as follows: \n");
    DisplayHeap();
    return 0;
}








#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define SIZE 1000

char Heap[SIZE];

typedef struct Metadata{
    int size;
    char mark;
    struct Metadata *next;
}Metadata;

//Meta data size is 12      

Metadata *head_ptr = (Metadata *)Heap;

void Initialize()
{
    head_ptr -> size = (SIZE - sizeof(Metadata));
    head_ptr -> mark = 'f';
    head_ptr -> next = NULL;
}


int Allocate(int request_size)
{
    Metadata *ptr = head_ptr;
    Metadata *prev = NULL;
    int index = 0;

    while (ptr != NULL && (ptr->mark == 'a' || ptr->size < request_size))
    {
        prev = ptr;
        ptr = ptr->next;
        index++;
    }

    if (ptr == NULL)
    {
        index = -1; // No block available
    }
    else
    {
        if (ptr->size == request_size)
        {
            ptr->mark = 'a';
        }
        else
        {
            // Metadata *new_block = (Metadata *)malloc(sizeof(Metadata));
            Metadata new_block = (void)((void *)ptr + sizeof(Metadata) + request_size);
            new_block->size = ptr->size - (request_size + sizeof(Metadata));
            new_block->mark = 'f';
            new_block->next = ptr->next;

            ptr->size = request_size;
            ptr->mark = 'a';
            ptr->next = new_block;
        }
    }
    return index; // Index of allocated block
}


void Merge()
{
    Metadata *ptr = head_ptr;
    Metadata *next_ptr;
    

    while(ptr != NULL && ptr -> next != NULL)
    {
        next_ptr = ptr -> next;
        
        if(ptr -> mark == 'f' && next_ptr -> mark == 'f')
        {
            ptr -> size = (ptr -> size)+(next_ptr -> size)+(sizeof(Metadata));
           
            ptr -> next = next_ptr -> next;
            //merged
        }
        
        ptr = ptr -> next;
        
    }

    

    
    //merging has been completed
}

bool Free(int index)
{
    Metadata *ptr;
    ptr = head_ptr;
    bool ret_val = 0;

    while(index != 0 && ptr != NULL)
    {
        ptr = ptr -> next;
        index--;
    }
    
    
    if(index == 0 && ptr != NULL && ptr -> mark == 'a')   //block to be freed id found
    {
        ptr -> mark = 'f';
        ret_val = 1;
    }

    Merge();

    return ret_val;

}

void Display_Heap()
{
    Metadata *ptr = head_ptr;
    printf("Heap Details : \n");
    printf("\t Block Size \t \t \t Block status \t \t \t \t Block Address \t \t \t Block Index \n");

    int index = 0;

    while(ptr != NULL)
    {
        printf("\t \t %d \t \t \t \t %c \t \t \t \t %p \t \t \t %d\n",ptr -> size,ptr -> mark,(void *)ptr,index);
        ptr = ptr -> next;
        index++;
    }

}

int main()
{
    Initialize();

    
    int flag = 0;
    while(flag == 0)
    {
        
            printf("Choose the function you want to perform \n");
            int n;
            printf(" 1. \t Allocate memory \n 2. \t De-allocate memory \n 3. \t Display heap elements \n 4. \t Quit \n");
            printf("Enter : ");
            scanf("%d",&n);
            if(n == 1)
            {
                int size;
                printf("Enter the size of block you want to allocate : \t");
                scanf("%d",&size);
                int ret_val;
                ret_val = Allocate(size);
                if(ret_val == -1)
                {
                    printf("Failed to assign memory \n");
                }
                else
                {
                    printf("Required memory is successfully assigned at index : %d \n",ret_val);
                }
            }
                
               
                
            
            if(n == 2) 
            {
                printf("Enter the index you wan to free : ");
                int index;
                scanf("%d",&index);
                bool ret_val = Free(index);
                if(ret_val == true)
                {
                    printf("The de-allocation is done successfully \n");
                }
                else
                {
                    printf("De-allocation has failed \n");
                }
                Merge();
            }   
                
                
                

            if(n == 3)
            {
                Display_Heap();
                printf("\n");
            }
                
            
                
                

            if(n == 4)
                {
                flag = 1;
                }
        }
    
    
    return 0;

}