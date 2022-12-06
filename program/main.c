#include <stdio.h>
#include <stdlib.h>


// food type is given as enum, where the int values are the number of days the food products last in the fridge
// this helps us with our calculations, as well as makes it easy for the user to input food type
typedef enum {SEAFOOD = 3, BAKEDGOODS = 4, MEAT = 5, GREENS = 6, EGGS = 7, DAIRY = 8} foodType;


typedef struct {
    char regionName[100];
    foodType foodType;
    int numOfProducers;
    int* baseExcessPerOrg;
    int numOfOrganizations;
    int* demandPerOrg;
    double* excessVolatility;
    double* costPerUnit;
    double* transportCost;
} RegionStruct;


typedef struct {
    char* regionName;
    char* foodType;
    int foodSaved;
    int foodWasted;
    double costOfUnits;
    double costOfTransport;
} RegionResultStruct;

RegionStruct* readFile(int* numOfRegions) {

// reads the file given by the user for data for the region(s)

    RegionStruct* newRegion;

// open the file and load how many regions there are
    FILE* regionsFile = fopen("settings.txt", "r");
    fscanf(regionsFile, " %d%*c", numOfRegions);

// allocating memory with malloc for our region structs (array if more than 1 region)
    newRegion = malloc(sizeof (RegionStruct) * *numOfRegions);

    // for-loop for loading all data for regions

    for (int i = 0; i < *numOfRegions; ++i) {
        fscanf(regionsFile, " %[0-9A-Za-z]%*c %u%*c %d%*c", newRegion[i].regionName,
               &newRegion[i].foodType, &newRegion[i].numOfProducers);

        //we allocate memory for excess produce, excess volatility, cost per unit and transport cost
        newRegion[i].baseExcessPerOrg = malloc(sizeof (int) * newRegion[i].numOfProducers);
        newRegion[i].excessVolatility = malloc(sizeof (double) * newRegion[i].numOfProducers);
        newRegion[i].costPerUnit = malloc(sizeof (double) * newRegion[i].numOfProducers);
        newRegion[i].transportCost = malloc(sizeof (double) * newRegion[i].numOfProducers);

        //we scan the base excess for each producer
        for (int j = 0; j < newRegion[i].numOfProducers; ++j) {
            fscanf(regionsFile, " %d%*c", &newRegion[i].baseExcessPerOrg[j]);
        }

        fscanf(regionsFile, " %d%*c", &newRegion[i].numOfOrganizations);

        //we allocate memory for demand
        newRegion[i].demandPerOrg = malloc(sizeof (int) * newRegion[i].numOfOrganizations);

        //we scan the demand for each organization
        for (int j = 0; j < newRegion[i].numOfOrganizations; ++j) {
            fscanf(regionsFile, " %d%*c", &newRegion[i].demandPerOrg[j]);
        }

        //we scan excess volatility for each producer
        for (int j = 0; j < newRegion[i].numOfProducers; ++j) {
            fscanf(regionsFile, " %lf%*c", &newRegion[i].excessVolatility[j]);
        }

        //we scan cost per unit for each producer
        for (int j = 0; j < newRegion[i].numOfProducers; ++j) {
            fscanf(regionsFile, " %lf%*c", &newRegion[i].costPerUnit[j]);
        }

        //we scan transport cost for each producer
        for (int j = 0; j < newRegion[i].numOfProducers; ++j) {
            fscanf(regionsFile, " %lf%*c", &newRegion[i].transportCost[j]);
        }
    }

    return newRegion;
}

// if the user chooses to input data themselves
RegionStruct* readFromTerminal(int* numOfRegions) {
    int regions, producers, organizations, foodtype;

    printf("Please enter the number of regions: ");
    scanf(" %d", numOfRegions);

    // allocating memory for the region struct
    RegionStruct* newRegion = malloc(sizeof(RegionStruct) * *numOfRegions);

    for (int i = 0; i < *numOfRegions; ++i) {
        printf("Please enter information for region %d: \n", i + 1);
        printf("Region name (less than 100 characters): ");
        scanf(" %s", newRegion[i].regionName);

        printf("How many producers are there in the region?: ");
        scanf(" %d", &producers);

        newRegion[i].numOfProducers = producers;

        // allocating memory for excess, volatility, cost per unit and transport cost based on the number of producers
        newRegion[i].baseExcessPerOrg = malloc(sizeof(int) * producers);
        newRegion[i].excessVolatility = malloc(sizeof(double) * producers);
        newRegion[i].costPerUnit = malloc(sizeof(double) * producers);
        newRegion[i].transportCost = malloc(sizeof(double) * producers);

        printf("Please enter excess food per organization (seperated by space): ");

        for (int j = 0; j < producers; ++j){
            scanf(" %d", &newRegion[i].baseExcessPerOrg[j]);
        }

        printf("Please input food type for region:\n");
        printf("Type 1 for meat\n");
        printf("Type 2 for greens\n");
        printf("Type 3 for dairy\n");
        printf("Type 4 for seafood\n");
        printf("Type 5 for baked goods\n");
        printf("Type 6 for eggs\n");
        scanf(" %d", &foodtype);

        switch(foodtype){
            case 1:
                newRegion[i].foodType = MEAT;
                break;
            case 2:
                newRegion[i].foodType = GREENS;
                break;
            case 3:
                newRegion[i].foodType = DAIRY;
                break;
            case 4:
                newRegion[i].foodType = SEAFOOD;
                break;
            case 5:
                newRegion[i].foodType = BAKEDGOODS;
                break;
            case 6:
                newRegion[i].foodType = EGGS;
                break;
            default:
                newRegion[i].foodType = MEAT;
        }

        printf("Please enter volatility of excess as a multiplier per producer (0: the excess is constant  "
               "1: the excess fluctuates between double of base and zero): ");

        for (int j = 0; j < producers; ++j){
            scanf(" %lf", &newRegion[i].excessVolatility[j]);
        }

        printf("Please enter cost per unit of the excess food per producer (seperated by space): ");

        for (int j = 0; j < producers; ++j){
            scanf(" %lf", &newRegion[i].costPerUnit[j]);
        }

        printf("Please enter transportation costs of the excess food per producer (seperated by space): ");

        for (int j = 0; j < producers; ++j){
            scanf(" %lf", &newRegion[i].transportCost[j]);
        }

        printf("How many organizations in the region?: ");
        scanf(" %d", &organizations);

        newRegion[i].numOfOrganizations = organizations;
        newRegion[i].demandPerOrg = malloc(sizeof(int) * organizations);

        printf("Please enter demand per organization (seperated by space): ");

        for (int j = 0; j < producers; ++j){
            scanf(" %d", &newRegion[i].demandPerOrg[j]);
        }
    }
    return newRegion;
}


