#pragma once

#include "cheese.h"
#include <mutex>

/// For printing how much work is done for an operation.
/// The operations are thread-safe so we can safely use this in multi-thread environment.
class ProgressReporter {
public:
    ProgressReporter(uint64_t total_work) : total_work(total_work), work_done(0) {
    }
    void update(uint64_t num) {
        std::lock_guard<std::mutex> lock(mutex);
        work_done += num;
        float work_ratio = (float)work_done / (float)total_work;
        int barWidth = 70;

        std::cout << "[";
        int pos = barWidth * work_ratio;
        for (int i = 0; i < barWidth; ++i)
        {
            if (i < pos)
                std::cout << "=";
            else if (i == pos)
                std::cout << ">";
            else
                std::cout << " ";
        }
        std::cout << "] " << int(work_ratio * 100.0) << " %\r";
        std::cout.flush();
    }
    void done() {
        work_done = total_work;
        fprintf(stdout,
                "\r %.2f Percent Done (%llu / %llu)\n",
                float(100.0),
                (unsigned long long)work_done,
                (unsigned long long)total_work);
    }
    uint64_t get_work_done() const {
        return work_done;
    }

private:
    const uint64_t total_work;
    uint64_t work_done;
    std::mutex mutex;
};
