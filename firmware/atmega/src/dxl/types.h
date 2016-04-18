/*****************************************************************************/
/* types.h                                                                   */
/* Author : Olivier Ly                                                       */
/* Date : 2007/02                                                            */
/*****************************************************************************/
#ifndef TYPES_H
#define TYPES_H

/*****************************************************************************/
/*! Types                                                                    */
/*****************************************************************************/

/*! \name Base Types */ /*@{*/

/*! \brief Unsigned integer on 8 bits */
typedef unsigned char  ui8; 
/*! \brief Unsigned integer on 16 bits */
typedef unsigned short ui16;
/*! \brief Unsigned integer on 16 bits */
typedef unsigned int   ui32;

#define ui8_max(a,b) ((a<b)?(b):(a))

/*@}*/

#ifdef IAR
#define int32 long int
#endif
#ifdef GCC
#define int32 int
#endif

#endif
/*****************************************************************************/
/*****************************************************************************/
