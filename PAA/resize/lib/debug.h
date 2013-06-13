/*
 * debug.h
 *
 *  Created on: 12/06/2013
 *      Author: Gustavo Pantuza
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#define DEBUG(x) 
#define DEBUG1(x) 
#define DEBUG2(x) 
#define ASSERT(x) 
#define ASSERT_TRUE(c,x) ASSERT(if(c){x;})

// Tuning options (Don't touch! It's too dangerous:)

//#define OPT_GRAPH_IGNORE_CARVING
// Surprisingly, this increases the runtime... Why?

// About 5..50% off runtime!
#define OPT_GRAPH_SHORTEST_PATH_BREAK

#endif /* DEBUG_H_ */
