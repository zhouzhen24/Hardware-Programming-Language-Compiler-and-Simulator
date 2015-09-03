//
//  fifo_scheduler.cpp
//  Cpractice
//
//  Created by Zhen Zhou on 11/19/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "fifo_scheduler.h"


void fifo_scheduler::insert(event_i *event)
{
    events_.push_back(event);
}

void fifo_scheduler::run()
{
    for (; !events_.empty(); events_.pop_front())
    {
        events_.front()->fire(this);
    }
}
