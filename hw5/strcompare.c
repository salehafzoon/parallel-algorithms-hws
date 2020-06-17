/* qsort example */
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int fitness;

} Individual;

Individual list[6];
int i = 0;

int compare (const void * a, const void * b)
{

    Individual *in1 = (Individual *)a;
    Individual *in2 = (Individual *)b;

    if (in1->fitness > in2->fitness)
        return 1;
    else
        return -1;

    // return ( in1->fitness - in2->fitness );
}

int main ()
{
    srand ( time(NULL) );

    printf("Before sorting\n");
    for(i=0; i<6; i++){ 
        list[i].fitness = rand()%10;
        printf ("fitness = %d \n",list[i].fitness);   
                
    }
    printf("AFTER sorting\n");
    int n;
    qsort (list, 6, sizeof(Individual), compare);

    for (n=0; n<6; n++)
        printf ("fitness = %d \n",list[i].fitness);   
    return 0;
}