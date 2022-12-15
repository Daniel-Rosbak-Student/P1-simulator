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
    double* distanceToOrg;
    double* excessVolatility;
    double* costPerUnit;
    double* transportCost;
} RegionStruct;


typedef struct {
    char* regionName;
    foodType foodType;
    int foodSaved;
    int foodWasted;
    int unmetDemand;
    double totalCost;
} RegionResultStruct;

//if the user chooses to load settings from a file, this function is called
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

        newRegion[i].distanceToOrg = malloc(sizeof (int) * newRegion[i].numOfOrganizations);

        //we scan the distance to each organization
        for (int j = 0; j < newRegion[i].numOfOrganizations; ++j) {
            fscanf(regionsFile, " %lf%*c", &newRegion[i].distanceToOrg[j]);
        }

        //we scan excess volatility for each producer
        for (int j = 0; j < newRegion[i].numOfProducers; ++j) {
            fscanf(regionsFile, " %lf%*c", &newRegion[i].excessVolatility[j]);
        }

        //we scan cost per unit for each producer
        for (int j = 0; j < newRegion[i].numOfProducers; ++j) {
            fscanf(regionsFile, " %lf%*c", &newRegion[i].costPerUnit[j]);
        }

        //we scan transport cost per km for each producer
        for (int j = 0; j < newRegion[i].numOfProducers; ++j) {
            fscanf(regionsFile, " %lf%*c", &newRegion[i].transportCost[j]);
        }
    }

    return newRegion;
}

