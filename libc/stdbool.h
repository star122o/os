#ifndef _STDBOOL_H
#define _STDBOOL_H

/* Define boolean type and values */
#define bool _Bool
#define true 1
#define false 0

/* Ensure compatibility with C++ */
#if defined(__cplusplus)
#undef bool
#undef true
#undef false
#define bool bool
#define true true
#define false false
#endif

#endif /* _STDBOOL_H */