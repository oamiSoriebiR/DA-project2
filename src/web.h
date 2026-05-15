#ifndef WEB_H
#define WEB_H

#include <vector>
#include "datastructures.h"

bool rangesIntersect(const LiveRange& r1, const LiveRange& r2);
std::vector<Web> webLinking(std::vector<LiveRange> ranges);

#endif //WEB_H