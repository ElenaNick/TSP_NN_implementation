/******************************************************************************
 * Author: Elena Greblova
 * Class: CS 325
 * Assignment: Final Project
 * Due date: December 1st?, 2017
 * Description: TSP problem modified algorithm implementation. Accepts a file
 *              with towns coordinates in the command line.
 *              Reads the city info into array of City structs pointers, run
 *              them through algorithm, and writes the calculated distance and
 *              path into the new file.
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

struct City{
	int identifier;
	int x;
	int y;
	int visited;
};

int read_cities(FILE* input, struct City** set, int capacity);
int find_closest(struct City** set, struct City* current, int numOfCities);

int main(int argc, char* argv[]){
	int numOfCities = 0;
	int i, closest;
	long int distance = 0;
	int* path;
	FILE* input, output;
	struct City* current;

	struct City** citySet = malloc(1024*sizeof(struct City*));
//check usage
	if(argc < 2){
		printf("USAGE: <executable name> <input file name>\n");
		exit(0);
	}

//open the file and read cities into the array of structs
	input = fopen(argv[1], "r");
	if(input == NULL){
		printf("ERROR: couln't open the file %s\n", argv[1]);
		exit(1);
	}

	numOfCities = read_cities(input, citySet, 1024);


/****
 * Nearest Neighbour Algorithm
 * ***/
	//allocate memory to store the path 
	path = malloc(numOfCities*sizeof(int));

	//pick up the start of the route and add it to the path
	current = citySet[0];
	path[0] = 0;
	current->visited = 1;

	//for each city in the set
	for(i = 1; i < numOfCities; i++){
		//find the index of the closest city
		closest = find_closest(citySet, current, numOfCities);
		//mark the city as visited
		citySet[closest]->visited = 1;
		//add distance from current to closest to the path distance
		distance = distance + lround(sqrt(pow((current->x - citySet[closest]->x),2) + pow((current->y - citySet[closest]->y),2)));
		//set current to the closest city
		current = citySet[closest];
		//ad add it to the path
		path[i] = closest;

	}

	printf("the path length is: %ld\n", distance);

	
	
	//clean up
	fclose(input);
	for(i = 0; i < numOfCities; i++)
		free(citySet[i]);
	free(citySet);
	free(path);






	return 0;
}

/********************************************************************************
 *                         read_city()
 *Input: FILE* input - file to read from
 *       struct City** set - dynamic array to store cities at
 *       capacity - original capacity of the array
 *Ouput: cities are saved in set. returns number of cities read from the file
 *******************************************************************************/
int read_cities(FILE* input, struct City** set, int capacity){
	char* readin = malloc(1024*sizeof(char));
	int numOfCities = 0;
	int i;
	printf("In the read function, number is %d, capacity is %d\n", numOfCities, capacity);

	//read cities from the file one by one 
	while(fgets(readin, 1023, input) != NULL){
		//if the array is full, create a new array of double capacity and
		//copy old array into it
		if(numOfCities == capacity){
			printf("time to double the array: number is %d, capacity is %d\n", numOfCities, capacity);

			struct City** temp = malloc(2*capacity*(sizeof(struct City*)));
			//printf("allocated temp
			for(i = 0; i < numOfCities; i++)
				temp[i] = set[i];
			free(set);
			set = temp;
			capacity *=2;
		}
		//allocate memory for new struct City and assing new info to its members
		struct City* new = malloc(sizeof(struct City));
		new->identifier = atoi(strtok(readin, " "));
		new->x = atoi(strtok(NULL, " "));
		new->y = atoi(strtok(NULL, " "));
		new->visited = 0;
		//add the new city to the array set
		set[numOfCities] = new;
		numOfCities++;
	}

	free(readin);
	return numOfCities;

}


/********************************************************************************
 *                     find_closest()
 *Input: struct City** set - the set of the cities to look through
 *       struct City* current - the city to look the closest for
 *       int numOfCities - total number of the cities in the set
 *Output: index of the city which is closest to the curtrent
 *******************************************************************************/
int find_closest(struct City** set, struct City* current, int numOfCities){
	int min, i;
	min = -1;
	long int distance = INT_MAX;

	//for each unvisited city calculate the distance and compare with current minimum
	for(i = 0; i < numOfCities; i++){
		if(set[i]->visited == 0){
			int d_cur = lround(sqrt(pow((current->x - set[i]->x), 2) + pow((current->y - set[i]->y), 2)));
			if(d_cur < distance){
				distance = d_cur;
				min = i;
			}
		}
	}
	return min;
}
