#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

#ifndef bool
#define bool uint8_t
#endif
    
#include "BML_DEF.h"

typedef struct {
    uint32_t ir;
    uint32_t red;
} rb_data_type ;

extern uint16_t             buf_size  ;
extern uint16_t             buf_count ;

void rb_init(void) ;
void rb_destruct(void) ;

/**
 * Add an element to the beginning of the buffer:
 * @returns: 0 if the addition caused overwriting an existing element.
 */
bool                rb_unshift(rb_data_type value) ;

/**
 * Add an element to the end of the buffer:
 * @returns: 0 if the addition caused overwriting an existing element.
 */
bool                rb_push(rb_data_type value) ;

/**
 * Removes an element from the beginning of the buffer.
*/
rb_data_type        rb_shift(void) ;

/**
 * Removes an element from the end of the buffer.
 */
rb_data_type        rb_pop(void) ;

/**
 * Returns the element of the beginnig of the buffer.
 */
rb_data_type  rb_first(void) ;

/**
 * Returns the element of the end of the buffer.
 */
rb_data_type  rb_last(void) ;

/**
 * In C++ [] operator
 */
rb_data_type        rb_data_at(uint16_t index) ;

/**
 * Returns how many elements are actually stored in the buffer.
 */
static uint16_t     inline rb_size(void) { return( buf_count ) ; }

/**
 * Returns how many elements can be saferly pushed into the buffer.
 */
uint16_t     inline rb_available(void) { return( buf_size - buf_count ) ; }

/**
 * Returns how many elements can be potentially stored in the buffer.
 */
uint16_t     inline rb_capacity(void) { return( buf_size ) ; }

/**
 * Returns 1 if no elements can be removed from the buffer.
 */
bool         inline rb_is_empty(void) { return( buf_count == 0 ) ; }

/**
 * Return 1 if no elements can be added the the buffer without overwriting existing elements.
 */
bool         inline rb_is_full(void) { return( buf_count == buf_size ) ; }

/**
 * Resets the buffer to a clean status, droppig any reference to current elements
 * and making all buffer position available again.
 */
void          rb_clear(void) ;









#endif /* _RING_BUFFER_H_ */
/* [] END OF FILE */
