#include "Builder.h"





int main( int argc, char ** argv )
{
    Builder * builder = new Builder() ;
    builder->launch_game() ;
    delete builder ;
    
    return 0;
}
