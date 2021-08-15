#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include "json/json.h"

#pragma warning(disable: 4996);

using namespace std;

double* SMA(double* data, int loop, int size) {
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

double* RMA(double* data, int loop, int size) {
    int s_v = 1;
    double s_v_1 = loop;
    double alpha = s_v / s_v_1;
    double* ema = new double[size];
    double* sma;

    fill_n(ema, size, 0);

    sma = SMA(data, loop, size);
    if (ema[loop - 1] == 0) {
        ema[loop - 1] = sma[loop - 1];
    }

    for (int i = loop; i < size; i++) {
        ema[i] = (alpha * data[i]) + (1 - alpha) * ema[i - 1];
    }

    return ema;
}

double* RSI(double* close, int loop, int size) {
    double* uR, * dR, * u, * d, * rsi;

    u = new double[size];
    d = new double[size];
    rsi = new double[size];

    fill_n(u, size, 0);
    fill_n(d, size, 0);
    fill_n(rsi, size, 0);


    for (int i = 1; i < size; i++) {
        double judgment = close[i] - close[i - 1];
        if (judgment >= 0) {
            u[i] = judgment;
            d[i] = 0;
        }
        else {
            u[i] = 0;
            d[i] = -judgment;
        }
    }

    uR = RMA(u, loop, size);
    dR = RMA(d, loop, size);

    for (int i = 0; i < size; i++) {
        rsi[i] = dR[i] == 0 ? 100 : uR[i] == 0 ? 0 : 100 - (100 / (1 + uR[i] / dR[i]));
    }

    return rsi;
}

int main() {
    int size = 0;
    double* c, * rsi;

    Json::Value root;
    Json::Reader reader;
    ifstream json("test.json", ifstream::binary);
    reader.parse(json, root);   //json >> root; 두 구문 모두 json을 파싱하는데 사용할 수 있다.

    size = root["data"].size(); // API로 불러온 JSON 데이터 사이즈 구하기

    c = new double[size];
    rsi = new double[size];

    fill_n(c, size, 0);
    fill_n(rsi, size, 0);


    for (int i = 0; i < size; i++) {
        double cc;
        stringstream convert1(root["data"][i][2].asString());
        convert1 >> cc;
        c[i] = cc;
    }

   rsi = RSI(c, 10, size);

   for (int i = 0; i < size; i++) {
       cout << fixed;
       cout << "close[" << i << "]" << c[i] << " RSI[" << i << "]" << rsi[i] << endl;
   }


}