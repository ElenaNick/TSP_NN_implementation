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
int find_closest(struct City* set, struct City* current, int numOfCities);

int main(int argc, char* argv[]){
	int numOfCities = 0;
	int i, j, closest;
	long int distance = 0;
	long int min_distance = INT_MAX;
	int *path, *min_path;
	FILE* input, *output;
	struct City* current;

	struct City* citySet = malloc(1024*sizeof(struct City));
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

	numOfCities = read_cities(input, &citySet, 1024);

/****
 * Nearest Neighbour Algorithm
 * ***/
	//allocate memory to store the path 
	path = malloc(numOfCities*sizeof(int));
	min_path = malloc(numOfCities*sizeof(int));
	for(i = 0; i < numOfCities; i++)
		min_path[i] = 0;

	
	//check the path for each city in the set
	for(j = 0; j < numOfCities; j++){
		//pick up the start of the route and add it to the path
		current = &citySet[j];
		path[0] = j;
		current->visited = 1;

		//for each city in the set
		for(i = 1; i < numOfCities; i++){
			//find the index of the closest city
			closest = find_closest(citySet, current, numOfCities);
			//mark the city as visited
			citySet[closest].visited = 1;
			//add distance from current to closest to the path distance
			distance = distance + lrint(sqrt(pow((current->x - citySet[closest].x),2) + pow((current->y - citySet[closest].y),2)));
			//set current to the closest city
			current = &citySet[closest];
			//ad add it to the path
			path[i] = closest;
		}
		//add the last edge to the distance
		distance = distance + lrint(sqrt(pow((citySet[path[numOfCities - 1]].x - citySet[j].x),2) + pow((citySet[path[numOfCities-1]].y - citySet[j].y),2)));

		if(distance < min_distance){
			for(i = 0; i < numOfCities; i++)
				min_path[i] = path[i];
			min_distance = distance;
		}

		printf("%d\n", j);
		int x;
		for(x = 0; x < numOfCities; x++)
			citySet[x].visited = 0;
		distance = 0;
	}


/*****
 * end of the algo
 * ****/

	//make up the output file name
	char* newName = malloc(strlen(argv[1]) + 7); 
	strcpy(newName, argv[1]);
	strcat(newName, ".tour");
	
	//open the output file and write results into it
	output = fopen(newName, "w");
	if (output == NULL) {
		printf("ERROR: couln't open the file to output.\n");
		exit(1);
	}
	fprintf(output, "%ld\n", min_distance);
	
	for (i = 0; i < numOfCities; i++)
	{
		fprintf(output, "%d\n", citySet[min_path[i]].identifier); 
	}


	
	
	//clean up
	fclose(input);
	fclose(output);
	free(citySet);
	free(path);
	free(min_path);
	free(newName);
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

	//read cities from the file one by one 
	while(fgets(readin, 1023, input) != NULL){
		//if the array is full, create a new array of double capacity and
		//copy old array into it
		if(numOfCities == capacity){
			capacity = capacity * 2;
			struct City* temp = malloc(capacity*(sizeof(struct City)));
			for(i = 0; i < numOfCities; i++)
				temp[i] = (*set)[i];
			free(*set);
			*set = temp;
		}
		//allocate memory for new struct City and assing new info to its members
		(*set)[numOfCities].identifier = atoi(strtok(readin, " "));
		(*set)[numOfCities].x = atoi(strtok(NULL, " "));
		(*set)[numOfCities].y = atoi(strtok(NULL, " "));
		(*set)[numOfCities].visited = 0;
		//add the new city to the array set
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
int find_closest(struct City* set, struct City* current, int numOfCities){
	int min, i;
	min = -1;
	long int distance = INT_MAX;

	//for each unvisited city calculate the distance and compare with current minimum
	for(i = 0; i < numOfCities; i++){
		if(set[i].visited == 0){
			int d_cur = lround(sqrt(pow((current->x - set[i].x), 2) + pow((current->y - set[i].y), 2)));
			if(d_cur < distance){
				distance = d_cur;
				min = i;
			}
		}
	}
	return min;
}


