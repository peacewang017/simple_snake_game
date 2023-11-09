#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define snake_speed 1
#define max_snake_length 100
#define max_height 48
#define max_width 64
#define num_apple 2
#define num_snake 1
#define num_air 0
#define stop 0
#define left 1
#define up 2
#define right 3
#define down 4
#define direction_num 4
#define half_direction_num 2
#define board_num ((max_height)*(max_width))
#define apple_init_x 5
#define apple_init_y 3
#define apple_move_x 7
#define apple_move_y 8

#define add_mod_x(x,y) ((((x)+(y))%max_height)==0 ? max_height:(((x)+(y))%max_height))
#define add_mod_y(x,y) ((((x)+(y))%max_width)==0 ? max_width:(((x)+(y))%max_width))

int main() {
    int apple_x = 5;
    int apple_y = 3;
    for(int i=0;i<7;i++){
        apple_x = add_mod_x(apple_x,7);
        apple_y = add_mod_y(apple_y,8);
        printf("%d %d\n",apple_x,apple_y);
    }
    return 0;
}