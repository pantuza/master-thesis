/*
 * debug.h
 *
 * @author: Gustavo Pantuza Coelho Pinto
 * @since: 17.05.2013
 *
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#define INFO(x)
#define DEBUG(x) 
#define DEBUG1(x) 
#define DEBUG2(x) 
#define ASSERT(x)
#define ASSERT_TRUE(c,x)  ASSERT(if(!(c)){x;})
#define ASSERT_FALSE(c,x) ASSERT(if(c){x;})

// Tuning options (Don't touch! It's too dangerous:)

/*
 * Terminates Dijkstra algorithm if it's reach a distance bigger than
 * the shortest path. Great results!
 */
#define OPT_GRAPH_SHORTEST_PATH_BREAK

/*
 * Remove vertices of shortest path from graph to perfect
 * consecutive seam-carving.
 * */
//#define OPT_GRAPH_REMOVE_SHORTEST_PATH

/*
 *  Paints each seam with different color
 */
//#define OPT_IMAGE_SEAM_DIFF_COLORS

#endif /* DEBUG_H_ */
