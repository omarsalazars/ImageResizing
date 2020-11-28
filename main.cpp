#define cimg_use_jpeg
#define cimg_use_png
#include "CImg.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>

using namespace cimg_library;
using namespace std;

int incrementoX=4,incrementoY=4,regionX,regionY;
int iniX=0,iniY=0;

CImg<unsigned char> image("Imagen1/img.jpg"),*region,*aumentada;

void  Aumentada()
{
    int width=(regionX-iniX)+((regionX-iniX-1)*incrementoX),
    height=(regionY-iniY)+((regionY-iniY-1)*incrementoY);
    cout<<"Width new: "<<width<<endl<<"Height new: "<<height<<endl;
    aumentada=new CImg<unsigned char>(width,height,image.depth(),image.spectrum(),0);
    int auxX=iniX,auxY=iniY;
    for(int y=0;y<aumentada->height() && auxY<regionY;y+=incrementoY+1)
    {
        for(int x=0;x<aumentada->width() && auxX<regionX;x+=incrementoX+1)
        {
            for(int z=0;z<aumentada->depth();z++)
            {
                for(int c=0;c<aumentada->spectrum();c++)
                {
                        (*aumentada)(x,y,z,c)=image(auxX,auxY,z,c);
                }
            }
            auxX++;
        }
        auxY++;
        auxX=iniX;
    }
}

void init()
{
    regionX=200;
    regionY=200;
    iniX=100;
    iniY=100;
    cout<<"Ancho: "<<image.width()<<endl<<"Altura: "<<image.height()<<endl;
    cout<<"espectro: "<<image.spectrum()<<endl;
    //fstream f("arc.txt",ios::out);
    int width=(regionX-iniX),cont=0,
    height=(regionY-iniY);
    region=new CImg<unsigned char>(width,height,image.depth(),image.spectrum(),0);
    int auxX=iniX,auxY=iniY;
    for(int y=0;y<region->height() && auxY<regionY;y++)
    {
        for(int x=0;x<region->width() && auxX<regionX;x++)
        {
                for(int z=0;z<region->depth();z++)
                {
                    //f<<"{ x="<<setw(2)<<x<<" y="<<setw(2)<<y<<" ";
                    for(int c=0;c<region->spectrum();c++)
                    {
                        (*region)(x,y,z,c)=image(auxX,auxY,z,c);
                        //f<<setw(3)<<(int)image(auxX,auxY,z,c)<<" ";
                    }
                    //f<<"},  ";
                }
                auxX++;
                cont++;
        }
        //f<<"\n";
        auxX=iniX;
        auxY++;
    }
    //f.close();
    Aumentada();
}

double Lagrange(double x,int n,double *xs,double *fx)
{
    double P=0;
    for(int i=0;i<n;i++)
    {
        double d=1,k=1;
        for(int j=0;j<n;j++)
        {
            if(i==j) continue;
            k*=(double)x-xs[j];
            d*=(double)xs[i]-xs[j];
        }
        P+=(k/d)*fx[i];
    }
    if(P>255)
        return 255;
    if(P<0)
        return 0;
    return P;
}

CImg<unsigned char> NInterpolacion(int grado)
{
    CImg<unsigned char> img=*aumentada;
    //EJES X
    for(int y=0;y<img.height();y+=(incrementoY+1))
    {
        for(int x=0;x+(incrementoX*grado+grado)<img.width();x+=(incrementoX+1))
        {
            double xs[grado+1],fx[3][grado+1];int aux=0;
            for(int i=x;i<=x+(incrementoX*grado+grado);i+=incrementoX+1)
            {
                xs[aux]=i;
                for(int c=0;c<img.spectrum();c++)
                    fx[c][aux]=img(i,y,0,c);
                aux++;
            }
            for(int i=x;i<=x+(incrementoX*grado+grado);i++)
            {
                for(int c=0;c<img.spectrum();c++)
                {
                    img(i,y,0,c)=Lagrange(i,grado+1,xs,fx[c]);
                }
            }
        }
    }

    //EJES Y

    for(int x=0;x<img.width();x++)
    {
        for(int y=0;y+(incrementoY*grado+grado)<img.height();y+=(incrementoY+1))
        {
            double xs[grado+1],fx[3][grado+1];int aux=0;
            for(int i=y;i<=y+(incrementoY*grado+grado);i+=incrementoY+1)
            {
                xs[aux]=i;
                for(int c=0;c<img.spectrum();c++)
                    fx[c][aux]=img(x,i,0,c);
                aux++;
            }
            for(int i=y;i<=y+incrementoY*grado+grado;i++)
            {
                for(int c=0;c<img.spectrum();c++)
                {
                    img(x,i,0,c)=Lagrange(i,grado+1,xs,fx[c]);
                }
            }
        }
    }
    return img;
}

