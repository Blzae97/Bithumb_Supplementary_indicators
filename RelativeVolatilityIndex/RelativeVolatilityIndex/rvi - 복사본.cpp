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

double* LMA(double* data, int loop, int size) {
    double* lma = new double[size];
    int weights = 0;
    int loop_w = 0;
    fill_n(lma, size, 0);

    for (int i = 1; i <= loop; i++) {
        weights += i;
    }

    cout << "wegiht: " << weights << endl;

    for (int i = loop - 1; i < size; i++) {
        double sum = 0;
        loop_w = loop;
        for (int j = i; j > i - loop; j--, loop_w--) {
            sum += data[j] * loop_w;
        }
        lma[i] = sum / weights;
    }
    
    cout << "==================================================================" << endl;
    for (int i = 0; i < size; i++) {
        cout << fixed;
        cout.precision(4);
        cout << "close[" << i << "]" << "[" << i + 1 << "]" << data[i] << " LMA[" << i << "]" << "[" << i + 1 << "]" << lma[i] << endl;
    }
    

    return lma;
}

double* EMA(double* data, int loop, int size) {
    int s_v = 2;
    double s_v_1 = 1 + loop;
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

    cout << "==================================================================" << endl;
    cout << "alpha: " << double(alpha) << endl;
    for (int i = 0; i < size; i++) {
        cout << fixed;
        cout.precision(4);
        cout << "close[" << i << "]" << "[" << i + 1 << "]" << data[i] << " SMA[" << i << "]" << "[" << i + 1 << "]" << sma[i] << " EMA[" << i << "]" << "[" << i + 1 << "]" << ema[i] << endl;
    }

    return ema;
}



/*
void RVI(double* c, int loop, int size) {
    int s_v = 2;
    double s_v_1 = 1 + loop;
    double* avgsC = new double[size];
    double* stdC = new double[size];
    double* varianceC = new double[size];
    double* upC = new double[size];
    double* downC = new double[size];
    double* uC = new double[size];
    double* dC = new double[size];
    double* rviC = new double[size];
    double* rvi = new double[size];
    double max = 0, min = 0;
    double* test = new double[size];
    double sung = s_v / s_v_1;

    fill_n(test, size, 0);
    fill_n(rvi, size, 0);
    fill_n(rviC, size, 0);
    fill_n(avgsC, size, 0);
    fill_n(stdC, size, 0);
    fill_n(varianceC, size, 0);
    fill_n(upC, size, 0);
    fill_n(downC, size, 0);
    fill_n(uC, size, 0);
    fill_n(dC, size, 0);

    for (int i = loop-1; i < size; i++) {
        double sum = 0;
        for (int j = i; j > i - loop; j--) {
            sum += c[j];
        }
       avgsC[i] = sum / loop;
    }
    
    for (int i = loop - 1; i < size; i++) {
        double sum = 0;
        for (int j = i; j > i - loop; j--) {
            sum += pow(c[j] - avgsC[j], 2);
        }
        varianceC[i] = sum / loop;
    }

    for (int i = loop - 1; i < size; i++) {
        stdC[i] = sqrt(varianceC[i]);
    }

    
    for (int i = 0; i < size; i++) {
        cout << fixed;
        cout.precision(4);
        cout << "close[" << i << "]" << int(c[i]) << "  test[" << i << "]" << double(test[i]) << "    avgsC[" << i << "]" << int(avgsC[i]) << "  varianceC[" << i << "]" << double(varianceC[i]) << "  std[" << i << "]" << double(stdC[i]) << endl;
    }
    

    for (int i = loop - 1; i < size; i++) {
        if (c[i] > c[i - 1]) {
            upC[i] = stdC[i];
            downC[i] = 0;
        }
        else {
            upC[i] = 0;
            downC[i] = stdC[i];
        }
        uC[i] = double((uC[i - 1] * (loop - 1)) + upC[i]) / loop;
        dC[i] = double((dC[i - 1] * (loop - 1)) + downC[i]) / loop;
        rviC[i] = (uC[i] / ((uC[i] + dC[i])) * 100);
    }
    
    for (int i = 0; i < size; i++) {
        cout << fixed;
        cout.precision(4);
        cout << "close[" << i << "]" << int(c[i]) << "    upc[" << i << "]" << double(upC[i]) << "  downC[" << i << "]" << double(downC[i]) << "  uc[" << i << "]" << double(uC[i]) << "  dc[" << i << "]" << double(dC[i]) << "  riv[" << i << "]" << double(rviC[i]) << endl;
    }

}
*/

/*
void RVI(double* c, int loop, int size) {
    int s_v = 2;
    double s_v_1 = 1 + loop;
    double* avgsC = new double[size];
    double* stdC = new double[size];
    double* varianceC = new double[size];
    double* upC = new double[size];
    double* downC = new double[size];
    double* uC = new double[size];
    double* dC = new double[size];
    double* rviC = new double[size];
    double* rvi = new double[size];
    double max = 0, min = 0;
    double* test = new double[size];
    double sung = s_v / s_v_1;

    fill_n(test, size, 0);
    fill_n(rvi, size, 0);
    fill_n(rviC, size, 0);
    fill_n(avgsC, size, 0);
    fill_n(stdC, size, 0);
    fill_n(varianceC, size, 0);
    fill_n(upC, size, 0);
    fill_n(downC, size, 0);
    fill_n(uC, size, 0);
    fill_n(dC, size, 0);

    for (int i = loop - 1; i < size; i++) {
        double sum = 0;
        for (int j = i; j > i - loop; j--) {
            sum += c[j];
        }
        avgsC[i] = sum / loop;
    }

    for (int i = loop - 1; i < size; i++) {
        double sum = 0;
        for (int j = i; j > i - loop; j--) {
            sum += pow(c[j] - avgsC[j], 2);
        }
        varianceC[i] = sum / loop;
    }

    for (int i = loop - 1; i < size; i++) {
        stdC[i] = sqrt(varianceC[i]);
    }


    for (int i = 0; i < size; i++) {
        cout << fixed;
        cout.precision(4);
        cout << "close[" << i << "]" << int(c[i]) << "  test[" << i << "]" << double(test[i]) << "    avgsC[" << i << "]" << int(avgsC[i]) << "  varianceC[" << i << "]" << double(varianceC[i]) << "  std[" << i << "]" << double(stdC[i]) << endl;
    }


    for (int i = loop - 1; i < size; i++) {
        if (c[i] > c[i - 1]) {
            upC[i] = stdC[i];
            downC[i] = 0;
        }
        else {
            upC[i] = 0;
            downC[i] = stdC[i];
        }
        uC[i] = double((uC[i - 1] * (loop - 1)) + upC[i]) / loop;
        dC[i] = double((dC[i - 1] * (loop - 1)) + downC[i]) / loop;
        rviC[i] = (uC[i] / ((uC[i] + dC[i])) * 100);
    }

    for (int i = 0; i < size; i++) {
        cout << fixed;
        cout.precision(4);
        cout << "close[" << i << "]" << int(c[i]) << "    upc[" << i << "]" << double(upC[i]) << "  downC[" << i << "]" << double(downC[i]) << "  uc[" << i << "]" << double(uC[i]) << "  dc[" << i << "]" << double(dC[i]) << "  riv[" << i << "]" << double(rviC[i]) << endl;
    }

}
*/

int main() {
    
    int size = 0;
    double* c;

    double* s;
    double* e;
    double* l;

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

    // RVI(c, 10 ,size);

    // s = SMA(c, 14, size);

    // e = EMA(c, 14, size);

    // l = LMA(c, 10, size);
}
