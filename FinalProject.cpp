#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <algorithm> //for sorting algo
using namespace std;

vector<vector<float>> dataPoints;
vector<vector<float>> supervisedTesting(220);
vector<vector<float>> supervisedTraining(880);
vector<vector<float>> distances(880);

const char *fileName = NULL;
int pointCount;
int K;
#define TRAIN_SIZE 1100

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
    cout << pointCount;
    for(int i = 0; i < pointCount; i++)
    {

        dataPoints[i] = vector<float>(8);
        fscanf(inputFile, "%f,%f,%f,%f,%f,%f,%f,%f", &dataPoints[i][0], &dataPoints[i][1], &dataPoints[i][2], &dataPoints[i][3],&dataPoints[i][4], &dataPoints[i][5], &dataPoints[i][6], &dataPoints[i][7]);
    }
    for(int i = 0; i < 220; i++) {
      supervisedTesting[i] = vector<float>(8);
    }
    for(int i = 0; i < 880; i++) {
      supervisedTraining[i] = vector<float>(8);
      distances[i] = vector<float>(2);
      distances[i][1] = i;
    }
    fclose(inputFile);
}

void normalizeData(int attribute)
{
    float min = 100, max = 0;
    for(int i = 0; i < dataPoints.size(); i++)
    {
        if(min > dataPoints[i][attribute])
            min = dataPoints[i][attribute];
        if(max < dataPoints[i][attribute])
            max = dataPoints[i][attribute];
    }

    for(int i = 0; i < dataPoints.size(); i++)
    {
        dataPoints[i][attribute] = (dataPoints[i][attribute]-min)/(max-min);
    }
}

void preprocessing () {

    for (int i = 0; i < dataPoints.size(); i++) {
      if (dataPoints[i][7] > 0) {
        dataPoints[i][7] = 1;
      }
    }
    for(int i = 0; i < 7; i++){
      normalizeData(i);
    }
}

void splitdata(int divide) {
  int trainCount = 0;
  int testCount = 0;
  for (int i = 0; i < TRAIN_SIZE; i++) {
    if (i >= (divide * 220) && i < ((divide + 1) * 220)) {
      //copy data points to testing
      for (int j = 0; j < 8; j++) {
          supervisedTesting[testCount][j] = dataPoints[i][j];
      }
      testCount++;
      //cout << "testing" << i << endl;
    }else {
      for (int j = 0; j < 8; j++) {
          supervisedTraining[trainCount][j] = dataPoints[i][j];
      }
      trainCount++;
      //cout << "training" << i << endl;
    }
  }
}

void distanceCalc() {
    for(int i = 0; i < 220; i++) {
      for(int j = 0; j < 880; j++) {
        //printf("%d: %f, %f\n",i , myTrainOne[i], myTrainTwo[i]);
        distances[j][0] = sqrt(pow((supervisedTraining[j][0] - supervisedTesting[i][0]), 2) + pow((supervisedTraining[j][1] - supervisedTesting[i][1]), 2) + pow((supervisedTraining[j][2] - supervisedTesting[i][2]), 2) + pow((supervisedTraining[j][3] - supervisedTesting[i][3]), 2) + pow((supervisedTraining[j][4] - supervisedTesting[i][4]), 2) + pow((supervisedTraining[j][5] - supervisedTesting[i][5]), 2) + pow((supervisedTraining[j][6] - supervisedTesting[i][6]), 2) + pow((supervisedTraining[j][7] - supervisedTesting[i][7]), 2));
        //printf("%d: %f\n", i, dist[i]);
        cout << "calculation: " << distances[j][0] << "original Inedx: " << distances[j][1] << endl;
      }
      bool boo = 0;
      sort(distances.begin(),distances.end());
    }
}

int main(int argc, char **argv){
    if(argc != 2) {
        cout << "usage: program <dataFile>" << endl;
        return 0;
    }
    fileName = argv[1];
    //reading in input
    readInput();
    //normalizing the data and preprocess
    preprocessing();

    for (int i = 1; i < 100; i+= 2) {
      K = i;
      //this is for each fold
      for (int j = 0; j < 5; j++) {
        splitdata(j);
        distanceCalc();
      }
    }


    return 0;
  }
