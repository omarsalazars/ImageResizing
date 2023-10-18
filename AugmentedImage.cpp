#include "CImg.h"
#include "AugmentedImage.h"
#include "DisplayedImage.h"
#include "MathUtil.h"
#include <iostream>
#include <string>

using namespace std;
using namespace cimg_library;

/**
* Calculate new width based on incrementX.
*/
int AugmentedImage::newWidth(){
    return (this->width())*(this->incrementX);
}

/**
* Calculate new height based on incrementY.
*/
int AugmentedImage::newHeight(){
    return (this->height())*(this->incrementY);
}

/**
* Returns augmented image without any processing. Fills empty space with black pixels.
*/
DisplayedImage AugmentedImage::EmptySpace()
{
    CImg<unsigned char> expandedImage(this->newWidth(), this->newHeight(), this->depth(), this->spectrum(), 0);
    for(int y=0;y<expandedImage.height();y++)
    {
        for(int x=0;x<expandedImage.width();x++)
        {
            for(int z=0;z<expandedImage.depth();z++)
            {
                for(int c=0;c<expandedImage.spectrum();c++)
                {
                        if(x%incrementX==0 && y%incrementY==0)
                            expandedImage(x,y,z,c)=(*this)(x/incrementX,y/incrementY,z,c);
                }
            }
        }
    }
    return DisplayedImage(expandedImage, "emptySpace.png", "Empty Space");
}

/**
* Returns the augmented image filling black spaces with the specified grade of Lagrange interpolation.
*/
DisplayedImage AugmentedImage::NLagrangeInterpolation(int grade)
{
    CImg<unsigned char> expandedImage = this->EmptySpace().image;
    int xAxisCalculatedZone = grade*incrementX;
    int yAxisCalculatedZone = grade*incrementY;

    if(expandedImage.width() < xAxisCalculatedZone){
        cerr<<"Impossible to interpolate. Expanded Image width should be larger than interpolation grade*incrementX"<<endl;
        return DisplayedImage(expandedImage, "grade"+to_string(grade)+".png", "Grade "+grade);
    }
    if(expandedImage.height() < yAxisCalculatedZone){
        cerr<<"Impossible to interpolate. Expanded Image height should be larger than interpolation grade*incrementY"<<endl;
        return DisplayedImage(expandedImage, "grade"+to_string(grade)+".png", "Grade "+grade);
    }
    
    // X Axis
    for(int y=0; y<expandedImage.height(); y+=incrementY)
    {
        for(int x=0;x<expandedImage.width(); x+=xAxisCalculatedZone)
        {
            // Fill last columns
            if(x+xAxisCalculatedZone > expandedImage.width())
                x = expandedImage.width()-xAxisCalculatedZone;
            double xs[grade+1],fx[3][grade+1];
            int aux=0;
            for(int i=x; i<=x+xAxisCalculatedZone ; i+=incrementX)
            {
                xs[aux]=i;
                for(int c=0;c<expandedImage.spectrum();c++)
                    fx[c][aux]=expandedImage(i,y,0,c);
                aux++;
            }
            for(int i=x;i<x+xAxisCalculatedZone;i++)
            {
                for(int c=0;c<expandedImage.spectrum();c++)
                {
                    if(i%incrementX!=0) // Skip recalculate original pixel
                        expandedImage(i,y,0,c)=MathUtil::Lagrange(i,grade+1,xs,fx[c]);
                }
            }
        }
    }

    // Y Axis
    for(int x=0;x<expandedImage.width();x++)
    {
        for(int y=0; y < expandedImage.height(); y+=yAxisCalculatedZone)
        {
            // Fill last rows
            if(y+yAxisCalculatedZone > expandedImage.height())
                y = expandedImage.height()-yAxisCalculatedZone;
            double xs[grade+1],fx[3][grade+1];int aux=0;
            for(int i=y;i<=y+yAxisCalculatedZone;i+=this->incrementY)
            {
                xs[aux]=i;
                for(int c=0;c<expandedImage.spectrum();c++)
                    fx[c][aux]=expandedImage(x,i,0,c);
                aux++;
            }
            for(int i=y; i<y+yAxisCalculatedZone; i++)
            {
                for(int c=0;c<expandedImage.spectrum();c++)
                {
                    if(i%incrementY!=0) // Skip recalculate original pixel
                        expandedImage(x,i,0,c)=MathUtil::Lagrange(i,grade+1,xs,fx[c]);
                }
            }
        }
    }
    return DisplayedImage(expandedImage, "grade"+to_string(grade)+".png", "Grade "+to_string(grade));
}

/**
* Returns augmented image filling black spaces with the average color of the neighbors.
*/
DisplayedImage AugmentedImage::Average(CImg<unsigned char> i1,CImg<unsigned char> i2)
{
    CImg<unsigned char> averageImage(this->newWidth(), this->newHeight(), this->depth(), this->spectrum());
    for(int y=0; y<averageImage.height(); y++)
        for(int x=0; x<averageImage.width(); x++)
            for(int c=0; c<averageImage.spectrum(); c++)
            {
                int m=i1(x,y,0,c),m2=i2(x,y,0,c);
                averageImage(x,y,0,c)=(m+m2)/2;
            }
    return DisplayedImage(averageImage, "average.png", "Average");
}

/**
* Returns augmented image filling black spaces with the value of the nearest neighbor.
*/
DisplayedImage AugmentedImage::NearestNeighbor()
{
    CImg<unsigned char> expandedImage(this->newWidth(), this->newHeight(), this->depth(), this->spectrum(), 0);
    for(int y=0;y<expandedImage.height();y++)
    {
        for(int x=0;x<expandedImage.width();x++)
        {
            for(int z=0;z<expandedImage.depth();z++)
            {
                for(int c=0;c<expandedImage.spectrum();c++)
                {
                        expandedImage(x,y,z,c)=(*this)(x/incrementX,y/incrementY,z,c);
                }
            }
        }
    }

    return DisplayedImage(expandedImage, "nearestNeighbor.png", "Nearest Neighbor");
}

void AugmentedImage::dumpProps(){
    cout<<"incrementX = "<<this->getIncrementX()<<endl;
    cout<<"incrementY = "<<this->getIncrementY()<<endl;
}

void AugmentedImage::setIncrementX(int incrementX){
    this->incrementX = incrementX;
}

void AugmentedImage::setIncrementY(int incrementY){
    this->incrementY = incrementY;
}

int AugmentedImage::getIncrementX(){
    return this->incrementX;
}

int AugmentedImage::getIncrementY(){
    return this->incrementY;
}