#include "kmeans.hpp"

const int kTHREAD_NUM = 19;

inline double Point::Distance(Point& other) {
  return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y);
}
void Point::operator+=(const Point& that) {
  x += that.x;
  y += that.y;
}

std::istream& operator>>(std::istream& is, Point& pt) {
  is >> pt.x >> pt.y;
  return is;
}
std::ostream& operator<<(std::ostream& os, Point& pt) {
  os << pt.x << " " << pt.y;
  return os;
}

Kmeans::Kmeans(const std::vector<Point>& points,
               const std::vector<Point>& initialCenters)
    : points_(points),
      centers_(initialCenters),
      num_points_(points.size()),
      num_centers_(initialCenters.size()),
      continue_(false),
      num_thread_(kTHREAD_NUM),
      processing_(0),
      stop_thread_(false) {
  CreateThreadPool();
}
Kmeans::~Kmeans() { DestroyThreadPool(); }

void Kmeans::UpdatePointsCluster(std::vector<index_t>& clusters_numbers,
                                 int left, int right,
                                 std::vector<Point>& total_center,
                                 std::vector<int>& times) {
  bool changed = false;
  for (int i = left; i < right; ++i) {
    double min_value = points_[i].Distance(centers_[0]);
    int min_cluster = 0;
    for (int j = 1; j < num_centers_; ++j) {
      double rival_value = points_[i].Distance(centers_[j]);
      if (rival_value < min_value) {
        min_value = rival_value;
        min_cluster = j;
      }
    }
    if (clusters_numbers[i] != min_cluster) {
      clusters_numbers[i] = min_cluster;
      changed = true;
    }
    total_center[min_cluster] += points_[i];
    ++times[min_cluster];
  }
  if (changed) continue_ = true;
}
void Kmeans::UpdateCenters(
    const std::vector<std::vector<Point>>& totalcenter_eachthread,
    const std::vector<std::vector<int>> pointstime_eachthread) {
  for (int i = 0; i < num_centers_; ++i) {
    int time = 0;
    Point new_center = Point();
    for (int j = 0; j < num_thread_; ++j) {
      new_center += totalcenter_eachthread[j][i];
      time += pointstime_eachthread[j][i];
    }
    if (time) {
      centers_[i].x = new_center.x / time;
      centers_[i].y = new_center.y / time;
    }
  }
}
std::vector<index_t> Kmeans::Run(int maxIterations) {
  // DO NOT MODIFY THESE CODE
  std::vector<index_t> assignment(num_points_, 0);  // the return vector
  int currIteration = 0;
  std::cout << "Running kmeans with num points = " << num_points_
            << ", num centers = " << num_centers_
            << ", max iterations = " << maxIterations << "...\n";
  // YOUR CODE HERE
  std::vector<std::thread> threads(num_thread_);
  for (currIteration = 0; currIteration < maxIterations; ++currIteration) {
    continue_ = false;
    std::vector<std::vector<Point>> totalcenter_eachthread(
        num_thread_, std::vector<Point>(num_centers_));
    std::vector<std::vector<int>> pointstime_eachthread(
        num_thread_, std::vector<int>(num_centers_));
    {
      std::unique_lock<std::mutex> lock(queue_mutex_);
      jobqueue_.emplace(std::bind(
          &Kmeans::UpdatePointsCluster, this, std::ref(assignment),
          (num_points_ / num_thread_) * (num_thread_ - 1), num_points_,
          std::ref(totalcenter_eachthread[num_thread_ - 1]),
          std::ref(pointstime_eachthread[num_thread_ - 1])));
      condition_.notify_one();
      for (int i = 0; i < num_thread_ - 1; ++i) {
        jobqueue_.emplace(std::bind(&Kmeans::UpdatePointsCluster, this,
                                    std::ref(assignment),
                                    (num_points_ / num_thread_) * i,
                                    (num_points_ / num_thread_) * (i + 1),
                                    std::ref(totalcenter_eachthread[i]),
                                    std::ref(pointstime_eachthread[i])));
        condition_.notify_one();
      }
      finished_.wait(lock,
                     [this]() { return jobqueue_.empty() && !processing_; });
    }
    UpdateCenters(totalcenter_eachthread, pointstime_eachthread);
    if (!continue_) break;
  }

  std::cout << "Finished in " << currIteration << " iterations." << std::endl;
  return assignment;
}

// Thread Pool
void Kmeans::CreateThreadPool() {
  for (int i = 0; i < num_thread_; ++i) {
    threads_.emplace_back([this] {
      while (true) {
        std::function<void()> task;
        {
          std::unique_lock<std::mutex> lock(queue_mutex_);
          condition_.wait(
              lock, [this] { return stop_thread_ || !jobqueue_.empty(); });
          if (stop_thread_ && jobqueue_.empty()) return;
          task = std::move(jobqueue_.front());
          jobqueue_.pop();
        }
        {
          std::unique_lock<std::mutex> lock(processing_mutex_);
          ++processing_;
        }
        task();
        {
          std::unique_lock<std::mutex> lock(processing_mutex_);
          --processing_;
        }
        finished_.notify_one();
      }
    });
  }
}
void Kmeans::DestroyThreadPool() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    stop_thread_ = true;
  }
  condition_.notify_all();
  for (auto& t : threads_) t.join();
}
