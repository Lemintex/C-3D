#ifndef _OPTIONS_H_
#define _OPTIONS_H_

typedef enum display_type { WIREFRAME, MONOCHROME, TEXTURE } display_type_e;

typedef struct options {
  unsigned short movement_speed;
  display_type_e display_type;
} options_t;

#endif
