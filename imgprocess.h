#ifndef IMGPROCESS_H
#define IMGPROCESS_H

#include "messages.h"
#include <QImage>
#include <QColor>

#define PI 3.14159265
#define R2D PI/180          // convert radyant to degree

const int sobelX[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
const int sobelY[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};

// class for the void(empty) line data
class voidLine{
    public:
        QPoint start;
        QPoint end;
        int length;

        voidLine(){}
};

// image processing class
class imgProcess{

    public:
        QImage imgOrginal;      // orginal image
        QImage imgMono;         // mono image
        QImage imgCorner;       // otginal image with detected corners shown

        const int imageWidth, imageHeight;  // org. image
        int edgeWidth, edgeHeight;          // edge detected image

        // hough transform vars
        int houghDistanceMax;               // max. hough distance, depends edge image size and center point
        int houghThetaSize;                 // hough search angle size, depends min/max/resolution of angle
        int centerX, centerY;               // search origin
        int thetaMin, thetaMax;             // search angles in between
        float thetaStep;                    // angle resolution

        // corner detection parameters
        int houghLineNo;                    // no. of max voted lines concerned
        int houghVoteAvg;                   // ave vote value of max. voted lines
        int voteThreshold;                  // vote threshold to accept primary line as concerned
        int voidThreshold;                  // void line length threhold in pixels to accept primary void line as concerned
        bool primaryLineDetected;           // true: primary line detected
        bool cornersDetected;               // true: corners detected
        int voidIndex;                      // index no of <void space list> holding primary void line data
        int trackCenterX, trackCenterY;     // coor. of center beteen corners
        QList<voidLine *> voidSpace;        // list to hold found void lines

        int **valueMatrix;                  // image data matrix
        int **edgeMatrix;                   // edge image data matrix
        int **houghMatrix;                  // hough image data matrix with max. voted lines coded

        int **houghSpace;                   // line votes: line search matrix, depends max. distance & angle scale
        float **houghLines;                 // line data of max. voted lines; distance/angle/vote value

        int *histogram;                     // histogram array

        // user information
        QString statusMessage;              // general message var about processing
        bool detected;                      // false for any known un-matched criteria
        int angleAvg;                       // average angle degree wrt center point (-90)
        bool angleInLimit;                  // true if <avgAngle> is within +/- 3 degree


        // constructor
        imgProcess(QImage &image, const int width, const int height) : imageWidth(width), imageHeight(height) {
            imgOrginal = image;     // passes image to class not copy of it
            valueMatrix = new int*[height];
            for (int i = 0; i < height; i++) valueMatrix[i] = new int[width];

            edgeWidth = imageWidth - 2;
            edgeHeight = imageHeight - 2;

            edgeMatrix = new int*[edgeHeight];
            for (int i = 0; i < edgeHeight; i++)   edgeMatrix[i] = new int[edgeWidth];

            houghMatrix = new int*[edgeHeight];
            for (int i = 0; i < edgeHeight; i++) houghMatrix[i] = new int[edgeWidth];

            thetaMin = 80;
            thetaMax = 100;
            thetaStep = 1.0;
            houghLineNo = 5;
            houghVoteAvg = 0;
            voteThreshold = 200;
            voidThreshold = 30;
            primaryLineDetected = false;
            cornersDetected = false;
            voidIndex = 0;
            trackCenterX = -1;
            trackCenterY = -1;

            statusMessage = message0;
            angleAvg = 0;
            angleInLimit = false;

        }

        void toMono();      // produce mono image
        void constructValueMatrix(QImage image);    // construct pixel value matrix of an image according to single color value
        int getMatrixPoint(int *matrix, int width, int x, int y);   // returns value of a matrix

        // saves a int matrix with given filename
        // saving with X and _ pointers (not used)
        bool saveMatrix(int **matrix, int width, int height, QString fname, int threshold = 255, bool xSwitch = false);

        bool saveMatrix(float **matrix, int width, int height, QString fname);  // saves a float matrix with given filename
        bool saveArray(int *array, int length, QString fname);  // saves a int array with given filename

        void detectEdgeSobel();                 // detect edges & construct edge matrix
        void houghTransform();                  // conduct hough transform & construct hough space matrix
        void calculateHoughMaxs(int number);    // copy data of <number> of max voted lines to hough lines matrix
        void constructHoughMatrix();            // construct hough matrix base on edge matrix with max voted lines coded on it
        int calcVoteAvg();                      // calc. vote ave. of max. voted lines
        int calcAngleAvg();                     // calc. vote ave. angle max. voted lines wrt center (-90)
        bool checkPrimaryLine();                // check if primary line found is above <voteThreshold>
        void detectVoidLines();                 // detect void lines on max. voted lines imposed on mono image

        void detectPrimaryVoid();               // detect primay void line using <voidThreshold>,
                                                // finds index of primary void in <voidSpace>, calculates center point of corners
                                                // LASER LINE MUST BE ALIGN TO WHOLE IMAGE AND VOID SPACE IN INTEREST
                                                // MUST BE BIGGEST VOID SPACE THROUGHOUT THE LINE

        // produces image from matrix. if hough line code is included in, dras lines with RED
        QImage* getImage(int **matrix, int width, int height, QImage::Format format = QImage::Format_RGB32);

        int getLineY(int x, float distance, float theta);   // get hough line Y coor from X coor
        int* edgeSobelHistogram();                          // produce edge matrix Y histogram accor. X values
        int* valueHistogram();                              // produce value matrix Y histogram accor. X values
        QImage cornerImage();                               // produce detected corner image based on org. image

        ~imgProcess();                                      // destructor
};
#endif // IMGPROCESS_H
