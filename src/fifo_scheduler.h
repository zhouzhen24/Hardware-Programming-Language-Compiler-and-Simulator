//
//  fifo_scheduler.h
//  Cpractice
//
//  Created by Zhen Zhou on 11/19/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef __Cpractice__fifo_scheduler__
#define __Cpractice__fifo_scheduler__

#include "event_scheduler.h"

#include <list>

class fifo_scheduler : public event_scheduler_i
{
    std::list<event_i *> events_;
    
public:
    void insert(event_i *event);
    void run();
}; // class fifo_scheduler

#endif /* defined(__Cpractice__fifo_scheduler__) */
