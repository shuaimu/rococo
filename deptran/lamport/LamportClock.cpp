//
// Created by chance_Lv on 15-8-12.
//

#include "LamportClock.h"

unsigned long LamportClock::logicalTime = 0L;

unsigned long LamportClock::sendTimestamp() {
    return incrementAndGet();
}

void LamportClock::updateTime(unsigned long updateTime) {
    long timeDiff = updateTime - logicalTime;
    unsigned long resultTime;
    if (timeDiff < 0) {
        resultTime = incrementAndGet();
    } else {
        resultTime = addAndGet(timeDiff + 1);
    }
}

unsigned long LamportClock::incrementAndGet() {
    return ++logicalTime;
}

unsigned long LamportClock::addAndGet(long timeDelta) {
    logicalTime = logicalTime + timeDelta;
    return logicalTime;
}