int myinit(int *array, int size);
int myinit_mt(int *array, int size);

int * mymalloc(int *array, int size);
int * mymalloc_mt(int *array, int size);

int myfree(int *array, int * block);
int myfree_mt(int *array, int * block);

int mydispose(int *array);
int mydispose_mt(int *array);
