#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>
#include <iomanip>
#include <locale>
#include <codecvt>

using namespace std;

const double PI = 3.14159265358979323846;
const double TWO_PI = 2.0 * PI;

struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
};

struct Rectangle {
    Point center;
    double width, height;
    double rotation; // in radians
    double distance; // from origin
    
    Rectangle(Point c, double w, double h, double rot, double dist)
        : center(c), width(w), height(h), rotation(rot), distance(dist) {}
};

// Convert Cartesian coordinates to polar angle (0 to 2π)
double cartesianToAngle(double x, double y) {
    double angle = atan2(y, x);
    return angle < 0 ? angle + TWO_PI : angle;
}

// Generate random rectangle
Rectangle generateRandomRectangle(mt19937& gen, int n) {
    uniform_real_distribution<> pos_dist(-100.0, 100.0);
    uniform_real_distribution<> size_dist(5.0, 20.0);
    uniform_real_distribution<> rot_dist(0.0, TWO_PI);
    uniform_real_distribution<> dist_dist(50.0, 200.0);
    
    Point center(pos_dist(gen), pos_dist(gen));
    double width = size_dist(gen);
    double height = size_dist(gen);
    double rotation = rot_dist(gen);
    double distance = dist_dist(gen);
    
    return Rectangle(center, width, height, rotation, distance);
}

// Calculate interval for a rectangle
pair<double, double> getRectangleAngularInterval(const Rectangle& rect) {
    vector<Point> vertices;
    
    // Calculate rectangle vertices
    double half_w = rect.width / 2.0;
    double half_h = rect.height / 2.0;
    
    vector<Point> local_vertices = {
        Point(-half_w, -half_h),
        Point(half_w, -half_h),
        Point(half_w, half_h),
        Point(-half_w, half_h)
    };
    
    //Transform vertices to coordinates
    for (const auto& vertex : local_vertices) {
        //Rotate
        double x_rot = vertex.x * cos(rect.rotation) - vertex.y * sin(rect.rotation);
        double y_rot = vertex.x * sin(rect.rotation) + vertex.y * cos(rect.rotation);
        
        //Translate
        double x_global = rect.center.x + x_rot;
        double y_global = rect.center.y + y_rot;
        
        vertices.emplace_back(x_global, y_global);
    }
    
    //Calculate angles for all vertices
    vector<double> angles;
    for (const auto& vertex : vertices) {
        angles.push_back(cartesianToAngle(vertex.x, vertex.y));
    }
    
    //Find min and max
    double min_angle = *min_element(angles.begin(), angles.end());
    double max_angle = *max_element(angles.begin(), angles.end());
    
    //Handle wrap-around
    if (max_angle - min_angle > PI) {
        //If rectangle spans more than 180 degrees, handles carefully
        return make_pair(0.0, TWO_PI);
    }
    
    return make_pair(min_angle, max_angle);
}

//Merge overlapping intervals and calculate total coverage
double mergeIntervals(vector<pair<double, double>>& intervals) {
    if (intervals.empty()) return 0.0;
    
    //Sort intervals by start angle
    sort(intervals.begin(), intervals.end());
    
    vector<pair<double, double>> merged;
    merged.push_back(intervals[0]);
    
    for (size_t i = 1; i < intervals.size(); i++) {
        auto& current = intervals[i];
        auto& last = merged.back();
        
        if (current.first <= last.second) {
            // Intervals overlap, merge them
            last.second = max(last.second, current.second);
        } else {
            // No overlap, add new interval
            merged.push_back(current);
        }
    }
    
    // Calculate total coverage
    double total_coverage = 0.0;
    for (const auto& interval : merged) {
        total_coverage += interval.second - interval.first;
    }
    
    return total_coverage;
}

int main() {
    int n;
    cout << "Enter number of rectangles (n): ";
    cin >> n;
    
    //random number generator
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 gen(seed);
    
    //Generate random rectangles
    vector<Rectangle> rectangles;
    for (int i = 0; i < n; i++) {
        rectangles.push_back(generateRandomRectangle(gen, n));
    }
    
    //Calculate angular intervals for all rectangles
    vector<pair<double, double>> intervals;
    for (const auto& rect : rectangles) {
        auto interval = getRectangleAngularInterval(rect);
        intervals.push_back(interval);
    }
    
    // Merge intervals and calculate total visible angle
    double total_visible_rad = mergeIntervals(intervals);
    double total_visible_deg = total_visible_rad * 180.0 / PI;
    
    // Calculate occlusion angle (what's NOT visible)
    double occlusion_angle = 360.0 - total_visible_deg;
    
    // Ensure occlusion angle is in [0, 359]
    occlusion_angle = max(0.0, min(359.0, occlusion_angle));
    
    // Output results with ASCII degree symbol to avoid encoding issues
    cout << "\n=== OCCLUSION CALCULATION RESULTS ===" << endl;
    cout << "Number of rectangles: " << n << endl;
    cout << "Total visible angle: " << fixed << setprecision(2) << total_visible_deg << " degrees" << endl;
    cout << "Occlusion angle: " << fixed << setprecision(2) << occlusion_angle << " degrees" << endl;
    
    return 0;
}