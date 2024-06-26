#ifndef RAYTRACINGINONEWEEKEND_INTERVAL_H
#define RAYTRACINGINONEWEEKEND_INTERVAL_H

#include "config.h"

class Interval {
   public:
    Interval() : min_(+config::infinity), max_(-config::infinity) {}
    Interval(double min, double max) : min_(min), max_(max) {}

    double Min() const { return min_; }
    void Min(const double min) { this->min_ = min; }

    double Max() const { return max_; }
    void Max(const double max) { this->max_ = max; }

    bool Contains(double x) const { return min_ <= x && x <= max_; }
    bool Surrounds(double x) const { return min_ < x && x < max_; }

    inline double Clamp(const double x) const {
        if (x < min_) {
            return min_;
        }

        if (x > max_) {
            return max_;
        }

        return x;
    }

    // create a new interval padded by delta (see section 3.4) from an existing Interval
    inline static Interval Expand(const Interval& interval, const double delta) {
        const auto padding = delta / 2;
        return Interval(interval.Min() - padding, interval.Max() + padding);
    }

   private:
    double min_, max_;

    static const Interval EMPTY, WORLD;
};

const static Interval EMPTY(+config::infinity, -config::infinity);
const static Interval WORLD(-config::infinity, +config::infinity);

#endif  // RAYTRACINGINONEWEEKEND_INTERVAL_H
