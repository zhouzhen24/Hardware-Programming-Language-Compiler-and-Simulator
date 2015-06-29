//
//  event_scheduler.h
//  Cpractice
//
//  Created by Zhen Zhou on 11/19/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef __Cpractice__event_scheduler__
#define __Cpractice__event_scheduler__

struct event_scheduler_i;
struct event_i
{
    virtual void fire(event_scheduler_i *scheduler /*, std::string msg */) = 0;//message:msg
}; // struct event_i

struct event_scheduler_i
{
    virtual void insert(event_i *event /*, std::string msg */) = 0;
}; // struct event_scheduler_i

#endif /* defined(__Cpractice__event_scheduler__) */
