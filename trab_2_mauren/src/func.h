#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "gl_canvas2d.h"
#include "math.h"
#define M_PI           3.14159265358979323846  /* pi */
#define N 8
#define M 8
#define TAM 256

class Graph{
    Histograma *histograma;
    int largura = 0,altura = 0;
    unsigned char *pixel;

    float **diffRed, **diffGreen, **diffBlue, **diffGray;
    float **dctGray, **dctRed, **dctGreen, **dctBlue;
    float **idctGray, **idctRed, **idctGreen, **idctBlue;
    float **red, **green, **blue, **grayScale;

    bool rgbC = true, grayC = false, quant = false;

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
        histograma = new Histograma(20,330,280,560);
        ///Alocação de memória  ----Para o Canvas2D não morrer
        diffRed = new float*[TAM];          dctGray = new float*[TAM];
        diffGreen = new float*[TAM];        dctRed = new float*[TAM];
        diffBlue = new float*[TAM];         dctGreen = new float*[TAM];
        diffGray = new float*[TAM];         dctBlue = new float*[TAM];

        idctGray = new float*[TAM];         red = new float*[TAM];
        idctRed = new float*[TAM];          green = new float*[TAM];
        idctGreen = new float*[TAM];        blue = new float*[TAM];
        idctBlue = new float*[TAM];         grayScale = new float*[TAM];