// if the user chooses to input data themselves this function is called
RegionStruct* readFromTerminal(int* numOfRegions) {
    int regions, producers, organizations, foodtype;

    printf("\nPlease enter the number of regions: ");
    scanf(" %d", numOfRegions);

    // allocating memory for the region struct(s) based on the number of regions
    RegionStruct* newRegion = malloc(sizeof(RegionStruct) * *numOfRegions);

    for (int i = 0; i < *numOfRegions; ++i) {
        printf("\nPlease enter information for region %d: ", i + 1);
        printf("\nRegion name (less than 100 characters): ");
        scanf(" %s", newRegion[i].regionName);

        printf("\nHow many producers are there in the region?: ");
        scanf(" %d", &producers);

        newRegion[i].numOfProducers = producers;

        // allocating memory for excess, volatility, cost per unit and transport cost based on the number of producers
        newRegion[i].baseExcessPerOrg = malloc(sizeof(int) * producers);
        newRegion[i].excessVolatility = malloc(sizeof(double) * producers);
        newRegion[i].costPerUnit = malloc(sizeof(double) * producers);
        newRegion[i].transportCost = malloc(sizeof(double) * producers);

        printf("\nPlease enter excess food per organization (seperated by space): ");

        for (int j = 0; j < producers; ++j){
            scanf(" %d", &newRegion[i].baseExcessPerOrg[j]);
        }

        printf("\nPlease input food type for region:\n");
        printf("Type 1 for meat\n");
        printf("Type 2 for greens\n");
        printf("Type 3 for dairy\n");
        printf("Type 4 for seafood\n");
        printf("Type 5 for baked goods\n");
        printf("Type 6 for eggs\n");
        scanf(" %d", &foodtype);

        // switch to "translate" the user input to our enum values for food type
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

        printf("\nPlease enter volatility of excess as a multiplier per producer \n(0: the excess is constant  "
               "1: the excess fluctuates between double of base and zero): ");

        for (int j = 0; j < producers; ++j){
            scanf(" %lf", &newRegion[i].excessVolatility[j]);
        }

        printf("\nPlease enter cost per unit of the excess food per producer (seperated by space):");

        for (int j = 0; j < producers; ++j){
            scanf(" %lf", &newRegion[i].costPerUnit[j]);
        }

        printf("\nPlease enter transportation cost per km of the excess food per producer (seperated by space):");

        for (int j = 0; j < producers; ++j){
            scanf(" %lf", &newRegion[i].transportCost[j]);
        }

        printf("\nHow many organizations in the region?:");
        scanf(" %d", &organizations);

        // using the user input for number of organizations, allocating memory accordingly for demand and distance
        newRegion[i].numOfOrganizations = organizations;
        newRegion[i].demandPerOrg = malloc(sizeof(int) * organizations);
        newRegion[i].distanceToOrg = malloc(sizeof(int) * organizations);

        printf("\nPlease enter demand per organization (seperated by space):");

        for (int j = 0; j < organizations; ++j) {
            scanf(" %d", &newRegion[i].demandPerOrg[j]);
        }

        printf("\nPlease enter distance to each organization (seperated by space):");

        for (int j = 0; j < organizations; ++j) {
            scanf(" %lf", &newRegion[i].distanceToOrg[j]);
        }
    }
    return newRegion;
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


void saveFile(RegionResultStruct* results, int numberOfRegions) {

    FILE* regionsFile = fopen("Result.txt", "w");

    fprintf(regionsFile, "Region\t\tFood type\tFood saved\tFood wasted\tUnmet demand\tTotal cost\n");
    int i;
    for(i = 0; i < numberOfRegions; i++)
    {
        fprintf(regionsFile, "%8s\t", results[i].regionName);
        fprintf(regionsFile, "%10s\t\t", convertFoodtypeEnum(results[i].foodType));
        fprintf(regionsFile, "%6d\t\t", results[i].foodSaved);
        fprintf(regionsFile, "%6d\t\t  ", results[i].foodWasted);
        fprintf(regionsFile, "%6d\t\t", results[i].unmetDemand);
        fprintf(regionsFile, "%6.2lf\n", results[i].totalCost);
    }

    fclose(regionsFile);
}


void calculateIteration(RegionStruct* regions, RegionResultStruct* results, int numOfRegions, int numOfIterations) {

   int** supplyArray = malloc(sizeof (int*) * numOfRegions);
    int* metDemandSinceLastRestock = malloc(sizeof (int) * numOfRegions);


    for (int i = 0; i < numOfRegions; ++i) {
        results[i].regionName = regions[i].regionName;
        results[i].foodType = regions[i].foodType;
        results[i].foodSaved = 0;
        results[i].foodWasted = 0;
        results[i].unmetDemand = 0;

        int* dayToDaySupply;
        dayToDaySupply = malloc(sizeof (int) * regions[i].numOfProducers);

        supplyArray[i] = dayToDaySupply;
        for (int j = 0; j < regions[i].numOfProducers; ++j) {
            supplyArray[i][j] = regions[i].baseExcessPerOrg[j];
        }
    }

    //Here we simulate the given amount of days
    for (int i = 0; i < numOfIterations; ++i) {

        // repeat for each region
        for (int j = 0; j < numOfRegions; ++j) {

            if (i > 0 && i % regions[j].foodType == 0) {
                for (int k = 0; k < regions[j].numOfProducers; ++k) {
                    results[j].foodWasted += supplyArray[j][k];
                    supplyArray[j][k] = regions[j].baseExcessPerOrg[k];
                }
                //results[j].foodWasted = regions[j].baseExcessPerOrg - results[j].foodSaved;
                metDemandSinceLastRestock[j] = 0;
            }
            for (int k = 0; k < regions[j].numOfOrganizations; ++k) {

                //calculate cheapest "option" while demand is over 0
                int dayToDayDemand = regions[j].demandPerOrg[k];

                for (int h = 0; h < regions[j].numOfProducers; ++h) {

                    // an int to save combination/producer to distribute from
                    // this is used again later to update supply and demand numbers
                    int cheapestProducersIndex = 0;

                    // a double variable to find the cheapest combination
                    double lowestCost, newCost;

                    // calculating unit cost + transport cost per organization
                    lowestCost = (regions[j].costPerUnit[0] * (double)regions[j].demandPerOrg[k]) +
                    (regions[j].transportCost[0] * regions[j].distanceToOrg[k]);

                    // finding the producer that can meet (some of) the demand at lowest costs
                    for (int l = 0; l < regions[j].numOfProducers; ++l) {

                        if (supplyArray[j][l] > 0) {

                            if (supplyArray[j][l] >= dayToDayDemand) {
                                newCost = (regions[j].costPerUnit[l] * (double)regions[j].demandPerOrg[k]) +
                                          (regions[j].transportCost[l] * regions[j].distanceToOrg[k]);

                                if (newCost < lowestCost) {
                                    lowestCost = newCost;
                                    cheapestProducersIndex = l;
                                }
                            }
                            else {
                                newCost = (regions[j].costPerUnit[l] * (double)supplyArray[j][l]) +
                                          (regions[j].transportCost[l] * regions[j].distanceToOrg[k]);

                                if (newCost < lowestCost) {
                                    lowestCost = newCost;
                                    cheapestProducersIndex = l;
                                }
                            }
                        } else {
                            break;
                        }
                    }
                    //we save the price and food saved from cheapest producer, and update daytodaysupply and -demand
                    if(supplyArray[j][cheapestProducersIndex] >= dayToDayDemand) {

                        results[j].foodSaved += dayToDayDemand;
                        metDemandSinceLastRestock[j] += dayToDayDemand;

                        results[j].totalCost += (regions[j].costPerUnit[cheapestProducersIndex] * (double)regions[j].demandPerOrg[k]) +
                                                (regions[j].transportCost[cheapestProducersIndex] * regions[j].distanceToOrg[k]);

                        supplyArray[j][cheapestProducersIndex] -= dayToDayDemand;
                        dayToDayDemand -= dayToDayDemand;
                        break;
                    } else {

                        results[j].foodSaved += supplyArray[j][cheapestProducersIndex];
                        metDemandSinceLastRestock[j] += supplyArray[j][cheapestProducersIndex];

                        results[j].totalCost += (regions[j].costPerUnit[cheapestProducersIndex] * (double)supplyArray[j][cheapestProducersIndex]) +
                                                (regions[j].transportCost[cheapestProducersIndex] * regions[j].distanceToOrg[k]);

                        int temp = dayToDayDemand;
                        dayToDayDemand -= supplyArray[j][cheapestProducersIndex];
                        supplyArray[j][cheapestProducersIndex] -= supplyArray[j][cheapestProducersIndex];
                    }
                }
                //we add excess demand to the unmetdemand result
                results[j].unmetDemand += dayToDayDemand;
            }

            if (i == numOfIterations - 1) {
                for (int u = 0; u < regions[j].numOfProducers; ++u) {
                    results[j].foodWasted += regions[j].baseExcessPerOrg[u];
                }
                results[j].foodWasted -= metDemandSinceLastRestock[j];
            }
        }
    }

   for (int i = 0; i < numOfRegions; ++i) {
        free(supplyArray[i]);
    }
    free(supplyArray);
}


void outputResult(RegionResultStruct* results, int numberOfRegions) {
    int i;

    printf("\nRegion\t\t Food type\tFood saved\tFood wasted\t  Unmet demand\t\tTotal cost\n");

    for(i = 0; i < numberOfRegions; i++)
    {
        printf("%8s\t", results[i].regionName);
        printf("%10s\t", convertFoodtypeEnum(results[i].foodType));
        printf("%6d\t\t", results[i].foodSaved);
        printf("%6d\t\t  ", results[i].foodWasted);
        printf("%6d\t\t", results[i].unmetDemand);
        printf("%6.2lf\n", results[i].totalCost);
    }

}


int compareResults(const void* element1, const void* element2) {

    RegionResultStruct* firstElement = (RegionResultStruct*)element1;
    RegionResultStruct* secondElement = (RegionResultStruct*)element2;

    if (firstElement->foodSaved < secondElement->foodSaved) {
        return 1;
    } else if (firstElement->foodSaved > secondElement->foodSaved) {
        return -1;
    } else {
        return 0;
    }
}


int main() {

    char input;
    int numberOfRegions, numOfIterations;
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

    printf("please input number of days to simulate: ");
    scanf(" %d", &numOfIterations);

    printf("%s, %d, %d, %d, %d, %d, %.2lf, %.2lf, %.2lf\n", regions[0].regionName, regions[0].foodType, regions[0].numOfProducers,
           regions[0].baseExcessPerOrg[0], regions[0].numOfOrganizations, regions[0].demandPerOrg[0],
           regions[0].excessVolatility[0], regions[0].costPerUnit[0], regions[0].transportCost[0]);

    calculateIteration(regions, results, numberOfRegions, numOfIterations);

    qsort(results, numberOfRegions, sizeof (RegionResultStruct),compareResults);

    outputResult(results, numberOfRegions);

    printf("Would you like to save result as a file? (y/n)");
    scanf(" %c", &input);

    if (input == 'y') {
        saveFile(results, numberOfRegions);
        printf("file saved as Result.txt");
    }

    free(regions);
    free(results);

    return 0;
}