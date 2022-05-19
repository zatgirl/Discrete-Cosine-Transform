#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "gl_canvas2d.h"
#include "math.h"
#define M_PI           3.14159265358979323846  /* pi */
#define N 8
#define M 8

class Graph{
    int largura = 0,altura = 0;
    unsigned char *pixel;
    float diffRed[32][32], diffGreen[32][32], diffBlue[32][32], diffGray[32][32];
    float dctGray[32][32], dctRed[32][32], dctGreen[32][32], dctBlue[32][32];
    float idctGray[32][32], idctRed[32][32], idctGreen[32][32], idctBlue[32][32];
    float red[32][32], green[32][32], blue[32][32], grayScale[32][32];
    bool rgb = true, gray = false, quant = false;

    double matrizQuant [8][8] = {{16, 11, 10, 16, 24, 40, 51, 61},
                                {12, 12, 14, 19, 26, 58, 60, 55},
                                {14, 13, 16, 24, 40, 57, 69, 56},
                                {18, 22, 37, 56, 68, 109,103,77},
                                {24, 35, 55, 64, 81, 104,113,92},
                                {49, 64, 78, 87, 103,121,101,101},
                                {72, 92, 95, 98, 112,100,103,99}};

public:
    Graph(){
    }

    Graph(int _largura, int _altura, unsigned char *_pixel)
    {
        pixel = _pixel;
        altura = _altura;
        largura = _largura;
    }

    void SplitBands(){
        int idx = 0, i = 0, j = 0;
        int y = 20, x;
        CV::color(0,0,0);
        CV::text(70, 300, "Original");

        for(int linha = 0; linha < largura; linha++){
            x = 70;
            for(int coluna = 0; coluna < altura; coluna++){
                grayScale[j][i] = ((0.3*((float)(pixel[idx]))) + (0.59*((float)(pixel[idx+1])))+ (0.11*((float)(pixel[idx+2]))));
                red[j][i] = ((float)pixel[idx]);
                green[j][i] = ((float)pixel[idx+1]);
                blue[j][i] = ((float)pixel[idx+2]);
                if (gray == true){
                    CV::color(grayScale[j][i]/255.0, grayScale[j][i]/255.0,grayScale[j][i]/255.0);
                }
                if (rgb == true){
                    CV::color(red[j][i]/255.0,green[j][i]/255.0,blue[j][i]/255.0);
                }
                CV::rectFill(coluna+x, linha+y, coluna+x+6, linha+y+6);
                idx +=3;
                x +=5;
                i ++;
            }
            y+=5;
            j++;
            i = 0;
        }
        x = 20;
        idx = 0;
        j = 0;
    }

