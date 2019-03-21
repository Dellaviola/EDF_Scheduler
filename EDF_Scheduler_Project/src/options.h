/*
 * options.h
 *
 *  Created on: Mar 21, 2019
 *      Author: mdellavi
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#define DEBUG 0

#if DEBUG == 1
    #define DPRINTF printf
#else
    #define DPRINTF
#endif

#endif /* OPTIONS_H_ */
