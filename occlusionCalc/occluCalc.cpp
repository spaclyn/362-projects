#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <corecrt_math_defines.h> //copilot error fix; error for M_PI without this

struct Rectangle {
    double x, y;       // center
    double width, height;
    double rotation;   // angle in radians
};

struct Interval {
    double start, end;
};

//degrees to radians
double deg2rad(double d) {
    return d * M_PI / 180.0;
}

// angle to [0, 360)
double normalize(double angle) {
    while (angle < 0) angle += 360;
    while (angle >= 360) angle -= 360;
    return angle;
}

int main() {
    srand(time(0));
    int n;
    std::cout << "Enter number of rectangles: ";
    std::cin >> n;

    std::vector<Rectangle> rects;
    for (int i = 0; i < n; i++) {
        Rectangle r;
        r.x = (rand() % 200 - 100);   // random position [-100,100]
        r.y = (rand() % 200 - 100);
        r.width = rand() % 20 + 5;
        r.height = rand() % 20 + 5;
        r.rotation = deg2rad(rand() % 360);
        rects.push_back(r);
    }

    std::vector<Interval> intervals;

    for (auto &r : rects) {
        //compute corners
        double hw = r.width / 2.0;
        double hh = r.height / 2.0;

        std::vector<std::pair<double,double>> corners = {
            { r.x - hw, r.y - hh },
            { r.x + hw, r.y - hh },
            { r.x + hw, r.y + hh },
            { r.x - hw, r.y + hh }
        };

        //Rotate corners
        for (auto &c : corners) {
            double dx = c.first - r.x;
            double dy = c.second - r.y;
            double newx = r.x + dx * cos(r.rotation) - dy * sin(r.rotation);
            double newy = r.y + dx * sin(r.rotation) + dy * cos(r.rotation);
            c.first = newx;
            c.second = newy;
        }
        
        // Print them out so Python can read
        std::cout << "Rectangle corners: ";
        for (auto &c : corners) {
            std::cout << "(" << c.first << "," << c.second << ") ";
        }
        std::cout << std::endl;


        //angles of corners
        double minAngle = 1e9, maxAngle = -1e9;
        for (auto &c : corners) {
            double angle = atan2(c.second, c.first) * 180.0 / M_PI;
            angle = normalize(angle);
            minAngle = std::min(minAngle, angle);
            maxAngle = std::max(maxAngle, angle);
        }

        intervals.push_back({minAngle, maxAngle});
    }

    //sort + merge intervals
    std::sort(intervals.begin(), intervals.end(),
              [](Interval a, Interval b){ return a.start < b.start; });

    double total = 0;
    double currentStart = intervals[0].start;
    double currentEnd = intervals[0].end;

    for (int i = 1; i < intervals.size(); i++) {
        if (intervals[i].start <= currentEnd) {
            //merge
            currentEnd = std::max(currentEnd, intervals[i].end);
        } else {
            total += currentEnd - currentStart;
            currentStart = intervals[i].start;
            currentEnd = intervals[i].end;
        }
    }
    total += currentEnd - currentStart;

    std::cout << "Total occlusion angle = " << total << " degrees\n";
}
