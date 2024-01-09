#ifndef __KMEANS_HPP_
#define __KMEANS_HPP_

#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

using index_t = int;

struct Point {
  double x, y;

  Point() : x(0), y(0){};
  Point(double x_, double y_) : x(x_), y(y_) {}
  Point(const Point& other) : x(other.x), y(other.y) {}

  // operator
  void operator+=(const Point& that);

  inline double Distance(Point& other);
};

class Kmeans {
 public:
  Kmeans(const std::vector<Point>& points,
         const std::vector<Point>& initialCenters);
  ~Kmeans();
  std::vector<index_t> Run(int maxIterations = 1000);

 private:
  std::vector<Point> points_;
  std::vector<Point> centers_;
  int num_points_;
  int num_centers_;
  bool continue_;
  void UpdatePointsCluster(std::vector<index_t>& clusters_numbers, int left,
                           int right, std::vector<Point>& total_center,
                           std::vector<int>& times);
  void UpdateCenters(
      const std::vector<std::vector<Point>>& totalcenter_eachthread,
      const std::vector<std::vector<int>> pointstime_eachthread);
  // Thread Pool
  int num_thread_;
  unsigned int processing_;
  bool stop_thread_;
  std::mutex queue_mutex_, processing_mutex_;
  std::condition_variable condition_, finished_;
  std::vector<std::thread> threads_;
  std::queue<std::function<void()>> jobqueue_;
  void CreateThreadPool();
  void DestroyThreadPool();
};

std::istream& operator>>(std::istream& is, Point& pt);
std::ostream& operator<<(std::ostream& os, Point& pt);

#endif  // __KMEANS_HPP_