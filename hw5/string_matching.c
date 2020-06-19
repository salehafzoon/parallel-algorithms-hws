// # %%writefile ga.cu
// %%cu
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define population_size 10
#define max_iteration 3
#define DEBUG 1

static const char alphanum[] =
        " "
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        // "abcdefghijklmnopqrstuvwxyz";

typedef struct Individual Individual;
typedef struct Childs Childs;

struct Individual
{
    char* chromosome;
    int fitness;
    
    void (*create)(Individual*,int size,char* target);
    void (*evaluate)(Individual*,char* target);
    void (*mutate)(Individual*);
    Childs (*cross_over)(Individual* ,Individual* , Individual *,Individual *);
    void (*print)(Individual*);
};
struct Childs
{
    Individual * child1;
    Individual * child2; 
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

void uniform_xover(Individual* p1 ,Individual* p2 , Individual *c1,Individual *c2){
    
    
    int size = strlen(p1->chromosome);
    
    // printf("\nchilds in xover and size = %d \n",size);
    // c1->print(c1);
    // c2->print(c1);

    for (int i = 0; i < size; i++)
    {
        if (rand()%10 < 10){
            c1->chromosome[i] = p1->chromosome[i];
            c2->chromosome[i] = p2->chromosome[i];
        }
        else{
            c2->chromosome[i] = p1->chromosome[i];
            c1->chromosome[i] = p2->chromosome[i];
        }
    
    }
    
}


Individual** initialize_population(int size,char* target){

    Individual** population = (Individual**)malloc(population_size * sizeof(Individual*)); 

    char *str;
    for (int i = 0; i < population_size; i++)
    {
        population[i] = (Individual*)malloc(sizeof(Individual));
        
        *population[i] = (Individual){"", 100000, creation, evaluation, mutation,uniform_xover, print_str};
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

Individual** allocate_space(){
    Individual** next_generation = (Individual**)malloc(population_size * sizeof(Individual*)); 

    for (int i = 0; i < population_size; i++)
    {
        next_generation[i] = (Individual*)malloc(sizeof(Individual));
    }

    return next_generation;
}

void print_array(Individual** arr){
    for (int i = 0; i < population_size; i++)
    {
        arr[i]->print(arr[i]);
    }
    printf("---------------\n");

}
void serial_ga(int str_size,char* target){
    
    int i,j;
    int p1,p2;
    Individual *child1,*child2;
    child1 = (Individual*)malloc((str_size)*sizeof(Individual));
    child2 = (Individual*)malloc((str_size)*sizeof(Individual));
   
    *child1 = (Individual){"", 100000, creation, evaluation, mutation,uniform_xover, print_str};
    *child2 = (Individual){"", 100000, creation, evaluation, mutation,uniform_xover, print_str};

    child1->create(child1,str_size,target);
    child2->create(child2,str_size,target);
    
    // child1->print(child1);
    // child2->print(child2);
    
    
    Individual** population = initialize_population(str_size,target);
    Individual** next_generation = initialize_population(str_size,target);
    
    for (i = 0; i < max_iteration; i++)
        {
            qsort (population, population_size, sizeof(Individual*), compare);

            if (DEBUG){
                print_array(population);
                printf("iteration %d best: ",i);
                population[0]->print(population[0]);
            }

            //terminate condition
            if (population[0]->fitness == 0){
                printf("solution founded:\n");
                break;
            }
            
            for (j = 0; j< population_size ; j+=2){
                
                p1 = rand() % population_size;
                p2 = rand() % population_size;
                // printf("p1:%d \t p2:%d",p1,p2);

                population[p1]->cross_over(population[p1],population[p2],child1,child2);

                if (rand()%10 <1){
                    child1->mutate(child1);
                    child2->mutate(child2);
                }

                child1->evaluate(child1,target);
                child2->evaluate(child2,target);

                // printf("---------childrens----------\n");
                // child1->print(child1);
                // child2->print(child2);
                
                *next_generation[j] = *child1;
                *next_generation[j+1] = *child2;
                
                // printf("---------childs in next generation----------\n");
                // next_generation[j]->print(next_generation[j]);
                // next_generation[j+1]->print(next_generation[j+1]);
                
            }
            
            qsort (next_generation, population_size, sizeof(Individual*), compare);
            
            printf("---------next generation----------\n");
            print_array(next_generation);

            // 20% of previous generation best and 80% of next_generation best
            for (j = population_size * 0.2 ; j< population_size ; j++){
                *population[j] = *next_generation[j];
                
                // population[j]->fitness = next_generation[j]->fitness;
                // population[j]->chromosome = next_generation[j]->chromosome;
                
            }

        }
            
    population[0]->print(population[0]);

    // freeing pointers
    for (int i = 0; i< population_size ; i++){
            free(population[i]);
            free(next_generation[i]);   
        }
    free(population);
    free(next_generation);
    free(child1);
    free(child2);

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