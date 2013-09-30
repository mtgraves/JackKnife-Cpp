#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "jkTools.h"

using namespace std;

int main(){

    /* Instantiate jkTools */
    JackKnifeTools jk;
 
    /* check if data file is empty before writing */
    ifstream filestr("JackknifeData.dat");
    if(!jk.IsEmpty(filestr)){
        cout<<"\nError: Data file is not empty!\n"<<endl;
        return 0;
    }

    /* open file to write analyzed data to disk */
    FILE *file;
    file = fopen("JackknifeData.dat", "a+");
    fprintf(file,"#%9s\t%10s\t%10s\n", "T", "Cv", "CvErr");
    
    /* vector of estimator file names */
    vector<string> estFiles = jk.Glob("*estimator*");

    /* loop over all estimator files */
    for (int i = 0; i<estFiles.size(); i++){
        cout<<estFiles[i]<<endl;
        string temp = estFiles[i].substr(13,6);

        /* read in file */
        ifstream inFile;
        inFile.open(estFiles[i].c_str());

        /* grab whitespace delimited data */
        vec_dVec allData = jk.ReadData(inFile);

        /* bookkeeping */
        const int numBins = allData.size();

        /* store specific heat data */
        dVec Cv1(numBins), Cv2(numBins), Cv3(numBins);
        for (int j=0; j<numBins; j++){
            Cv1[j] = allData[j][11];
            Cv2[j] = allData[j][12];
            Cv3[j] = allData[j][13];
        }

        /* delete all unused data from memory */
        vector<dVec>().swap(allData);

        /* perform jackknife averaging */
        dVec jkResults;
        jk.JkAnalysis(Cv1, Cv2, Cv3, jkResults);

        cout<<"est = "<<jkResults[0]<<" +/- "<<jkResults[1]<<endl;

        /* Write results to disk */
        fprintf(file,"%10s\t%10f\t%10f\n", temp.c_str(), jkResults[0], jkResults[1]);
    }

    fclose(file);
    return 0;
}
