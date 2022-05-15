#ifndef __DRAW_H__
#define __DRAW_H__

#include "gl_canvas2d.h"
#include "Histograma.h"


class Img{
    int altura, largura, scale =1;
    int x, y, outroX, outroY;
    unsigned char* pixel;
    int imagem[200][200];
    float grayScale;
    bool red = false, green = false, blue = false, gray = false, rgb = false;
    //Histograma *histograma;

public:
    Img(){
    }

    Img(int _altura, int _largura, int _iniciox, int _inicioy, unsigned char* _pixel){
        altura = _altura;
        largura = _largura;
        outroX = _iniciox;
        outroY = _inicioy;
        pixel = _pixel;
        //histograma = new Histograma(5,20,280,260,pixel);
    }

    int getStartX (int _StartX){
        return outroX = _StartX;
    }

    int getStartY (int _StartY){
        return outroY = _StartY;
    }

    void ViewImg(int _x, int _y, int _scale, bool _rotClock, bool _rotCounterClock){
        int idx = 0;
        bool clockwize = false, counterclockwize = false, defaul = true;
            if(_rotClock == true){
                counterclockwize = false;
                clockwize = true;
                defaul = false;
                x = _y;
                y = _x;
                printf("ent");
            }
            if(_rotClock == true){
                counterclockwize = true;
                clockwize = false;
                defaul = false;
                x = _y;
                y = _x;
            }
            if((_rotCounterClock == false)&&(_rotClock == false)){
                counterclockwize = false;
                clockwize = false;
                defaul = true;
                x = _x;
                y = _y;
            }
            if (_scale != scale){
                scale = _scale;
            }
            y = _y;

            for(int linha = 0; linha < largura; linha++){
                x = _x;
                for(int coluna = 0; coluna < altura; coluna++){
                    if(rgb == true){
                        CV::color((float)(pixel[idx])/255.0, (float)(pixel[idx+1])/255.0, (float)(pixel[idx+2])/255.0);
                    }
                    if(red == true){
                        CV::color((float)(pixel[idx])/255.0, 0, 0);
                    }
                    if(green == true){
                        CV::color(0, (float)(pixel[idx+1])/255.0, 0);
                    }
                    if(blue == true){
                        CV::color(0, 0, (float)(pixel[idx+2])/255.0);
                    }
                    if(gray == true){
                        grayScale = (0.3*((float)(pixel[idx])/255.0)) + (0.59*((float)(pixel[idx+1])/255.0))+ (0.11*((float)(pixel[idx+2])/255.));
                        CV::color(grayScale, grayScale, grayScale);
                    }
                    if(defaul == true){
                        CV::rectFill(coluna+x, linha+y, coluna+x+8, linha+y+8);
                    }
                    if(clockwize == true){
                        CV::rectFill(linha+y, coluna+x, linha+y+2, coluna+x+2);
                        //printf("ent");
                        Sleep(10);
                    }
                    idx +=3; x +=scale;
                }   y += scale;
            }   idx = 0;
    }

    int setNewStartX (){
        return x;
    }
    int setNewStartY (){
        return y;
    }

    void RedChoice(bool _red){
        red = true;
        green = false;
        blue = false;
        gray = false;
        rgb = false;
        //histograma->ViewHistograma(true,false,false,false,false);
    }

    void GreenChoice(bool _green){
        red = false;
        green = true;
        blue = false;
        gray = false;
        rgb = false;
        //histograma->ViewHistograma(false,true,false,false,false);
    }

    void BlueChoice(bool _blue){
        red = false;
        green = false;
        blue = true;
        gray = false;
        rgb = false;
        //histograma->ViewHistograma(false,false,true,false,false);
    }

    void GrayChoice(bool _gray){
        red = false;
        green = false;
        blue = false;
        gray = true;
        rgb = false;
        //histograma->ViewHistograma(false,false,false,true,false);
    }

    void RGBChoice(bool _rgb){
        red = false;
        green = false;
        blue = false;
        gray = false;
        rgb = true;
        //histograma->ViewHistograma(false,false,false,false,true);
    }

    bool ColidiuImg(int mx, int my)
    {
        //printf("collider");
        if( mx >= x && mx <= (x + (float)largura) && my >= y && my <= (y + (float)altura) )
        {
            return true;
        }
            return false;
    }
};
#endif
