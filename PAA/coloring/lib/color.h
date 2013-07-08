
/*
 * color.h
 *
 *  Created on: 02/06/2013
 *      Author: Gustavo Pantuza
 */

#ifndef COLOR_H_
#define COLOR_H_

#define BASIC_COLORS 28
#define COLOR_BUFFER_SIZE 32

namespace Graph {

    typedef struct
    {
        int r, g, b;
    } RGB;

    static RGB color[BASIC_COLORS] = {
            {0,0,0},
            {0,0,255},
            {0,255,0},
            {255,0,0},
            {255,255,0},
            {255,0,255},
            {0,255,255},
            {0,0,128},
            {0,128,0},
            {128,0,0},
            {128,128,0},
            {128,0,128},
            {0,128,128},
            {128,128,128},
            {0,0,192},
            {0,192,0},
            {192,0,0},
            {192,192,0},
            {192,0,192},
            {0,192,192},
            {192,192,192},
            {0,0,64},
            {0,64,0},
            {64,0,0},
            {64,64,0},
            {64,0,64},
            {0,64,64},
            {64,64,64}
    };

    class Color {
        private:
            char buffer[COLOR_BUFFER_SIZE];
        public:
            inline int isBasic(int max)
            {
                return max < BASIC_COLORS;
            }

            inline char *get(int max, int i)
            {
                if (!i || max < BASIC_COLORS)
                    sprintf(buffer, "#%.2X%.2X%.2X",
                            color[i].r,
                            color[i].g,
                            color[i].b);
                else
                    sprintf(buffer, "%f 1.0 1.0", (float)max / (float)i);
                return buffer;
            }
    };
}

#endif /* COLOR_H_ */
