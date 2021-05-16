#include <opencv2/opencv.hpp>
#include <sstream>
#include "./structures.hpp"

using namespace cv;

static const int COLOR = 200;
static const int GRAPH_COLOR = 255;
static const int WINDOW_SIZE = 256;
static const int BUTTON_SIZE = 40;
static const int SPACING = 10;

static int displayClass = 0;
static Mat3b canvas; 
static Rect reset;
static vector<stock> stocks;
static bool* runProgramPtr = NULL;

void loopUI(){

    // Prepare to redraw
    canvas(reset) = Vec3b(0, 0, 0);
    stock dispStock = stocks[displayClass];
    float maxVal = dispStock.getCurrentMaxval();
    float minVal = dispStock.getCurrentMinval(maxVal);
    Scalar pixColor = Scalar(COLOR, COLOR, COLOR);
    Scalar graphColor = Scalar(0, GRAPH_COLOR, 0);

    // Calculate axes and resolutions
    int leftX = BUTTON_SIZE + 4 * SPACING;
    int rightX = BUTTON_SIZE + WINDOW_SIZE - SPACING;
    int topY = SPACING;
    int bottomY = WINDOW_SIZE-SPACING;
    int graphSize = bottomY - topY;
    int dX = graphSize/dispStock.maxSize;
    int dY = (maxVal - minVal)/5;

    // Draw
    line(canvas, Point(leftX, topY), Point(leftX, bottomY), pixColor);
    line(canvas, Point(leftX, bottomY), Point(rightX, bottomY), pixColor);

    // Add labels to y-axis
    stringstream stream;
    int labelVal = minVal;
    int labelY = bottomY;
    for (int label = 0; label < 6; ++label){
        stream << std::fixed << std::setprecision(2) << labelVal;
        putText(canvas, stream.str(), Point(leftX-2*SPACING, labelY), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(COLOR, COLOR, COLOR));
        stream.str("");

        labelVal += dY;
        labelY = bottomY - graphSize * labelVal/maxVal ; 
    }
    

    // Calc first point manually
    float price = dispStock.data[0];
    int xLast = leftX;
    int yLast = bottomY - graphSize * price/maxVal ; 
    
    int cursor;
    for (int i = 0; i < dispStock.maxSize; ++i){
        cursor = (dispStock.startPtr + i) % dispStock.maxSize;
        price = dispStock.data[cursor];

        if (price == 0){
            break;
        }

        int xNew = xLast + dX;
        int yNew = bottomY - graphSize * price/maxVal;

        line(canvas, Point(xLast, yLast), Point(xNew, yNew), graphColor);

        xLast = xNew;
        yLast = yNew;
    }

    imshow("display", canvas);
    int c = waitKey(10000);
    if (c == 27) *runProgramPtr = false; destroyAllWindows();
}

void onMouseClick(int event, int x, int y, int flags, void* userInp){
    if (event == EVENT_LBUTTONDOWN){
        int pixel = (int)canvas.at<Vec3b>(y, x)[0];
        if (pixel < 7 && pixel > 0){
            displayClass = pixel - 1;
            destroyAllWindows();
            loopUI();
        }
    }
}

void initUI(vector<stock>& stocks_, bool& runProgram){ 

    canvas = Mat3b(WINDOW_SIZE, WINDOW_SIZE+BUTTON_SIZE+3*SPACING, Vec3b(0,0,0));
    reset = Rect(BUTTON_SIZE+SPACING, 0, WINDOW_SIZE+2*SPACING, WINDOW_SIZE);
    stocks = stocks_;
    runProgramPtr = &runProgram;

    Rect sideBar = Rect(0, 0, BUTTON_SIZE+SPACING, WINDOW_SIZE);
    canvas(sideBar) = Vec3b(COLOR, COLOR, COLOR);

    // Max 6 items
    int displacement = WINDOW_SIZE/6;
    int y;

    for (int i = 0; i < stocks.size(); ++i){
        y = displacement*i+SPACING;
        Rect button = Rect(5, y, BUTTON_SIZE, BUTTON_SIZE);

        // Hide the button ID in the pixel data
        // +1 to avoid mistaking the background as a class
        canvas(button) = Vec3b(i+1,i+1,i+1);
        putText(canvas, stocks[i].name, Point(SPACING, y+button.height/2), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(COLOR, COLOR, COLOR));
    }

    namedWindow("display");
    setMouseCallback("display", onMouseClick);
    imshow("display", canvas);
    waitKey(1);
}