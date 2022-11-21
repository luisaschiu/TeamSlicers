/** @file shares.h
 *  This file contains extern declarations of shares and queues which are used
 *  in more than one file of a demonstration project.
 * 
 *  @author JR Ridgely
 *  @date   2021-Oct-23 Original file
 *  @copyright (c) 2021 by JR Ridgely, released under the LGPL 3.0. 
 */

#ifndef _shares_h_
#define _shares_h_

//#include "taskqueue.h"
#include "taskshare.h"

// A share which holds a counter of how many times a simulated event occurred
extern Share<float> distance;


#endif // _shares_h_