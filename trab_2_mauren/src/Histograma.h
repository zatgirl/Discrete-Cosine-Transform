#ifndef __HISTOGRAMA_H__
#define __HISTOGRAMA_H__

#include "gl_canvas2d.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>
#define TAM 256

class Histograma{
    int x, y, altura, largura, valMax;
    unsigned char* data;
    float **grayScale, **r, **g, **b, valMaxR = -1, valMaxG = -1, valMaxB = -1, valMaxGray = -1;
    bool  gray = true, rgb = false;

public:
    Histograma(int _x, int _y, float _larg, float _alt)
    {
     altura  = _alt;    x = _x;
     largura = _larg;   y = _y;
        r = new float*[TAM];
        g = new float*[TAM];
        b = new float*[TAM];
        grayScale = new float*[TAM];

        for (int i = 0; i < TAM; i++) {
            r[i] = new float[TAM];
            g[i] = new float[TAM];
            b[i] = new float[TAM];
            grayScale[i] = new float[TAM];
        }
    }

    void ViewHistograma(float **_grayDiff, float **_rDiff, float **_gDiff, float **_bDiff){

        grayScale = _grayDiff;
        r = _rDiff;
        g = _gDiff;
        b = _bDiff;

        int temp;
        temp = x;
        CV::color(0.2941,0.3058,0.3921);
        CV::rectFill(x,altura,largura,altura+30);
        CV::color(0,0,0);
        CV::text(x+15, altura+11, "HISTOGRAMA");
        CV::color(0.7294,0.7294,0.7294);
        CV::rectFill(x,y,largura,altura);

        for(int i = 0; i <= 255; i++){
            for(int j = 0; j <= 255; j++){
                if (gray == true){
                    CV::color(0.5,0.5,0.5);
                    CV::rectFill((float)x+18,(float)y,(float)x+19,(float)y+grayScale[i][j]);
                }
                if (rgb == true){
                    CV::color(1,0,0);
                    CV::rectFill((float)x+18,(float)y,(float)x+19,(float)y+r[i][j]);
                    CV::color(0,1,0);
                    CV::rectFill((float)x+18,(float)y,(float)x+19,(float)y+g[i][j]);
                    CV::color(0,0,1);
                    CV::rectFill((float)x+18,(float)y,(float)x+19,(float)y+b[i][j]);
                }   x ++;
            } x = 0;
        }   x = temp;
    }

    void GrayChoice(bool _gray){
        gray = true;
        rgb = false;
    }
    void RGBChoice(bool _rgb){
        gray = false;
        rgb = true;
    }

};

#endif
