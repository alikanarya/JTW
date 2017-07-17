#ifndef IMGTOOLS_H
#define IMGTOOLS_H

#include <QImage>
#include "math.h"

template<class T>
inline const T& kClamp( const T& x, const T& low, const T& high )
{
    if ( x < low )       return low;
    else if ( high < x ) return high;
    else                 return x;
}

int changeBrightness( int value, int brightness );
int changeContrast( int value, int contrast );
int changeGamma( int value, int gamma );
int changeUsingTable( int value, const int table[] );
template< int operation( int, int ) >
static
QImage changeImage( const QImage& image, int value );


// brightness is multiplied by 100 in order to avoid floating point numbers
QImage changeBrightness( const QImage& image, int brightness );

// contrast is multiplied by 100 in order to avoid floating point numbers
QImage changeContrast( const QImage& image, int contrast );

// gamma is multiplied by 100 in order to avoid floating point numbers
QImage changeGamma( const QImage& image, int gamma );

#endif // IMGTOOLS_H