void saveFile(RegionStruct* region, int numOfRegions) {

    FILE* regionsFile = fopen("Result.txt", "w");

    fprintf(regionsFile, "%d, ", numOfRegions);

    for (int i = 0; i < numOfRegions; ++i) {
        fprintf(regionsFile, "%s, %d, %d, ", region[i].regionName, region[i].foodType, region[i].numOfProducers);

        for (int j = 0; j < region[i].numOfProducers; ++j) {
            fprintf(regionsFile, "%d, ", region[i].baseExcessPerOrg[j]);
        }

        fprintf(regionsFile, "%d, ", region[i].numOfOrganizations);

        for (int j = 0; j < region[i].numOfOrganizations; ++j) {
            fprintf(regionsFile, "%d, ", region[i].demandPerOrg[j]);
        }


        for (int j = 0; j < region[i].numOfProducers; ++j) {
            fprintf(regionsFile, "%.2lf, ", region[i].excessVolatility[j]);
        }

        //we scan cost per unit for each producer
        for (int j = 0; j < region[i].numOfProducers; ++j) {
            fprintf(regionsFile, "%.2lf, ", region[i].costPerUnit[j]);
        }

        //we scan transport cost for each producer
        for (int j = 0; j < region[i].numOfProducers; ++j) {
            fprintf(regionsFile, "%.2lf, ", region[i].transportCost[j]);
        }
    }

}


char* convertFoodtypeEnum(foodType input){
    char* output;

    switch (input) {
        case SEAFOOD:
            output = "Seafood";
            break;
        case BAKEDGOODS:
            output = "Bakedgoods";
            break;
        case MEAT:
            output = "Meat";
            break;
        case GREENS:
            output = "Greens";
            break;
        case EGGS:
            output = "Eggs";
            break;
        case DAIRY:
            output = "Dairy";
            break;
        default:
            output = "ERROR";
    }
    return output;
}


void calculateIteration(RegionStruct* regions, RegionResultStruct* results) {

    for (int i = 0; i < 1; ++i) {
        results[i].costOfTransport = regions[i].transportCost[i]; //can be expanded upon
        results[i].costOfUnits = regions[i].costPerUnit[i];
        results[i].foodWasted = regions[i].baseExcessPerOrg[i];
        results[i].foodSaved = regions[i].baseExcessPerOrg[i] * 1.2; //here we would input the amount of food redistributed to the people
        results[i].foodType = convertFoodtypeEnum(regions[i].foodType);
        results[i].regionName = "mogens";
    }

}


void outputResult(RegionResultStruct* results, int numberOfRegions) {

    int i;
    for(i = 0; i < numberOfRegions; i++)
    {
        printf("Region\tFood type\tFood saved\tFood wasted\tCost of food\tCost of Transport\n");
        printf("%s\t", results[i].regionName);
        printf("%s\t\t", results[i].foodType);
        printf("%d\t\t", results[i].foodSaved);
        printf("%d\t\t", results[i].foodWasted);
        printf("%.2lf\t\t", results[i].costOfUnits);
        printf("%.2lf\n", results[i].costOfTransport);
    }

    /*
     * Result format:
     * Region    food type    food saved    food wasted    cost of food    cost of transport
     * region1   Eggs        126            35            430.12           232.12
     * region2   Seafood     832            793           1534.54          12.2
     * Mogens    bakedgoods  23             46            242.75           971.23
     * Robert    meat        427            0             623.84           726.45
     */

}


int main() {

    char input;
    int numberOfRegions;
    RegionStruct* regions;
    RegionResultStruct* results;

    //ask if user wants to read file or terminal
    printf("Would you like to load a file? (y/n)");
    while (1) {
        scanf("%c", &input);

        if(input == 'y'){
            regions = readFile(&numberOfRegions);
            results = malloc(sizeof (RegionResultStruct) * numberOfRegions);
            break;
        } else if(input == 'n') {
            regions = readFromTerminal(&numberOfRegions);
            results = malloc(sizeof (RegionResultStruct) * numberOfRegions);
            break;
        } else {
            printf("Please input 'y' or 'n'");
        }
    }

    printf("%s, %d, %d, %d, %d, %d, %.2lf, %.2lf, %.2lf\n", regions[0].regionName, regions[0].foodType, regions[0].numOfProducers,
           regions[0].baseExcessPerOrg[0], regions[0].numOfOrganizations, regions[0].demandPerOrg[0],
           regions[0].excessVolatility[0], regions[0].costPerUnit[0], regions[0].transportCost[0]);

    calculateIteration(regions, results);

    outputResult(results, numberOfRegions);

    saveFile(regions, numberOfRegions);

    free(regions);

    return 0;
}