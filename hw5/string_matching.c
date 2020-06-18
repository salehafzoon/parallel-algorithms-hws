// # %%writefile ga.cu
// %%cu
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define population_size 100
#define max_iteration 20
#define DEBUG 1

static const char alphanum[] =
        " "
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        // "abcdefghijklmnopqrstuvwxyz";

typedef struct Individual Individual;

struct Individual
{
    char* chromosome;
    int fitness;
    
    void (*create)(Individual*,int size,char* target);
    void (*evaluate)(Individual*,char* target);
    void (*mutate)(Individual*);
    void (*cross_over)(Individual*,Individual*);
    void (*print)(Individual*);
};

int str_compare(char* str1,char* str2){
    int diff = 0;
    for (int i = 0; i < strlen(str1); i++)
    {
        diff += (abs(str1[i]-str2[i]));
    }
    return diff;
}

void evaluation(struct Individual* ind,char* target)
{
    ind->fitness = str_compare(ind->chromosome,target);

}

void mutation(struct Individual* ind){

    int index = rand()%strlen(ind->chromosome);

    ind->chromosome[index] = alphanum[rand() % (sizeof(alphanum) - 1)];
}

void print_str(Individual* ind)
{
    printf("chromosome = %s\t fitness = %d\n",ind->chromosome,ind->fitness);
}

void creation(Individual* ind,int size,char* target) {
    
    ind->chromosome = (char*)malloc((size+1)*sizeof(char));
    strncpy(ind->chromosome, "", size+1);
    
    for (int i = 0; i < size; ++i) {
        ind->chromosome[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    ind->evaluate(ind,target);
}

void uniform_xover(Individual* p1 ,Individual* p2){
    int size = strlen(p1->chromosome);
    
    char *child_chor1 = (char*)malloc((size)*sizeof(char));
    char *child_chor2 = (char*)malloc((size)*sizeof(char));

    for (int i = 0; i < size; i++)
    {
        if (rand()%10 < 5){
            child_chor1[i] = p1->chromosome[i];
            child_chor2[i] = p2->chromosome[i];
        }
        else{
            child_chor2[i] = p1->chromosome[i];
            child_chor1[i] = p2->chromosome[i];
        }
    }
    p1->chromosome = child_chor1;
    p2->chromosome = child_chor2;
}

Individual** initialize_population(int size,char* target){

    Individual** population = (Individual**)malloc(population_size * sizeof(Individual*)); 

    char *str;
    for (int i = 0; i < population_size; i++)
    {
        population[i] = (Individual*)malloc(sizeof(Individual));
        
        *population[i] = (Individual){"", rand()%20, creation, evaluation, mutation,uniform_xover, print_str};
        population[i]->create(population[i],size,target);
        // population[i]->print(population[i]);
    }
    return population;
}

int compare (const void * a, const void * b)
{

  Individual **ind1 = (Individual **)a;
  Individual **ind2 = (Individual **)b;

  return (*ind1)->fitness - (*ind2)->fitness;
}

void serial_ga(int str_size,char* target){
    
    // int i;
    Individual** population;
    population = initialize_population(str_size,target);
    
    for (int i = 0; i < max_iteration; i++)
        {
            qsort (population, population_size, sizeof(Individual*), compare);
            
            if (DEBUG){
                printf("iteration %d best: ",i);
                population[0]->print(population[0]);
            }

            //terminate condition
            if (population[0]->fitness == 0){
                break;
            }

            //10 percent of population move to next generation
            int index = ((int)0.1*population_size);
            
            for (int j = index; j< population_size ; j+=2){
                
                population[j]->cross_over(population[j],population[j+1]);

                if (rand()%10 <2){
                    population[j]->mutate(population[j]);
                }
                if (rand()%10 <2){
                    population[j+1]->mutate(population[j+1]);
                }
            }

            for (int j = 0; j< population_size ; j++){
                population[j]->evaluate(population[j],target);
            }

        }
            
    printf("solution founded:\n");
    population[0]->print(population[0]);

    //freeing pointers
    for (int i = 0; i< population_size ; i++){
            free(population[i]);
        }
    free(population);

    return;
}
int main(void)
{
    int i=0;
    
    char target[30] = "HELLO WORLD";
    int str_size = strlen(target);
    serial_ga(str_size,target);
 
    /*

    Individual** population;
    population = initialize_population(str_size,target);
 
    for (i = 0; i < population_size; i++)
        {
        population[i]->print(population[i]);   
        }

    //qsort (population, population_size, sizeof(Individual*), compare);

    population[0]->cross_over(population[0],population[1]);
    printf("------------------\n");

    for (i = 0; i < population_size; i++)
        {
        population[i]->print(population[i]);   
        }

    population[0]->mutate(population[0]);
    printf("after mutate child 1\n");
    population[0]->print(population[0]);   
    */
    return 0;
}