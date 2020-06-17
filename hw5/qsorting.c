/* qsort example */
#include <stdio.h>
#include <stdlib.h>

typedef struct Individual Individual;
struct Individual {
    int fitness;
    char* chromosome;

    void (*create)(struct Individual*,int size,char* target);
    void (*evaluate)(struct Individual*,char* target);
    void (*mutate)(struct Individual*);
    void (*cross_over)(struct Individual*,struct Individual*);
    
};

int compare (const void * a, const void * b)
{

  Individual *ind1 = (Individual *)a;
  Individual *ind2 = (Individual *)b;

  return ( ind1->fitness - ind2->fitness );
}

int main ()
{
    
    Individual list[6];
    int i = 0;
    
    for(i=0; i<6; i++){ 
        list[i].fitness = rand()%10;
        list[i].chromosome = "asd"; 
        printf ("Individual chromosome = %s fitness = %d \n",list[i].chromosome, list[i].fitness);           
    }
    
    printf("----------------\n");
    int n;
    qsort (list, 6, sizeof(Individual), compare);
    
    for (n=0; n<6; n++)
         printf ("Individual chromosome = %s fitness = %d \n",list[n].chromosome, list[n].fitness);          
    
    return 0;
}