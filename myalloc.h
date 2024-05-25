struct node
{
unsigned size;
struct node *next;
};
void sort_by_base(struct node*b);
void free_node();
void myfree(void *a);
void *myalloc(unsigned n);

