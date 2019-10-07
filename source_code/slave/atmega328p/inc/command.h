#ifndef _HEADER_COMMAND_
#define _HEADER_COMMAND_

typedef struct command_t command_t;

struct command_t
{
  union
  {
    short unsigned mix;
    struct
    {
      char unsigned data_type;
      char unsigned command;
    };
  };
};

#endif
