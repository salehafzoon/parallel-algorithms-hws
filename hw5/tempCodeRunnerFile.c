    int i=0;
    char target[30] = "HELLO WORLD";
    int str_size = strlen(target);
    struct Individual** population;
    
    
    printf("compare %d\n",strcmp("FYEYHUIRXNR ","HELLO WORLD"));
    
    population = initialize_population(str_size,target);

        
    for (i = 0; i < population_size; i++)
    {
     population[i]->print(population[i]);   
    }
