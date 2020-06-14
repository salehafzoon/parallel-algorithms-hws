#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define population_size 2
struct Individual
{
    char* chromosome;
    int fitness;
    
    void (*evaluate)(struct Individual*);
    void (*mutate)(struct Individual*);
    void (*print)(struct Individual*);
}Individual;

void fitness_evaluate(struct Individual* ind)
{
    ind->fitness = 10;
}
void mutation(struct Individual* ind)
{
    ind->fitness = 10;
}
void print_fitness(struct Individual* ind)
{
    printf("chromosome = %s\t fitness = %d\n",ind->chromosome,ind->fitness);
}

int main(void)
{

    int i;
    char target[20] = "saleh";

    struct Individual** population = (struct Individual**)malloc(population_size * sizeof(struct Individual*)); 

    for (i = 0; i < population_size; i++)
    {
        population[i] = (struct Individual*)malloc(sizeof(struct Individual));
        
        *population[i] = (struct Individual){"saleh", 0, fitness_evaluate, mutation, print_fitness};
    }
    for (i = 0; i < population_size; i++){
        population[i]->print(population[i]);
    }
    
    // struct Individual indiv1 = {"", 0, fitness_evaluate, mutation, print_fitness};

    // population[0]->evaluate(&population[0]);
    // population[0]->print(&population[0]);


    return 0;
}