        for (int i = 0; i < TAM; i++) {
            diffRed[i] = new float[TAM];    dctGray[i]= new float[TAM];
            diffGreen[i] = new float[TAM];  dctRed[i] = new float[TAM];
            diffBlue[i] = new float[TAM];   dctGreen[i] = new float[TAM];
            diffGray[i] = new float[TAM];   dctBlue[i] = new float[TAM];

            idctGray[i] = new float[TAM];   red[i] = new float[TAM];
            idctRed[i] = new float[TAM];    green[i] = new float[TAM];
            idctGreen[i] = new float[TAM];  blue[i] = new float[TAM];
            idctBlue[i] = new float[TAM];   grayScale[i] = new float[TAM];
        }

    }

    ///Load: Carrega todas as func
    void Load(){
        SplitBands();
        DCT();
        IDCT();
        DIFF();
    }

    void SplitBands(){
        int idx = 0, i = 0, j = 0;
        for(int linha = 0; linha < largura; linha++){
            for(int coluna = 0; coluna < altura; coluna++){
                grayScale[j][i] = ((0.3*((float)(pixel[idx]))) + (0.59*((float)(pixel[idx+1])))+ (0.11*((float)(pixel[idx+2]))));
                red[j][i] = ((float)pixel[idx]);
                green[j][i] = ((float)pixel[idx+1]);
                blue[j][i] = ((float)pixel[idx+2]);
                idx +=3;
                i ++;
            }
            j++;
            i = 0;
        }
        idx = 0;
        j = 0;
    }

    void DCT(){
        float const_u, const_v, tempDCT_gray, tempDCT_red, tempDCT_green, tempDCT_blue;

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
                                tempDCT_gray = (grayC == true) ? tempDCT_gray + grayScale[imgx+x][imgy+y] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M)) : tempDCT_gray;
                                tempDCT_red = (rgbC == true) ? tempDCT_red + red[imgx+x][imgy+y] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M)) : tempDCT_red;
                                tempDCT_green = (rgbC == true) ? tempDCT_green + green[imgx+x][imgy+y] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M)) : tempDCT_green;
                                tempDCT_blue = (rgbC == true) ? tempDCT_blue + blue[imgx+x][imgy+y] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M)) : tempDCT_blue;
                            }
                        const_u = (u == 0) ? 1 / sqrt(2) : 1;
                        const_v = (v == 0) ? 1 / sqrt(2) : 1;
                        dctGray[imgx+u][imgy+v] = (grayC == true) ? (((1/4.0) * const_u * const_v * tempDCT_gray))  : dctGray[imgx+u][imgy+v];
                        dctRed[imgx+u][imgy+v]  =  (rgbC == true) ? (((1/4.0) * const_u * const_v * tempDCT_red))   : dctRed[imgx+u][imgy+v];
                        dctGreen[imgx+u][imgy+v]=  (rgbC == true) ? (((1/4.0) * const_u * const_v * tempDCT_green)) : dctGreen[imgx+u][imgy+v];
                        dctBlue[imgx+u][imgy+v] =  (rgbC == true) ? (((1/4.0) * const_u * const_v * tempDCT_blue))  : dctBlue[imgx+u][imgy+v];
                        ///Se quant for habilitado
                        dctGray[imgx+u][imgy+v] = ((grayC == true)&&(quant == true)) ? round(dctGray[imgx+u][imgy+v]  / matrizQuant[u][v]) : dctGray[imgx+u][imgy+v];
                        dctRed[imgx+u][imgy+v]  = ((rgbC == true)&&(quant == true))  ? round(dctRed[imgx+u][imgy+v]   / matrizQuant[u][v]) : dctRed[imgx+u][imgy+v];
                        dctGreen[imgx+u][imgy+v]= ((rgbC == true)&&(quant == true))  ? round(dctGreen[imgx+u][imgy+v] / matrizQuant[u][v]) : dctGreen[imgx+u][imgy+v];
                        dctBlue[imgx+u][imgy+v] = ((rgbC == true)&&(quant == true))  ? round(dctBlue[imgx+u][imgy+v]  / matrizQuant[u][v]) : dctBlue[imgx+u][imgy+v];

                    }
                }
            }
        }
    }

    void IDCT(){
        float const_u, const_v, tempIDCT_gray, tempIDCT_red, tempIDCT_green, tempIDCT_blue;

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
                                tempIDCT_gray  = (grayC == true) ? (tempIDCT_gray + const_u * const_v * dctGray[imgx+u][imgy+v] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M))) : tempIDCT_gray;
                                idctGray[imgx+u][imgy+v] = ((grayC == true)&&(quant == true)) ? round(idctGray[imgx+u][imgy+v]  * matrizQuant[u][v]) : idctGray[imgx+u][imgy+v];
                                tempIDCT_red   = (rgbC == true) ? (tempIDCT_red + const_u * const_v * dctRed[imgx+u][imgy+v] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M))) : tempIDCT_red;
                                idctRed[imgx+u][imgy+v]  = ((rgbC == true)&&(quant == true))  ? round(idctRed[imgx+u][imgy+v]   / matrizQuant[u][v]) : idctRed[imgx+u][imgy+v];
                                tempIDCT_green = (rgbC == true) ? (tempIDCT_green + const_u * const_v * dctGreen[imgx+u][imgy+v] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M))) : tempIDCT_green;
                                idctGreen[imgx+u][imgy+v]= ((rgbC == true)&&(quant == true))  ? round(idctGreen[imgx+u][imgy+v] / matrizQuant[u][v]) : idctGreen[imgx+u][imgy+v];
                                tempIDCT_blue  = (rgbC == true) ? (tempIDCT_blue + const_u * const_v * dctBlue[imgx+u][imgy+v] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M))) : tempIDCT_blue;
                                idctBlue[imgx+u][imgy+v] = ((rgbC == true)&&(quant == true))  ? round(idctBlue[imgx+u][imgy+v]  / matrizQuant[u][v]) : idctBlue[imgx+u][imgy+v];
                            }
                        idctGray[imgx+x][imgy+y] = (grayC == true) ? (1/4.0) * const_u * const_v * tempIDCT_gray  : idctGray[imgx+x][imgy+y];
                        idctRed[imgx+x][imgy+y]  =  (rgbC == true) ? (1/4.0) * const_u * const_v * tempIDCT_red   : idctRed[imgx+x][imgy+y];
                        idctGreen[imgx+x][imgy+y]=  (rgbC == true) ? (1/4.0) * const_u * const_v * tempIDCT_green : idctGreen[imgx+x][imgy+y];
                        idctBlue[imgx+x][imgy+y] =  (rgbC == true) ? (1/4.0) * const_u * const_v * tempIDCT_blue  : idctBlue[imgx+x][imgy+y];
                    }
                }
            }
        }
    }

    void DIFF(){
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
    }

    ///ViewGraphs
    void ViewGraphs(){
        CV::color(0,0,0);
        CV::text(30, 300, "Original");
        int iniciox, inicioy = 20;
        for(int i = 0; i < largura; i ++){
            iniciox = 30;
            for(int j = 0; j < altura; j ++){
                if (grayC == true){
                    CV::color(grayScale[i][j]/255.0, grayScale[i][j]/255.0,grayScale[i][j]/255.0);
                }
                if (rgbC == true){
                    CV::color(red[i][j]/255.0,green[i][j]/255.0,blue[i][j]/255.0);
                }
                CV::rectFill(iniciox+j,inicioy+i,iniciox+j+2,inicioy+i+2);
                iniciox +=0.5;
            } inicioy +=0.5;
        }

        ViewDCT();
        ViewIDCT();
        ViewDiff();

    }

    void ViewDCT(){
        CV::color(0,0,0);
        CV::text(309, 300, "DCT");
        int iniciox, inicioy = 20;
        for(int i = 0; i < TAM; i ++){
            iniciox = 309;
            for(int j = 0; j < TAM; j ++){
                if(grayC == true) {
                    CV::color(dctGray[i][j]/255.0, dctGray[i][j]/255.0, dctGray[i][j]/255.0);
                }
                if(rgbC == true) {
                    CV::color(dctRed[i][j]/255.0, dctGreen[i][j]/255.0, dctBlue[i][j]/255.0);
                }
                CV::rectFill(iniciox+j,inicioy+i,iniciox+j+2,inicioy+i+2);
                iniciox +=0.5;
            } inicioy +=0.5;
        }
    }

    void ViewIDCT(){
        CV::color(0,0,0);
        CV::text(588, 300, "IDCT");
        int iniciox, inicioy = 20;
        for(int i = 0; i < TAM; i ++){
            iniciox = 588;
            for(int j = 0; j < TAM; j ++){
                if(grayC == true) {
                    CV::color(idctGray[i][j]/255.0, idctGray[i][j]/255.0, idctGray[i][j]/255.0);
                }
                if(rgbC == true) {
                    CV::color(idctRed[i][j]/255.0, idctGreen[i][j]/255.0, idctBlue[i][j]/255.0);
                }
                CV::rectFill(iniciox+j,inicioy+i,iniciox+j+2,inicioy+i+2);
                iniciox +=0.5;
            } inicioy +=0.5;
        }
    }

    void ViewDiff(){
        CV::color(0,0,0);
        CV::text(867, 300, "DIFERENCA");
        int iniciox, inicioy = 20;
        for(int i = 0; i < largura; i ++){
            iniciox = 867;
            for(int j = 0; j < altura; j ++){
                if(grayC == true) {
                    CV::color(diffGray[i][j]/255.0, diffGray[i][j]/255.0, diffGray[i][j]/255.0);
                }
                if(rgbC == true) {
                    CV::color(diffRed[i][j]/255.0, diffGreen[i][j]/255.0, diffBlue[i][j]/255.0);
                }
                CV::rectFill(iniciox+j,inicioy+i,iniciox+j+2,inicioy+i+2);
                iniciox +=0.5;
            } inicioy +=0.5;
        }
    }

    void quantChoice(bool _quant){
        quant = _quant;
    }

    void GrayChoice(bool _gray){
        grayC = true;
        rgbC = false;
        histograma->ViewHistograma(diffGray,diffRed,diffGreen,diffBlue);
        histograma->GrayChoice(true);
    }

    void RGBChoice(bool _rgb){
        grayC = false;
        rgbC = true;
        histograma->ViewHistograma(diffGray,diffRed,diffGreen,diffBlue);
        histograma->RGBChoice(true);
    }


};
#endif

