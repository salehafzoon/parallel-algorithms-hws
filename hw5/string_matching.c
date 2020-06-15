#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define population_size 4
struct Individual
{
    char* chromosome;
    int fitness;
    
    void (*create)(struct Individual*,int size);
    void (*evaluate)(struct Individual*);
    void (*mutate)(struct Individual*);
    void (*print)(struct Individual*);
}Individual;

void evaluation(struct Individual* ind)
{
    ind->fitness = 10;
}
void mutation(struct Individual* ind)
{
    ind->fitness = 10;
}
void print_str(struct Individual* ind)
{
    printf("chromosome = %s\t fitness = %d\n",ind->chromosome,ind->fitness);
}
void creation(struct Individual* ind,int size) {
    
    ind->chromosome = (char*)malloc((size+1)*sizeof(char));
    strncpy(ind->chromosome, "", size+1);
    
    static const char alphanum[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

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
        
        *population[i] = (struct Individual){"", 100000, creation, evaluation, mutation, print_str};
        population[i]->create(population[i],size);
        population[i]->print(population[i]);
    }
    return population;
}

int main(void)
{

    int i=0;
    char target[20] = "saleh";
    int str_size = strlen(target);
    struct Individual** population;
    initialize_population(str_size);

    return 0;
}