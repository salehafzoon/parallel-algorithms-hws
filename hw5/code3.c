//%%cu
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MUTATION_RATE 1
#define MAX_GENERATION 300
#define DEBUG 0

#define numThread 50 // threads in a block

static const char alphanum[] =
        " "
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        // "abcdefghijklmnopqrstuvwxyz";

typedef struct Individual Individual;

int pop_size;

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
    free(p1->chromosome);
    free(p2->chromosome);
    
    p1->chromosome = child_chor1;
    p2->chromosome = child_chor2;
}

Individual** initialize_population(int size,char* target,int population_size){

    Individual** population = (Individual**)malloc(population_size * sizeof(Individual*)); 

    for (int i = 0; i < population_size; i++)
    {
        population[i] = (Individual*)malloc(sizeof(Individual));
        
        char* chrom = (char*)malloc(strlen(target) * sizeof(char));
     
        *population[i] = (Individual){chrom, 100000, creation, evaluation, mutation,uniform_xover, print_str};
        population[i]->create(population[i],size,target);
    }
    return population;
}

int compare (const void * a, const void * b)
{

  Individual **ind1 = (Individual **)a;
  Individual **ind2 = (Individual **)b;

  return (*ind1)->fitness - (*ind2)->fitness;
}

void serial_task (Individual** population,int population_size,char* target){
    int j,index,p1,p2;
    
    // percent of population move to next generation
    index = population_size*0.02;
         
    for (j = index; j< population_size ; j+=2){
                
        //p1 = (rand() % (int)(population_size * 0.6)) +index;
        //p2 = (rand() % (int)(population_size* 0.6)) +index;
        
        p1 = j;
        p2 = j+1;
        
        population[p1]->cross_over(population[p1],population[p2]);

        if (rand()%10 < MUTATION_RATE){
            population[p1]->mutate(population[p1]);
        }
        if (rand()%10 < MUTATION_RATE){
            population[p2]->mutate(population[p2]);
        }
    }

    for (j = 0; j< population_size ; j++){
                population[j]->evaluate(population[j],target);
            }
}

/*
__global__ 
void parallel_task(Individual** population,int population_size,char* target){
    
    // percent of population move to next generation
    int begin = population_size*0.02;
    
    // the initial index that this thread will work on
    int tid = begin + blockDim.x * blockIdx.x + threadIdx.x;
    
    while (tid < POP_SIZE -1) {
        
        population[tid]->cross_over(population[tid],population[tid+1]);

        
        if (rand()%10 < MUTATION_RATE){
            population[tid]->mutate(population[tid]);
        }
        if (rand()%10 < MUTATION_RATE){
            population[tid+1]->mutate(population[tid+1]);
        }
        
        
        population[tid]->evaluate(population[tid],target);
        population[tid+1]->evaluate(population[tid+1],target);
    }

}
*/

void ga(int str_size,char* target,int pop_size,int parallel){
    
    int i;
    Individual** population,dev_pop;
    population = initialize_population(str_size,target,pop_size);
    
    for (i = 0; i < MAX_GENERATION; i++)
        {
            qsort (population, pop_size, sizeof(Individual*), compare);
            
            if (DEBUG){
                printf("iteration %d best: ",i);
                population[0]->print(population[0]);
            }

            //terminate condition
            if (population[0]->fitness == 0){
                printf("solution founded:\n");
                break;
            }
            serial_task(population,pop_size,target);
            
        }
            
    population[0]->print(population[0]);

    //freeing pointers
    for (i = 0; i< pop_size ; i++){
            free(population[i]);
        }
    free(population);

    return;
}
int main(int argc , char* argv[]){

	// pop_size = atoi(argv[1]);
    // int parallel = atoi(argv[2]);
    
    pop_size = 300;
    int parallel = 0;
    
    printf("population size = %d \t max generation = %d \n ",pop_size,MAX_GENERATION);

    char target[30] = "HELLO WORLD";
    int str_size = strlen(target);
    ga(str_size,target,pop_size,parallel);
 
    return 0;
}