CImg<unsigned char> Promedio(CImg<unsigned char> i1,CImg<unsigned char> i2)
{
    int width=(regionX-iniX)+((regionX-iniX-1)*incrementoX),
    height=(regionY-iniY)+((regionY-iniY-1)*incrementoY);
    CImg<unsigned char> img(width,height,image.depth(),image.spectrum());
    for(int y=0;y<img.height();y++)
        for(int x=0;x<img.width();x++)
            for(int c=0;c<img.spectrum();c++)
            {
                int m=i1(x,y,0,c),m2=i2(x,y,0,c);
                img(x,y,0,c)=(m+m2)/2;
            }
    return img;

}

CImg<unsigned char> NearestNeighbor()
{
    int width=(regionX-iniX)+((regionX-iniX-1)*incrementoX),
    height=(regionY-iniY)+((regionY-iniY-1)*incrementoY);
    CImg<unsigned char> img(width,height,image.depth(),image.spectrum(),0);
    int auxX=iniX,auxY=iniY;
    for(int y=0;y<img.height() && auxY<regionY;y++)
    {
        for(int x=0;x<img.width() && auxX<regionX;x++)
        {
            for(int z=0;z<img.depth();z++)
            {
                for(int c=0;c<img.spectrum();c++)
                {
                        img(x,y,z,c)=image(auxX,auxY,z,c);
                }
            }
            if(x%(incrementoX+1)==0)
                auxX++;
        }
        if(y%(incrementoY+1)==0)
            auxY++;
        auxX=iniX;
    }
    return img;
}

int main()
{
    cimg::imagemagick_path("C:\\Program Files\\ImageMagick-7.0.8-Q16\\convert.exe",true);
    cimg::graphicsmagick_path("C:\\Program Files\\GraphicsMagick-1.3.31-Q16\\gm.exe",true);
    init();

    CImg<unsigned char> bilineal=NInterpolacion(2);
    CImg<unsigned char> bicubica=NInterpolacion(3);
    CImg<unsigned char> nearest=NearestNeighbor();
    CImg<unsigned char> promedio=Promedio(bilineal,bicubica);
    CImg<unsigned char> quinta=NInterpolacion(5);
    CImg<unsigned char> decima=NInterpolacion(10);

    CImgDisplay main_disp(image,"Original");
    CImgDisplay draw_dispR(*aumentada,"Aumentada");
    CImgDisplay draw_dispA(promedio,"Promedios");
    CImgDisplay draw_dispJ(bicubica,"Interpolacion bicubica");
    CImgDisplay draw_dispK(bilineal,"Interpolacion bilineal");
    CImgDisplay draw_dispN(nearest,"Nearest Neighbor");
    CImgDisplay draw_dispAux(*region,"aux");

    /*bilineal.save("bilineal.png");
    bicubica.save("bicubica.png");
    nearest.save("nearest.png");
    promedio.save("promedio.png");
    quinta.save("quinta.png");
    decima.save("decima.png");
    aumentada->save("aumentada.png");
    region->save("aux.png");*/

    while (!main_disp.is_closed()){
        main_disp.wait();
    }


    return 0;
}
