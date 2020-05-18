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
vector<vector<float>> distancesCopy;
vector<float> predictions(220);
vector<vector<float>> distancesTesting(1100);
vector<float> predictionsTesting(218);

int OVERALL_K = 20;
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
    for(int i = 0; i < pointCount; i++)
    {
        dataPoints[i] = vector<float>(7);
        fscanf(inputFile, "%f,%f,%f,%f,%f,%f,%f", &dataPoints[i][0], &dataPoints[i][1], &dataPoints[i][2], &dataPoints[i][3],&dataPoints[i][4], &dataPoints[i][5], &dataPoints[i][6]);
    }
    for(int i = 0; i < 220; i++) {
      supervisedTesting[i] = vector<float>(7);
    }
    for(int i = 0; i < 880; i++) {
      supervisedTraining[i] = vector<float>(7);
      distances[i] = vector<float>(2);
      distances[i][1] = i;
    }
    for(int i = 0; i < 1100; i++) {
      distancesTesting[i] = vector<float>(2);
      distancesTesting[i][1] = i;
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
      if (dataPoints[i][6] > 0) {
        dataPoints[i][6] = 1;
      }
    }
    for(int i = 0; i < 6; i++){
      normalizeData(i);
    }
}

void splitdata(int divide)
{
  int trainCount = 0;
  int testCount = 0;
  for (int i = 0; i < TRAIN_SIZE; i++) {
    if (i >= (divide * 220) && i < ((divide + 1) * 220)) {
      for (int j = 0; j < 7; j++) {
          supervisedTesting[testCount][j] = dataPoints[i][j];
      }
      testCount++;
    }else {
      for (int j = 0; j < 7; j++) {
          supervisedTraining[trainCount][j] = dataPoints[i][j];
      }
      trainCount++;
    }
  }
}

void distanceCalc()
{
     for(int i = 0; i < 220; i++) {
      int rain = 0, noRain = 0;
      for(int j = 0; j < 880; j++) {
        distances[j][0] = sqrt(pow((supervisedTraining[j][0] - supervisedTesting[i][0]), 2) + pow((supervisedTraining[j][1] - supervisedTesting[i][1]), 2) + pow((supervisedTraining[j][2] - supervisedTesting[i][2]), 2) + pow((supervisedTraining[j][3] - supervisedTesting[i][3]), 2) + pow((supervisedTraining[j][4] - supervisedTesting[i][4]), 2) + pow((supervisedTraining[j][5] - supervisedTesting[i][5]), 2));
      }
      distancesCopy = distances;
      sort(distancesCopy.begin(),distancesCopy.end());
      for (int i = 0; i < K; i++) {
        if (dataPoints[distancesCopy[i][1]][6] == 1) {
          rain++;
        }else {
          noRain++;
        }
      }
      if (rain > noRain) {
        predictions[i] = 1;
      }else {
        predictions[i] = 0;
      }
    }
}

void distanceCalcTesting(int bestK)
{
     vector<vector<float>> distancesCopy;

     for(int i = 0; i < 218; i++) {
      int rain = 0, noRain = 0;
      for(int j = 0; j < 1100; j++) {
        distancesTesting[j][0] = sqrt(pow((dataPoints[j][0] - dataPoints[i + 1100][0]), 2) + pow((dataPoints[j][1] - dataPoints[i + 1100][1]), 2) + pow((dataPoints[j][2] - dataPoints[i + 1100][2]), 2) + pow((dataPoints[j][3] - dataPoints[i + 1100][3]), 2) + pow((dataPoints[j][4] - dataPoints[i + 1100][4]), 2) + pow((dataPoints[j][5] - dataPoints[i + 1100][5]), 2));
      }
      distancesCopy = distancesTesting;
      sort(distancesCopy.begin(),distancesCopy.end());
      for (int i = 0; i < bestK; i++) {
        if (dataPoints[distancesCopy[i][1]][6] == 1) {
          rain++;
        }else {
          noRain++;
        }
      }
      if (rain > noRain) {
        predictionsTesting[i] = 1;
      }else {
        predictionsTesting[i] = 0;
      }
     }
}


void accuracy(float *foldAcc, int foldCount)
{
  float correct = 0;
  for (int i = 0; i < 220; i++) {
      if (predictions[i] == supervisedTesting[i][6]) {
        correct++;
      }
  }
  foldAcc[foldCount] = (correct / 220);
}

float accuracyOfTesting() {
  float correctPred = 0.0;
  float predictionAcc;
  for (int i = 0; i < 218; i++) {
      if (predictionsTesting[i] == dataPoints[i+1100][6]){
        correctPred++;
      }
  }
  predictionAcc = (correctPred / 218);
  return predictionAcc;
}

float averageFold(float foldAcc[]) {
  float avg = 0;
  for(int i = 0; i < 5; i++) {
    avg += foldAcc[i];
  }
  avg = (avg / 5);
  //cout << "avg" << avg;
  return avg;
}

int bestKValue(float *kAvg)
{
  int bestK = -1;
  float avg = 0;
  for (int i = 1; i < OVERALL_K;i += 2) {
    if (kAvg[i] > avg) {
      bestK = i;
      avg = kAvg[i];
      cout << "best average " << kAvg[i] << " ";
      cout << "best k so far" << bestK << endl;
    }
  }
  return bestK;
}

int main(int argc, char **argv){
    if(argc != 2) {
        cout << "usage: program <dataFile>" << endl;
        return 0;
    }
    float foldAcc[5] = {0};
    fileName = argv[1];
    //reading in input
    readInput();
    //normalizing the data and preprocess
    preprocessing();
    //set this equal to first for loop from below equal to total K
    float kAvg[OVERALL_K];
    int bestK = 0;
    float _2017Acc = 0;
    for (int i = 1; i < OVERALL_K; i+= 2) {
      K = i;
      //this is for each fold
      for (int j = 0; j < 5; j++) {
        splitdata(j);
        distanceCalc();
        accuracy(foldAcc, j);
      }
      kAvg[K] = averageFold(foldAcc);
    }
    bestK = bestKValue(kAvg);
    cout << bestK;
    distanceCalcTesting(bestK);
    _2017Acc = accuracyOfTesting();
    
    cout << "our prediction Accuracy for the year of 2017 is: " << _2017Acc;
    return 0;
  }
