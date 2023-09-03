#ifndef RAYTRACINGINONEWEEKEND_INTERVAL_H
#define RAYTRACINGINONEWEEKEND_INTERVAL_H

#include "config.h"

class Interval {
   public:
    Interval() : min_(+config::infinity), max_(-config::infinity) {}
    Interval(double min, double max) : min_(min), max_(max) {}

    const double min() const { return min_; }
    const double max() const { return max_; }

    bool contains(double x) const { return min_ <= x && x <= max_; }

    bool surrounds(double x) const { return min_ < x && x < max_; }

    inline double clamp(const double x) const {
        if (x < min_) {
            return min_;
        }

        if (x > max_) {
            return max_;
        }

        return x;
    }

   private:
    double min_, max_;

    static const Interval EMPTY, WORLD;
};

const static Interval EMPTY(+config::infinity, -config::infinity);
const static Interval WORLD(-config::infinity, +config::infinity);

#endif  // RAYTRACINGINONEWEEKEND_INTERVAL_H
