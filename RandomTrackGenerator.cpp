#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

class Point
{
  public:
    double x;
    double y;
    Point()
    {
        x = 0;
        y = 0;
    }
    Point(float X, float Y)
    {
        x = X;
        y = Y;
    }
};

double bezier3funcX(double t, Point *controlP)
{
    double part0 = controlP[0].x * (1 - t) * (1 - t) * (1 - t);
    double part1 = 3 * controlP[1].x * (1 - t) * (1 - t) * t;
    double part2 = 3 * controlP[2].x * (1 - t) * t * t;
    double part3 = controlP[3].x * t * t * t;
    return part0 + part1 + part2 + part3;
}

double bezier3funcY(double t, Point *controlP)
{
    double part0 = controlP[0].y * (1 - t) * (1 - t) * (1 - t);
    double part1 = 3 * controlP[1].y * (1 - t) * (1 - t) * t;
    double part2 = 3 * controlP[2].y * (1 - t) * t * t;
    double part3 = controlP[3].y * t * t * t;
    return part0 + part1 + part2 + part3;
}

double dx(double t, Point *controlP)
{
    double part0 = controlP[0].x * (-3 * t * t + 6 * t - 3);
    double part1 = 3 * controlP[1].x * (3 * t * t - 4 * t + 1);
    double part2 = 3 * controlP[2].x * (2 * t - 3 * t * t);
    double part3 = controlP[3].x * 3 * t * t;
    return part0 + part1 + part2 + part3;
}

double dy(double t, Point *controlP)
{
    double part0 = controlP[0].y * (-3 * t * t + 6 * t - 3);
    double part1 = 3 * controlP[1].y * (3 * t * t - 4 * t + 1);
    double part2 = 3 * controlP[2].y * (2 * t - 3 * t * t);
    double part3 = controlP[3].y * 3 * t * t;
    return part0 + part1 + part2 + part3;
}

double bezier3theta(double t, Point *controlP)
{
    double dX    = dx(t, controlP);
    double dY    = dy(t, controlP);
    double theta = std::atan2(dY, dX);
    return theta;
}

void createCurve(Point *originPoint, int originCount,
                 std::vector<Point> &curvePoint, std::vector<Point> &left,
                 std::vector<Point> &right)
{
    double scale     = 0.6;
    Point *midpoints = new Point[originCount];
    for (int i = 0; i < originCount; i++)
        {
            int nexti      = (i + 1) % originCount;
            midpoints[i].x = (originPoint[i].x + originPoint[nexti].x) / 2.0;
            midpoints[i].y = (originPoint[i].y + originPoint[nexti].y) / 2.0;
        }

    Point *extrapoints = new Point[2 * originCount];
    for (int i = 0; i < originCount; i++)
        {
            int   nexti = (i + 1) % originCount;
            int   backi = (i + originCount - 1) % originCount;
            Point midinmid;
            midinmid.x        = (midpoints[i].x + midpoints[backi].x) / 2.0;
            midinmid.y        = (midpoints[i].y + midpoints[backi].y) / 2.0;
            double offsetx    = originPoint[i].x - midinmid.x;
            double offsety    = originPoint[i].y - midinmid.y;
            int    extraindex = 2 * i;
            extrapoints[extraindex].x = midpoints[backi].x + offsetx;
            extrapoints[extraindex].y = midpoints[backi].y + offsety;
            double addx =
                (extrapoints[extraindex].x - originPoint[i].x) * scale;
            double addy =
                (extrapoints[extraindex].y - originPoint[i].y) * scale;
            extrapoints[extraindex].x = originPoint[i].x + addx;
            extrapoints[extraindex].y = originPoint[i].y + addy;

            int extranexti            = (extraindex + 1) % (2 * originCount);
            extrapoints[extranexti].x = midpoints[i].x + offsetx;
            extrapoints[extranexti].y = midpoints[i].y + offsety;
            addx = (extrapoints[extranexti].x - originPoint[i].x) * scale;
            addy = (extrapoints[extranexti].y - originPoint[i].y) * scale;
            extrapoints[extranexti].x = originPoint[i].x + addx;
            extrapoints[extranexti].y = originPoint[i].y + addy;
        }

    Point *controlPoint = new Point[4];
    for (int i = 0; i < originCount; i++)
        {
            controlPoint[0] = originPoint[i];
            int extraindex  = 2 * i;
            controlPoint[1] = extrapoints[extraindex + 1];
            int extranexti  = (extraindex + 2) % (2 * originCount);
            controlPoint[2] = extrapoints[extranexti];
            int nexti       = (i + 1) % originCount;
            controlPoint[3] = originPoint[nexti];
            double u        = 0;
            while (u <= 1)
                {
                    double px = bezier3funcX(u, controlPoint);
                    double py = bezier3funcY(u, controlPoint);
                    u += 0.1;
                    Point tempP = Point(px, py);
                    curvePoint.push_back(tempP);
                }

            for (double j = 0.1; j < 0.7; j += 0.5)
                {
                    double cone_x     = bezier3funcX(j, controlPoint);
                    double cone_y     = bezier3funcY(j, controlPoint);
                    double cone_theta = bezier3theta(j, controlPoint);
                    Point  tmp;
                    tmp.x = cone_x + 2 * cos(cone_theta + 1.57);
                    tmp.y = cone_y + 2 * sin(cone_theta + 1.57);
                    left.push_back(tmp);
                    tmp.x = cone_x + 2 * std::cos(cone_theta - 1.57);
                    tmp.y = cone_y + 2 * std::sin(cone_theta - 1.57);
                    right.push_back(tmp);
                }
        }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        {
            std::cout << "usage: " << argv[0] << " path_file" << std::endl;
            exit(-1);
        }

    srand((int)time(NULL));
    Point *control = new Point[36];
    double start   = double(rand()) / RAND_MAX * 20 + 20;
    double current = start;
    double dec     = 2;
    for (int i = 0; i < 36; i++)
        {
            double angle = 10 * (double)i * 3.14 / 180;
            Point  tmp;
            if (rand() < 0.5 * RAND_MAX)
                current = current - dec > 10 ? current - dec : 10;
            else
                current = current + dec < 30 ? current + dec : 30;
            tmp.x      = 35 + current * std::cos(angle);
            tmp.y      = 35 + current * std::sin(angle);
            control[i] = tmp;
        }

    std::vector<Point> curvePoint;
    std::vector<Point> left;
    std::vector<Point> right;
    createCurve(control, 36, curvePoint, left, right);
    std::ofstream outfile;
    // write your target path here!
    // outfile.open("/home/lucy/Desktop/path.txt");
    outfile.open(argv[1]);
    for (int i = 0; i < curvePoint.size(); ++i)
        outfile << curvePoint[i].x << " " << curvePoint[i].y << std::endl;
    outfile.close();
    return 0;
}
