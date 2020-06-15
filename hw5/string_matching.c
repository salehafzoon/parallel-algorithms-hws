#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define population_size 10

static const char alphanum[] =
        " "
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        // "abcdefghijklmnopqrstuvwxyz";

struct Individual
{
    char* chromosome;
    int fitness;
    
    void (*create)(struct Individual*,int size);
    void (*evaluate)(struct Individual*);
    void (*mutate)(struct Individual*);
    void (*print)(struct Individual*);
};

void evaluation(struct Individual* ind)
{
    ind->fitness = 10;
}
void mutation(struct Individual* ind){

    int index = rand()%strlen(ind->chromosome);

    ind->chromosome[index] = alphanum[rand() % (sizeof(alphanum) - 1)];
}
void print_str(struct Individual* ind)
{
    printf("chromosome = %s\t fitness = %d\n",ind->chromosome,ind->fitness);
}
void creation(struct Individual* ind,int size) {
    
    ind->chromosome = (char*)malloc((size+1)*sizeof(char));
    strncpy(ind->chromosome, "", size+1);
    
    for (int i = 0; i < size; ++i) {
        ind->chromosome[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
}

struct Individual** initialize_population(int size){

    struct Individual** population = (struct Individual**)malloc(population_size * sizeof(struct Individual*)); 

    
    char *str;
    for (int i = 0; i < population_size; i++)
    {
        population[i] = (struct Individual*)malloc(sizeof(struct Individual));
        
        *population[i] = (struct Individual){"", rand()%20, creation, evaluation, mutation, print_str};
        population[i]->create(population[i],size);
        // population[i]->print(population[i]);
    }
    return population;
}

/* qsort fitness comparison function */ 
int sorter(const void *p1, const void *p2) {

    const struct Individual *in1 = (struct Individual *)p1;
    const struct Individual *in2 = (struct Individual *)p2;
    
    if (in1->fitness < in2->fitness)
        return -1;
    else if (in1->fitness > in2->fitness)
        return +1;
    else
        return 0;
}

int main(void)
{

    int i=0;
    char target[30] = "HELLO WORLD";
    int str_size = strlen(target);
    struct Individual** population;
    
    population = initialize_population(str_size);

        
    for (i = 0; i < population_size; i++)
    {
     population[i]->print(population[i]);   
    }
    // printf("----------------\n");

    // printf("%d\n",sorter( ((const void*)population[0]),((const void*)population[1])));

    // population[0]->print(population[0]);   
    // population[0]->mutate(population[0]);
    // population[0]->print(population[0]);
        
    // qsort(population, population_size, sizeof(struct Individual), sorter);

    // for (i = 0; i < population_size; i++)
    // {
    //     population[i]->print(population[i]);   
    // }

    return 0;
}