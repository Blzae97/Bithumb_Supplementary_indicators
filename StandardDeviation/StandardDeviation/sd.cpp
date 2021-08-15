#include <iostream>
#include "json/json.h"
#include <fstream>
#include <vector>
#include <math.h>

#pragma warning(disable: 4996);

using namespace std;

double* STD(double* data, int loop, int size) {
    double* avgsC = new double[size];
    double* varianceC = new double[size];
    double* stdC = new double[size];

    fill_n(avgsC, size, 0);
    fill_n(stdC, size, 0);
    fill_n(varianceC, size, 0);

    for (int i = loop - 1; i < size; i++) {
        double sum = 0;
        for (int j = i; j > i - loop; j--) {
            sum += data[j];
        }
        avgsC[i] = sum / loop;
    }

    for (int i = loop - 1; i < size; i++) {
        double sum = 0;
        for (int j = i; j > i - loop; j--) {
            sum += pow(data[j] - avgsC[i], 2);
        }
        varianceC[i] = sum / (loop);
    }

    for (int i = loop - 1; i < size; i++) {
        stdC[i] = sqrt(varianceC[i]);
    }

    return stdC;
}

int main() {
    double* std;
    int length = 20;
    double* c;
    int size = 0;

    Json::Value root;
    Json::Reader reader;
    ifstream json("test.json", ifstream::binary);
    reader.parse(json, root);   //json >> root; 두 구문 모두 json을 파싱하는데 사용할 수 있다.

    size = root["data"].size(); // API로 불러온 JSON 데이터 사이즈 구하기

    c = new double[size];
    fill_n(c, size, 0);

    for (int i = 0; i < size; i++) {
        double cc;
        stringstream convert1(root["data"][i][2].asString());
        convert1 >> cc;
        c[i] = cc;
    }

    std = STD(c, length, size);

    for (int i = 0; i < size; i++) {
        cout << fixed;
        cout << "STD[" << i << "]" << std[i] << endl;

    }
}
