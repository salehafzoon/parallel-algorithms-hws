// %%writefile ga.cu
//%%cu
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include<cuda.h>

#define MUTATION_RATE 2
#define MAX_GENERATION 200
#define DEBUG 0

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

void ga(int str_size,char* target, int population_size){
    
    int i,j,index,p1,p2;
    Individual** population;
    population = initialize_population(str_size,target,population_size);
    
    for (i = 0; i < MAX_GENERATION; i++)
        {
            qsort (population, population_size, sizeof(Individual*), compare);
            
            if (DEBUG){
                printf("iteration %d best: ",i);
                population[0]->print(population[0]);
            }

            //terminate condition
            if (population[0]->fitness == 0){
                printf("solution founded:\n");
                break;
            }

            index = population_size*0.02;
         
            for (j = index; j< population_size ; j+=2){
                        
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

        }
            
    population[0]->print(population[0]);

    //freeing pointers
    for (i = 0; i< population_size ; i++){
            free(population[i]);
        }
    free(population);

    return;
}
int main(int argc , char* argv[]){

	  //clock_t ser_msec;
	//   int population_size = atoi(argv[1]);
	  int population_size = 1000;

    printf("population size = %d \t max generation = %d \n ",population_size,MAX_GENERATION);

    char target[30] = "HELLO WORLD";
    int str_size = strlen(target);
    // int parallel = 0;
    ga(str_size,target,population_size);
 
    return 0;
}