
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<time.h>

#define MUTATION_RATE 1
#define MAX_GENERATION 10000
#define DEBUG 1
#define TOURNAMENT_SIZE 5

static const char alphanum[] =
        " "
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        // "abcdefghijklmnopqrstuvwxyz";

typedef struct Individual Individual;

int pop_size;
int max_iteration;
int full;

struct Individual
{
    char chromosome[20];
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
    
    char child_chor1[size];
    char child_chor2[size];
    
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

    strcpy(p1->chromosome ,child_chor1);
    strcpy(p2->chromosome ,child_chor2);
}

Individual** initialize_population(int size,char* target,int population_size){

    Individual** population = (Individual**)malloc(population_size * sizeof(Individual*)); 

    for (int i = 0; i < population_size; i++)
    {
        population[i] = (Individual*)malloc(sizeof(Individual));
        
        *population[i] = (Individual){" ", 100000, creation, evaluation, mutation,uniform_xover, print_str};
        population[i]->create(population[i],size,target);
    }
    return population;
}

Individual** initial_array(int pop_size,int str_size){
    Individual** arr = (Individual**)malloc(pop_size * sizeof(Individual*)); 

    for (int i = 0; i < pop_size; i++)
    {
        arr[i] = (Individual*)malloc(sizeof(Individual));
        
        *arr[i] = (Individual){" ", 100000, creation, evaluation, mutation,uniform_xover, print_str};
    }
    return arr;
}

int compare (const void * a, const void * b)
{

  Individual **ind1 = (Individual **)a;
  Individual **ind2 = (Individual **)b;

  return (*ind1)->fitness - (*ind2)->fitness;
}

int tournament_selection(Individual** population,int pop_size){
    int index = rand() % pop_size;
    int j;

    for (int i = 0; i < TOURNAMENT_SIZE; i++)
    {
        j = rand() % pop_size;
        if ( population[j]->fitness < population[index]->fitness )
            index = j;
    }
    return index;
}

void copy_array(Individual** src,Individual** dest,int start,int end){
    for (int i = start; i < end; i++)
    {
        *dest[i] = *src[i];
    }
}

void serial_eval(Individual** pop,int size,char* target){
    for (int i = 0; i < size; i++)
    {
        pop[i]->evaluate(pop[i],target);
    }   
}

void ga(int str_size,char* target,int pop_size,int max_iteration,int full){
    
    int i,j,begin,p1,p2,cap;
    Individual** pop,** next_pop,** dev_pop;
    
    pop = initialize_population(str_size,target,pop_size);
    next_pop = initial_array(2*pop_size,str_size);
    

    for (i = 0; i < max_iteration; i++)
        {
            qsort (pop, pop_size, sizeof(Individual*), compare);
            
            if (DEBUG){
                printf("iteration %d best: ",i);
                pop[0]->print(pop[0]);
            }

            //terminate condition
            
            if (full==0 && pop[0]->fitness == 0){
                printf("solution founded:\n");
                break;
            }
            

            // 10 percent of population move to next generation
            begin = pop_size*0.01;

            copy_array(pop,next_pop,0,begin);

            cap = begin;

            for (j = begin; j< pop_size ; j+=2){
                        
                p1 = tournament_selection(pop,pop_size);
                p2 = tournament_selection(pop,pop_size);

                pop[p1]->cross_over(pop[p1],pop[p2]);

                if (rand()%10 < MUTATION_RATE){
                    pop[p1]->mutate(pop[p1]);
                }
                if (rand()%10 < MUTATION_RATE){
                    pop[p2]->mutate(pop[p2]);
                }
            
                *next_pop[cap] = *pop[p1];
                *next_pop[cap+1] = *pop[p2];
                cap+=2;
            }

            serial_eval(next_pop,pop_size,target);

            qsort (next_pop, pop_size, sizeof(Individual*), compare);
            
            copy_array(next_pop,pop,0,pop_size);
            
        }
    
    qsort (pop, pop_size, sizeof(Individual*), compare);

    pop[0]->print(pop[0]);

    //freeing pointers
    for (i = 0; i< pop_size ; i++){
            free(pop[i]);
            free(next_pop[i]); 
        }

    // free the memory we allocated on the CPU
    free(pop);
    free(next_pop);
    
    return;
}
int main(int argc , char* argv[]){

    pop_size = atoi(argv[1]);
    max_iteration = atoi(argv[2]);
    full = atoi(argv[3]);
    
    printf("population size = %d \t max generation = %d \n ",pop_size,max_iteration);

    char target[30] = "HELLO WORLD";
    int str_size = strlen(target);

    clock_t start = clock();

    ga(str_size,target,pop_size,max_iteration,full);
    
    clock_t end = clock();
    float seconds = (float)(end - start) *1000000 / CLOCKS_PER_SEC;
    
    printf("\nelapsed Time = %.1f microseconds \n",seconds);

    return 0;
}