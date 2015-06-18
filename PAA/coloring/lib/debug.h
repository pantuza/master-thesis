/*
 * debug.h
 *
 * @author: Gustavo Pantuza Coelho Pinto
 * @since: 17.05.2013
 *
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#define INFO(x) x
#define DEBUG(x) x
#define DEBUG1(x) 
#define DEBUG2(x) 
#define ASSERT(x) x
#define ASSERT_TRUE(c,x)  ASSERT(if(!(c)){x;})
#define ASSERT_FALSE(c,x) ASSERT(if(c){x;})

#endif /* DEBUG_H_ */