    void DCT(bool _rgb, bool _gray){
        rgb = _rgb;
        gray = _gray;
        SplitBands();
        float const_u, const_v, tempDCT_gray, tempDCT_red, tempDCT_green, tempDCT_blue;

        CV::color(0,0,0);
        CV::text(370, 300, "DCT");

        ///Separa a imagem em blocos de 8x8
        for(int imgx = 0; imgx < largura; imgx +=8)
        {

            for(int imgy = 0; imgy < altura; imgy +=8)
            {

                for (int u = 0; u < N; u ++)
                {
                    for (int v = 0; v < M; v ++)
                    {
                        tempDCT_gray = 0;
                        tempDCT_red  = 0;
                        tempDCT_green= 0;
                        tempDCT_blue = 0;
                        for (int x = 0; x < N; x ++)
                            for (int y = 0; y < M; y ++)
                            {
                                tempDCT_gray = (gray == true) ? tempDCT_gray + grayScale[imgx+x][imgy+y] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M)) : tempDCT_gray;
                                tempDCT_red = (rgb == true) ? tempDCT_red + red[imgx+x][imgy+y] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M)) : tempDCT_red;
                                tempDCT_green = (rgb == true) ? tempDCT_green + green[imgx+x][imgy+y] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M)) : tempDCT_green;
                                tempDCT_blue = (rgb == true) ? tempDCT_blue + blue[imgx+x][imgy+y] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M)) : tempDCT_blue;
                            }
                        const_u = (u == 0) ? 1 / sqrt(2) : 1;
                        const_v = (v == 0) ? 1 / sqrt(2) : 1;
                        dctGray[imgx+u][imgy+v] = (gray == true) ? (((1/4.0) * const_u * const_v * tempDCT_gray))  : dctGray[imgx+u][imgy+v];
                        dctRed[imgx+u][imgy+v]  =  (rgb == true) ? (((1/4.0) * const_u * const_v * tempDCT_red))   : dctRed[imgx+u][imgy+v];
                        dctGreen[imgx+u][imgy+v]=  (rgb == true) ? (((1/4.0) * const_u * const_v * tempDCT_green)) : dctGreen[imgx+u][imgy+v];
                        dctBlue[imgx+u][imgy+v] =  (rgb == true) ? (((1/4.0) * const_u * const_v * tempDCT_blue))  : dctBlue[imgx+u][imgy+v];
                        ///Se quant for habilitado
                        dctGray[imgx+u][imgy+v] = ((gray == true)&&(quant == true)) ? round(dctGray[imgx+u][imgy+v]  / matrizQuant[u][v]) : dctGray[imgx+u][imgy+v];
                        dctRed[imgx+u][imgy+v]  = ((rgb == true)&&(quant == true))  ? round(dctRed[imgx+u][imgy+v]   / matrizQuant[u][v]) : dctRed[imgx+u][imgy+v];
                        dctGreen[imgx+u][imgy+v]= ((rgb == true)&&(quant == true))  ? round(dctGreen[imgx+u][imgy+v] / matrizQuant[u][v]) : dctGreen[imgx+u][imgy+v];
                        dctBlue[imgx+u][imgy+v] = ((rgb == true)&&(quant == true))  ? round(dctBlue[imgx+u][imgy+v]  / matrizQuant[u][v]) : dctBlue[imgx+u][imgy+v];

                    }
                }
            }
        }

        ///Desenha a DCT gerada
        int iniciox, inicioy = 20;
        for(int i = 0; i < 32; i ++){
            iniciox = 370;
            for(int j = 0; j < 32; j ++){
                if(gray == true) {
                    CV::color(dctGray[i][j]/255.0, dctGray[i][j]/255.0, dctGray[i][j]/255.0);
                }
                if(rgb == true) {
                    CV::color(dctRed[i][j]/255.0, dctGreen[i][j]/255.0, dctBlue[i][j]/255.0);
                }
                CV::rectFill(iniciox+j,inicioy+i,iniciox+j+6,inicioy+i+6);
                iniciox +=5;
            } inicioy +=5;
        }

        IDCT();
    }

    void IDCT(){
        float const_u, const_v, tempIDCT_gray, tempIDCT_red, tempIDCT_green, tempIDCT_blue;
        CV::color(0,0,0);
        CV::text(640, 300, "IDCT");

        for(int imgx = 0; imgx < largura; imgx +=8)
        {
            for(int imgy = 0; imgy < altura; imgy +=8)
            {

                for (int x = 0; x < N; x ++)
                {
                    for (int y = 0; y < M; y ++)
                    {
                        tempIDCT_gray   = 0;
                        tempIDCT_red    = 0;
                        tempIDCT_green  = 0;
                        tempIDCT_blue= 0;
                        for (int u = 0; u < N; u ++)
                            for (int v = 0; v < M; v ++)
                            {
                                const_u = (u == 0) ? 1 / sqrt(2) : 1;
                                const_v = (v == 0) ? 1 / sqrt(2) : 1;
                                tempIDCT_gray  = (gray == true) ? (tempIDCT_gray + const_u * const_v * dctGray[imgx+u][imgy+v] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M))) : tempIDCT_gray;
                                idctGray[imgx+u][imgy+v] = ((gray == true)&&(quant == true)) ? round(idctGray[imgx+u][imgy+v]  * matrizQuant[u][v]) : idctGray[imgx+u][imgy+v];
                                tempIDCT_red   = (rgb == true) ? (tempIDCT_red + const_u * const_v * dctRed[imgx+u][imgy+v] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M))) : tempIDCT_red;
                                idctRed[imgx+u][imgy+v]  = ((rgb == true)&&(quant == true))  ? round(idctRed[imgx+u][imgy+v]   / matrizQuant[u][v]) : idctRed[imgx+u][imgy+v];
                                tempIDCT_green = (rgb == true) ? (tempIDCT_green + const_u * const_v * dctGreen[imgx+u][imgy+v] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M))) : tempIDCT_green;
                                idctGreen[imgx+u][imgy+v]= ((rgb == true)&&(quant == true))  ? round(idctGreen[imgx+u][imgy+v] / matrizQuant[u][v]) : idctGreen[imgx+u][imgy+v];
                                tempIDCT_blue  = (rgb == true) ? (tempIDCT_blue + const_u * const_v * dctBlue[imgx+u][imgy+v] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M))) : tempIDCT_blue;
                                idctBlue[imgx+u][imgy+v] = ((rgb == true)&&(quant == true))  ? round(idctBlue[imgx+u][imgy+v]  / matrizQuant[u][v]) : idctBlue[imgx+u][imgy+v];
                            }
                        idctGray[imgx+x][imgy+y] = (gray == true) ? (1/4.0) * const_u * const_v * tempIDCT_gray  : idctGray[imgx+x][imgy+y];
                        idctRed[imgx+x][imgy+y]  =  (rgb == true) ? (1/4.0) * const_u * const_v * tempIDCT_red   : idctRed[imgx+x][imgy+y];
                        idctGreen[imgx+x][imgy+y]=  (rgb == true) ? (1/4.0) * const_u * const_v * tempIDCT_green : idctGreen[imgx+x][imgy+y];
                        idctBlue[imgx+x][imgy+y] =  (rgb == true) ? (1/4.0) * const_u * const_v * tempIDCT_blue  : idctBlue[imgx+x][imgy+y];
                    }
                }
            }
        }

        int iniciox, inicioy = 20;
        for(int i = 0; i < 32; i ++){
            iniciox = 640;
            for(int j = 0; j < 32; j ++){
                if(gray == true) {
                    CV::color(idctGray[i][j]/255.0, idctGray[i][j]/255.0, idctGray[i][j]/255.0);
                }
                if(rgb == true) {
                    CV::color(idctRed[i][j]/255.0, idctGreen[i][j]/255.0, idctBlue[i][j]/255.0);
                }
                CV::rectFill(iniciox+j,inicioy+i,iniciox+j+6,inicioy+i+6 );
                iniciox +=5;
            } inicioy +=5;
        }
        diff();
    }

    void diff(){

        for (int x = 0; x < largura; x ++)
        {
            for (int y = 0; y < altura; y ++)
            {
                diffGray[x][y]  = abs(grayScale[x][y]- idctGray[x][y]);
                diffRed[x][y]   = abs(red[x][y] - idctRed[x][y]);
                diffGreen[x][y] = abs(green[x][y] - idctGreen[x][y]);
                diffBlue[x][y]  = abs(blue[x][y] - idctGreen[x][y]);
            }
        }

        int iniciox, inicioy = 70;
        for(int i = 0; i < largura; i ++){
            iniciox = 640;
            for(int j = 0; j < altura; j ++){
                if(gray == true) {
                    CV::color(diffGray[i][j]/255.0, diffGray[i][j]/255.0, diffGray[i][j]/255.0);
                }
                if(rgb == true) {
                    CV::color(diffRed[i][j]/255.0, diffGreen[i][j]/255.0, diffBlue[i][j]/255.0);
                }
                CV::rectFill(iniciox+j,inicioy+i,iniciox+j+6,inicioy+i+6 );
                iniciox +=5;
            } inicioy +=5;
        }
    }


    void quantChoice(bool _quant){
        quant = _quant;

//        DCT();
    }


};
#endif

