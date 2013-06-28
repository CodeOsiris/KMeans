#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <ctime>
#include <stdlib.h>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;

#define N 150
#define K 3
#define NUM_OF_ATTR 4
#define ZERO 0.00001

class Element{
public:
    double attr[NUM_OF_ATTR];
    string label;
};

double convert_to_double(string s){
    stringstream ss(s);
    double result;
    ss >> result;
    return result;
}

//计算元素距离
double cal_dist(Element e1, Element e2){
    double sum = 0;
    for (int i = 0;i < NUM_OF_ATTR;i++){
        sum += (e1.attr[i] - e2.attr[i]) * (e1.attr[i] - e2.attr[i]);
    }
    return sqrt(sum);
}

//选择聚类
int choose_cluster(Element median[], Element e){
    double dist = cal_dist(median[0], e);
    double tmp = dist;
    int index = 0;
    for (int i = 1;i < K;i++){
        if ((tmp = cal_dist(median[i], e)) < dist){
            dist = tmp;
            index = i;
        }
    }
    return index;
}

//计算质心
Element get_median(vector<Element> cluster){
    double attr[NUM_OF_ATTR];
    for (int i = 0;i < NUM_OF_ATTR;i++)
        attr[i] = 0;
    int nc = cluster.size();
    Element tmp;
    for (int i = 0;i < nc;i++)
        for (int j = 0;j < NUM_OF_ATTR;j++)
            attr[j] += cluster[i].attr[j];
    for (int i = 0;i < NUM_OF_ATTR;i++)
        tmp.attr[i] = attr[i] / nc;
    tmp.label = "Median";
    return tmp;
}

//计算方差
double cal_square(vector<Element> cluster[], Element median[]){
    double square = 0;
    for (int i = 0;i < K;i++)
        for (int j = 0;j < cluster[i].size();j++)
            square += cal_dist(cluster[i][j], median[i]);
    return square;
}

int main(){
    cout << "KMeans demo.\nData set: Iris\nAttributions: 4\nElements: 150\nClusters: 3\n";
    srand(time(0));
    vector<Element> elements;
    vector<Element> cluster[K];
    Element median[K];
    ifstream fin("data_iris.txt");
    double tmp;
    int count = 0;
    for (int i = 0;i < N;i++){
        Element tmpe;
        string tmps;
        for (int j = 0;j < NUM_OF_ATTR;j++){
            getline(fin,tmps,',');
            tmpe.attr[j] = convert_to_double(tmps);
        }
        getline(fin,tmps);
        tmpe.label = tmps;
        elements.push_back(tmpe);
    }
    fin.close();
    for (int t = 1;t <= 5;t++){
    //使用KMeans++进行初始点选择
    median[0] = elements[0];
    for (int i = 1;i < K;i++){
        double d[N];
        double sum = 0;
        for (int j = 0;j < N;j++){
            d[j] = cal_dist(median[i - 1], elements[j]);
            sum += d[j];
        }
        double random = (double)(rand() % ((int)ceil(sum) * 1000000)) / 1000000;
        for (int j = 0;j < N;j++){
            random -= d[j];
            if (random <= 0){
                median[i] = elements[j];
                break;
            }
        }
    }
    int index = 0;
    for (int i = 0;i < elements.size();i++){
        index = choose_cluster(median, elements[i]);
        cluster[index].push_back(elements[i]);
    }
    double square = -1, newsquare = cal_square(cluster,median);
    string a;
    while (abs(newsquare - square) >= ZERO){
        square = newsquare;
        for (int i = 0;i < K;i++)
            median[i] = get_median(cluster[i]);
        newsquare = cal_square(cluster,median);
        for (int i = 0;i < K;i++)
            cluster[i].clear();
        for (int i = 0;i < elements.size();i++){
            index = choose_cluster(median, elements[i]);
            cluster[index].push_back(elements[i]);
        }
    }
    ofstream fout("output" + to_string(t) + ".txt");
    for (int i = 0;i < K;i++){
        fout << "Cluster " << i << endl;
        for (int j = 0;j < cluster[i].size();j++){
            fout << cluster[i][j].label << endl;
        }
    }
    }
    return 0;
}
