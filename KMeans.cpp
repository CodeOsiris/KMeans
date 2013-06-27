#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <ctime>
#include <stdlib.h>
#include <iomanip>
using namespace std;

#define N 30
#define K 3
#define NUM_OF_ATTR 2
#define ZERO 0.01

class Element{
public:
    double attr[NUM_OF_ATTR];
};

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
    for (int i = 1;i < K;i++)
        if ((tmp = cal_dist(median[i], e)) < dist)
        {
            dist = tmp;
            index = i;
        }
    return index;
}

//计算质心
Element get_median(vector<Element> cluster){
    double attr[NUM_OF_ATTR];
    int nc = cluster.size();
    Element tmp;
    for (int i = 0;i < nc;i++)
    {
        for (int j = 0;j < NUM_OF_ATTR;j++)
            attr[j] += cluster[i].attr[j];
    }
    for (int i = 0;i < NUM_OF_ATTR;i++)
        tmp.attr[i] = attr[i] / nc;
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
    cout << "KMeans demo.\nAttributions: 2\nElements: 30\nClusters: 3\n";
    srand(time(0));
    vector<Element> elements;
    vector<Element> cluster[K];
    Element median[K];
    ifstream fin("data.txt");
    double tmp;
    for (int i = 0;i < N;i++){
        Element tmpe;
        for (int j = 0;j < NUM_OF_ATTR;j++)
            fin >> tmpe.attr[j];
        elements.push_back(tmpe);
    }
    fin.close();
    //使用KMeans++进行初始点选择
    median[0] = elements[0];
    for (int i = 1;i < K;i++){
        double d[N];
        double sum = 0;
        for (int j = 0;j < N;j++){
            d[j] = cal_dist(median[i - 1], elements[j]);
            sum += d[j];
        }
        double random = (double)(rand() % ((int)ceil(sum) * 1000)) / 1000;
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
    for (int i = 0;i < K;i++){
        cout << "Cluster " << i << endl;
        for (int j = 0;j < cluster[i].size();j++){
            for (int k = 0;k < NUM_OF_ATTR;k++)
                cout << fixed << setprecision(1) << cluster[i][j].attr[k] << " ";
            cout << endl;
        }
    }
    return 0;
}
