#include <stdio.h>

#include <iomanip>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

using namespace std;

class Alarm {
 public:
  // constructors and destructor
  Alarm() = default;
  Alarm(const string& name, const string& time);
  virtual ~Alarm() = default;

  // other functions
  virtual void Trigger() = 0;
  virtual char AlarmType() = 0;
  virtual int triggertime();

 protected:
  int triggertime_hours_, triggertime_mins_;
  const string name_;

  // member variables are protected to let child classes access them.
};

class RepeatableAlarm : public Alarm {
 public:
  RepeatableAlarm(const string& name, const string& time);
  ~RepeatableAlarm() = default;
  void Trigger() override;
  char AlarmType() override;
};

class SnoozeableAlarm : public Alarm {
 public:
  SnoozeableAlarm(const string& name, const string& time);
  ~SnoozeableAlarm() = default;
  void Trigger() override;
  char AlarmType() override;

 private:
  void Snooze();
};

class AlarmClock {
 public:
  // constructors and destructor
  AlarmClock();
  ~AlarmClock();

  // other functions
  void AddAlarm(Alarm* alarm);
  void TimeElapse();
  constexpr static int kTimeEachDay = 60 * 24;

 private:
  // a container of Alarm*
  vector<Alarm*> alarms_vec_;

  // other member variables
  int currenttime_;
  void ClearAlarm();
};

/**************** Implementations ******************/

Alarm::Alarm(const string& name, const string& time) : name_(name) {
  triggertime_hours_ = (time[0] - '0') * 10 + (time[1] - '0');
  triggertime_mins_ = (time[3] - '0') * 10 + (time[4] - '0');
};
int Alarm::triggertime() { return triggertime_hours_ * 60 + triggertime_mins_; }

RepeatableAlarm::RepeatableAlarm(const string& name, const string& time)
    : Alarm(name, time) {}
void RepeatableAlarm::Trigger() {
  printf("Alarm %s has triggered at %02d:%02d.\n", name_.c_str(),
         triggertime_hours_, triggertime_mins_);
}
char RepeatableAlarm::AlarmType() { return 'R'; }

SnoozeableAlarm::SnoozeableAlarm(const string& name, const string& time)
    : Alarm(name, time) {}
void SnoozeableAlarm::Trigger() {
  printf("Alarm %s has triggered at %02d:%02d. Snooze? (Y/N)\n", name_.c_str(),
         triggertime_hours_, triggertime_mins_);
  char choice;
  cin >> choice;
  if (choice == 'Y') Snooze();
}
void SnoozeableAlarm::Snooze() {
  triggertime_mins_ += 10;
  triggertime_hours_ += triggertime_mins_ / 60;
  triggertime_mins_ %= 60;
}
char SnoozeableAlarm::AlarmType() { return 'S'; }

void AlarmClock::AddAlarm(Alarm* alarm) { alarms_vec_.push_back(alarm); }
void AlarmClock::TimeElapse() {
  for (auto it = alarms_vec_.begin(); it != alarms_vec_.end(); it++)
    if ((*it)->triggertime() == currenttime_) (*it)->Trigger();
  currenttime_++;
  if (currenttime_ == kTimeEachDay) {
    ClearAlarm();
    currenttime_ = 0;
  }
}
void AlarmClock::ClearAlarm() {
  auto it = alarms_vec_.begin();
  while (it != alarms_vec_.end()) {
    if ((*it)->AlarmType() == 'S') {
      delete *it;
      it = alarms_vec_.erase(it);
    } else
      it++;
  }
}
AlarmClock::AlarmClock() : currenttime_(0) {}
AlarmClock::~AlarmClock() {
  for (auto it = alarms_vec_.begin(); it != alarms_vec_.end(); it++) delete *it;
}

/**************** Input function ******************/
void Input(AlarmClock& clock) {
  int n;
  cin >> n;
  for (int i = 0; i < n; i++) {
    string type, name, time;
    cin >> type >> time >> name;
    if (type == "R")
      clock.AddAlarm(new RepeatableAlarm(name, time));
    else if (type == "S")
      clock.AddAlarm(new SnoozeableAlarm(name, time));
    else
      exit(-1);
  }
}
int main() {
  AlarmClock clock;
  for (int days = 0; days < 3; days++) {
    cout << "Do you want to add any alarms?" << endl;
    Input(clock);
    for (int minute = 0; minute < 24 * 60; minute++) {
      clock.TimeElapse();
    }
    cout << "A day has elapsed." << endl;
  }
}
