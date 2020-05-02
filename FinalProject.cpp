#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <time.h>
using namespace std;

vector<vector<float>> dataPoints;
const char *fileName = NULL;
int pointCount;

void readInput()
{
    FILE *inputFile = fopen(fileName, "r");
    int ch;
    pointCount = 0;

    do
    {
        ch = fgetc(inputFile);
        if(ch == '\n') pointCount++;
    }
    while(ch != EOF);

    dataPoints.resize(pointCount);
    rewind(inputFile);
    for(int i = 0; i < pointCount; i++)
    {
        dataPoints[i] = vector<float>(4);
        fscanf(inputFile, "%f,%f,%f,%f", &dataPoints[i][0], &dataPoints[i][1], &dataPoints[i][2], &dataPoints[i][3]);
    }
    fclose(inputFile);
}

int main(int argc, char **argv){
    if(argc != 2) {
        cout << "usage: program <dataFile>" << endl;
        return 0;
    }
    fileName = argv[1];
    
    //reading in input
    readInput();
 
    return 0;
  }