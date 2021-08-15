#include <iostream>
#include "json/json.h"
#include <fstream>
#include <vector>
#include <math.h>

#pragma warning(disable: 4996);

using namespace std;

double* SMA(double* data,int loop ,int size) {
    double* sumSMA = new double[size];
    fill_n(sumSMA, size, 0);

    for (int i = loop - 1; i < size; i++) {
        double sum = 0;
        for (int j = i; j > i - loop; j--) {
            sum += data[j];
        }
        sumSMA[i] = sum / loop;
    }

    return sumSMA;
}

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

double* RVI(double* close, int stdLength, int loop, int size) {
    int s_v = 2;
    double s_v_1 = 1 + loop;
    double* std;
    double* lower = new double[size];
    double* upper = new double[size];
    double* rvi = new double[size];
    double alpha = s_v / s_v_1;
    double* stdSam;
    double* a = new double[size];
    double* b = new double[size];
    double* ema = new double[size];
    double* emab = new double[size];
    double* sma = new double[size];

    fill_n(ema, size, 0);
    fill_n(emab, size, 0);
    fill_n(a, size, 0);
    fill_n(b, size, 0);
    fill_n(sma, size, 0);
    fill_n(lower, size, 0);
    fill_n(upper, size, 0);
    fill_n(rvi, size, 0);

    cout << "size : " << size << endl;
    std = STD(close, stdLength, size);
    sma = SMA(std, loop, size);

    for (int i = loop; i < size; i++) {
        if (close[i] - close[i - 1] <= 0) {
            if (ema[loop - 1] == 0) {
                ema[loop - 1] = sma[loop - 1];
            }
            a[i] = ema[i] = (alpha * 0) + (1 - alpha) * ema[i - 1];
        }
        else {
            if (ema[loop - 1] == 0) {
                ema[loop - 1] = sma[loop - 1];
            }
            a[i] = ema[i] = (alpha * std[i]) + (1 - alpha) * ema[i - 1];
        }
    }

    for (int i = loop; i < size; i++) {
        if (close[i] - close[i - 1] > 0) {
            if (emab[loop - 1] == 0) {
                emab[loop - 1] = sma[loop - 1];
            }
            b[i] = emab[i] = (alpha * 0) + (1 - alpha) * emab[i - 1];
        }
        else {
            if (emab[loop - 1] == 0) {
                emab[loop - 1] = sma[loop - 1];
            }
            b[i] = emab[i] = (alpha * std[i]) + (1 - alpha) * emab[i - 1];
        }
    }

    for (int i = loop; i < size; i++) {
        rvi[i] = a[i] / (a[i] + b[i]) * 100;
    }

    for (int i = 0; i < size; i++) {
        cout << fixed;
        cout <<"c[" << i << "]" << close[i] <<  "   a[" << i << "]" << a[i] << "  b[" << i << "]" << b[i] << "  rvi[" << i << "]" << rvi[i] << endl;
    }

    return rvi;
}

int main() {
    
    int size = 0;
    int length = 10;
    double* c;

    double* s;
    double* e;
    double* l;

    int a = 3;

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

    RVI(c, 10, 14, size);
}
