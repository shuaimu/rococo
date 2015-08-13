//
// Created by chance_Lv on 15-8-12.
//

#ifndef ROCOCO_LAMPORTCLOCK_H
#define ROCOCO_LAMPORTCLOCK_H

#include <atomic>

/*
 * This class implements lamport timestamp.
 * Each outgoing rpc takes its server's current logical time by
 * calling sendTimestamp;
 * Each incoming rpc, the receiving server call updateTime to
 * update its local logical time based upon the ts in the rpc
 *
 * Each process should create and maintain only one LamportClock
 * object; so the methods are static, and logical time is
 * atomically updated.
 */
class LamportClock {
public:
    static unsigned long sendTimestamp();
    static void updateTime(unsigned long updateTime);
    static unsigned long addAndGet(long timeDelta);

private:
    LamportClock() {}   // make me private, should only instantiate once
    static std::atomic<unsigned long> logicalTime;
    static unsigned long incrementAndGet();
};


#endif //ROCOCO_LAMPORTCLOCK_H
