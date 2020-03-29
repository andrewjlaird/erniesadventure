#include <cstdlib>
#include <iostream>
#include <allegro.h>

#ifndef GLOBAL_TIMER
#define GLOBAL_TIMER

static volatile double process_elapsed_time = 0;
static volatile double process_last_time = 0;
static volatile double process_dt = 0;
static volatile int process_frames_per_second = 0;

static volatile double display_elapsed_time = 0;
static volatile double display_last_time = 0;
static volatile double display_dt = 0;
static volatile int display_frames_per_second = 0;

static void __inc_elapsed_time()
{
   process_elapsed_time += 0.001;
   display_elapsed_time += 0.001;
}
END_OF_FUNCTION(__inc_elapsed_time);

//Understood that this isn't usual object oriented programming
static void setup_timer()
{
   LOCK_VARIABLE(process_elapsed_time);
   LOCK_VARIABLE(display_elapsed_time);
   LOCK_FUNCTION(__inc_elapsed_time);
   install_int_ex(__inc_elapsed_time, BPS_TO_TIMER(1000));
}

#endif
