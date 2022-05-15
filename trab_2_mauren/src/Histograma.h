#ifndef __HISTOGRAMA_H__
#define __HISTOGRAMA_H__

#include "gl_canvas2d.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>

class Histograma{
    int x, y, altura, largura, valMax;
    unsigned char* data;
    float grayScale[255], r[255], g[255], b[255], valMaxR = -1, valMaxG = -1, valMaxB = -1, valMaxGray = -1;
    bool red = false, green = false, blue = false, gray = false, rgb = false;

public:
    Histograma(int _x, int _y, float _larg, float _alt, unsigned char*_data)
    {
     altura  = _alt;    x = _x;
     largura = _larg;   y = _y;
     data = _data;
    }

    void SplitBands(){
        int idx = 0;
        for(int linha = 0; linha < largura; linha++){
            for(int coluna = 0; coluna < altura; coluna++){
                if (idx < 256){
                    r[idx]   = (int)data[idx];
                    if (valMaxR < r[idx]){
                        valMaxR = r[idx];
                    }
                }
                if (idx+1 < 255){
                    g[idx+1] = (int)data[idx+1];
                    if (valMaxG < g[idx]){
                        valMaxG = g[idx];
                    }
                }
                if (idx+2 < 255){
                    b[idx+2] = (int)data[idx+2];
                    if (valMaxB < b[idx]){
                        valMaxB = b[idx];
                    }
                }  idx += 3;
            }
        } idx = 0;
        for(int pos = 0; pos < 256; pos ++){
            grayScale[pos] = (0.3*r[pos] + 0.59*g[pos] + 0.11*b[pos])/3;
            if (valMaxGray < grayScale[pos]){
                valMaxGray = grayScale[pos];
            }
        }
    }

    void ViewHistograma(){
        SplitBands();
        int temp;
        temp = x;
        CV::color(0.2941,0.3058,0.3921);
        CV::rectFill(x,altura,largura,altura+30);
        CV::color(0,0,0);
        CV::text(x+15, altura+11, "HISTOGRAMA");
        CV::color(0.7294,0.7294,0.7294);
        CV::rectFill(x,y,largura,altura);
        for(int i = 0; i <= 255; i++){
            if (red == true){
                CV::color(1,0,0);
                CV::rectFill((float)x+18,(float)y,(float)x+19,(float)y+r[i]);
            }
            if (green == true){
                CV::color(0,1,0);
                CV::rectFill((float)x+18,(float)y,(float)x+19,(float)y+g[i]);
            }
            if (blue == true){
                CV::color(0,0,1);
                CV::rectFill((float)x+18,(float)y,(float)x+19,(float)y+b[i]);
            }
            if (gray == true){
                CV::color(0.5,0.5,0.5);
                CV::rectFill((float)x+18,(float)y,(float)x+19,(float)y+grayScale[i]);
            }
            if (rgb == true){
                CV::color(1,0,0);
                CV::rectFill((float)x+18,(float)y,(float)x+19,(float)y+r[i]);
                CV::color(0,1,0);
                CV::rectFill((float)x+18,(float)y,(float)x+19,(float)y+g[i]);
                CV::color(0,0,1);
                CV::rectFill((float)x+18,(float)y,(float)x+19,(float)y+b[i]);
            }   x ++;
        }   x = temp;
    }

    void RedChoice(bool _red){
        red = true;
        green = false;
        blue = false;
        gray = false;
        rgb = false;
    }
    void GreenChoice(bool _green){
        red = false;
        green = true;
        blue = false;
        gray = false;
        rgb = false;
    }
    void BlueChoice(bool _blue){
        red = false;
        green = false;
        blue = true;
        gray = false;
        rgb = false;
    }
    void GrayChoice(bool _gray){
        red = false;
        green = false;
        blue = false;
        gray = true;
        rgb = false;
    }
    void RGBChoice(bool _rgb){
        red = false;
        green = false;
        blue = false;
        gray = false;
        rgb = true;
    }

};

#endif
