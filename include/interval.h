#ifndef RAYTRACINGINONEWEEKEND_INTERVAL_H
#define RAYTRACINGINONEWEEKEND_INTERVAL_H

#include "config.h"

class Interval {
   public:
    Interval() : min_(+config::infinity), max_(-config::infinity) {}

    Interval(double min, double max) : min_(min), max_(max) {}

    Interval(const Interval& a, const Interval& b) {
        min_ = a.min_ <= b.min_ ? a.min_ : b.min_;
        max_ = a.max_ >= b.max_ ? a.max_ : b.max_;
    }

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

    inline double GetSize() const { return this->max_ - this->min_; }

    // create a new interval padded by delta (see section 3.4) from an existing Interval
    inline Interval Expand(const double delta) {
        const auto padding = delta / 2;
        return {this->Min() - padding, this->Max() + padding};
    }

   private:
    double min_, max_;
};

inline Interval operator+(const Interval& interval, const double displacement) {
    return Interval{interval.Min() + displacement, interval.Max() + displacement};
}

inline Interval operator+(const double displacement, const Interval& interval) {
    // invoke the above
    return interval + displacement;
}

namespace RTInterval {
const Interval EMPTY(+config::infinity, -config::infinity);
const Interval UNIVERSE(-config::infinity, +config::infinity);
}  // namespace RTInterval

#endif  // RAYTRACINGINONEWEEKEND_INTERVAL_H
