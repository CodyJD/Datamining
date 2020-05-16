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
vector<int> pointID;
float epsilon = 0.000001;

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
    pointID.resize(pointCount);
    rewind(inputFile);
    for(int i = 0; i < pointCount; i++)
    {
        dataPoints[i] = vector<float>(4);
        fscanf(inputFile, "%f,%f,%f,%f", &dataPoints[i][0], &dataPoints[i][1], &dataPoints[i][2], &dataPoints[i][3]);
    }
    fclose(inputFile);
}

void closestCluster(vector<float> d, vector<vector<float>> clusters, int currPos)
{
    float min = 1000, tmp;
    for (int i = 0; i < clusters.size(); ++i) {
          tmp = sqrt(pow((d[0]-clusters[i][0]),2) + pow((d[1]-clusters[i][1]),2) + pow((d[2]-clusters[i][2]),2) + pow((d[3]-clusters[i][3]),2) );
          if (tmp < min){
            min = tmp;
            pointID[currPos] = i + 1;
          }
    }
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

int main(int argc, char **argv){
    if(argc != 3) {
        cout << "usage: program <dataFile> k-value" << endl;
        return 0;
    }
    fileName = argv[1];
    int K = atoi(argv[2]);
    srand(time(0));
    //Initialize clusters
    vector<vector<float>> clusters(K);
    vector<vector<float>> prevClusters(K);
    for(int i = 0; i < K; i++){
      clusters[i] = vector<float>(4);
      prevClusters[i] = vector<float>(4);
      for(int j = 0; j < clusters[i].size(); j++){
        clusters[i][j] = (float)(rand() % 10)/10;
        cout << clusters[i][j];
        cout << " ";
      }
      cout << endl;
    }
    //reading in input
    readInput();
    for(int i = 0; i < 4; i++){
      normalizeData(i);
    }
    bool equal = false;
    int pastFirst = 0;
    //iterate until convergence of mean
    //while(!equal) {
    for (int i = 0; i < 10; i++) {
      //calculate distance
      for (int i = 0; i < dataPoints.size(); ++i) {
          closestCluster(dataPoints[i], clusters, i);
          cout << pointID[i];
      }
      cout << endl;

      //find mean of clusters
      //find number points of each cluster
      //knockout
      vector<int> count(K,0);
      vector<vector<float>> sum(K);
      for (int i = 0; i < sum.size(); i++) sum[i]=vector<float>(4,0);
      for (int i = 0; i < pointID.size(); i++) {
        count[(pointID[i] - 1)]++;
        sum[pointID[i] - 1][0] += dataPoints[i][0];
        sum[pointID[i] - 1][1] += dataPoints[i][1];
        sum[pointID[i] - 1][2] += dataPoints[i][2];
        sum[pointID[i] - 1][3] += dataPoints[i][3];
      }
      for (int i = 0; i < sum.size(); i++) {
        for (int j = 0; j < 4; j++) {
          prevClusters[i][j] = clusters[i][j];
          clusters[i][j] = sum[i][j]/(count[i] + epsilon);
        }
      }

      for (int i = 0; i < sum.size(); i++) {
        cout << clusters[i][0] << " ";
        cout << clusters[i][1] << " ";
        cout << clusters[i][2] << " ";
        cout << clusters[i][3] << " ";
        cout << endl;
      }
  }
  FILE *output = fopen("log.txt", "w+");
  for(int i = 0; i < dataPoints.size(); i++){
      fprintf(output, "%d", pointID[i]);
  }
/*
    for(int i = 0; i < dataPoints.size(); i++){
          for(int j = 0; j < dataPoints[i].size(); j++)
          {
              cout << dataPoints[i][j] << " ";
          }
          cout << endl;
    }
*/
    return 0;
  }
