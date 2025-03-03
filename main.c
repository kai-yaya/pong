#include <SDL.h>
#include <stdio.h>
#define FPS 90
#define MOVE_SPEED 5

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float FRAME_TARGET_TIME = 1000 / FPS;
SDL_Window * window = NULL;
SDL_Renderer * renderer = NULL;
int is_game_running = 0;
int last_frame_time = 0;
int ball_moving_right = -1; // 1 is moving right, -1 is moving left
int ball_moving_up = 1; // 1 is moving up, -1 us moving down
int ball_speed_vertical = 3;
int platform_height = 90;
int platform_width = 7;
int ball_size = 6;


struct object{
    int x;
    int y;
    int width;
    int height;
}objLeft, objRight, objBall;


int init(void);
void setup(void);
void logic(void);
void render(void);
void exit(void);
void moveUp(struct object * obj, int speed);
void moveDown(struct object * obj, int speed);
void ballMovement(struct object * obj, const SDL_Rect *, const SDL_Rect *, const SDL_Rect *);
void resetBallPos();


int main(int argc, char * argv[]){
    is_game_running = init();

    setup();

    render();
    resetBallPos();

    while(is_game_running){
        logic();
        render();
    }

    exit();

    return 0;
}


void setup(void){
    objLeft.x = WINDOW_WIDTH * 0.1;
    objLeft.y = WINDOW_HEIGHT/2 - platform_height/2;
    objLeft.height = platform_height;
    objLeft.width = platform_width;

    objRight.x = WINDOW_WIDTH * 0.9;
    objRight.y = WINDOW_HEIGHT/2 - platform_height/2;
    objRight.height = platform_height;
    objRight.width = platform_width;

    objBall.x = WINDOW_WIDTH/2;
    objBall.y = WINDOW_HEIGHT/2;
    objBall.height = ball_size;
    objBall.width = ball_size;
}


int init(void){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("Error Initializing");
        return 0;
    }

    window = SDL_CreateWindow("Pong, but worse 👉😎👈", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    if(window == NULL){
        printf("boo womp\n%s", SDL_GetError());
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if(window == NULL){
        printf("boo womp\n%s", SDL_GetError());
        return 0;
    }
    return 1;
}


void logic(void){
    SDL_Delay(FRAME_TARGET_TIME);
    SDL_Event event;

    int left_speed = MOVE_SPEED;
    int right_speed = MOVE_SPEED;

    const Uint8 *keystate = SDL_GetKeyboardState(NULL);


    while (SDL_PollEvent(&event)){
        switch (event.type){
            case SDL_QUIT:
                is_game_running = 0;  
                break;
        
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                    case SDLK_ESCAPE:
                        is_game_running = 0;
                        break;
                break;
        }
    }

    if(keystate[SDL_SCANCODE_ESCAPE]){
        is_game_running = 0; 
    }


    if(keystate[SDL_SCANCODE_LSHIFT]){
        left_speed = MOVE_SPEED * 2;
    }
    if(keystate[SDL_SCANCODE_RSHIFT]){
        right_speed = MOVE_SPEED * 2;
    } 


    if(keystate[SDL_SCANCODE_W]){
        moveUp(&objLeft, left_speed);
    } 
    if(keystate[SDL_SCANCODE_S]){
        moveDown(&objLeft, left_speed);
    }
    if(keystate[SDL_SCANCODE_I]){
        moveUp(&objRight, right_speed);
    } 
    if(keystate[SDL_SCANCODE_K]){
        moveDown(&objRight, right_speed);
    } 
}


void moveUp(struct object * obj, int speed){
    obj->y -= speed;
    if(obj->y < 0){
        obj->y = 0;
    }
}


void moveDown(struct object * obj, int speed){
    obj->y += speed;
    if(obj->y > WINDOW_HEIGHT-obj->height){
        obj->y = WINDOW_HEIGHT-obj->height;
    }
}


void ballMovement(struct object * obj, const SDL_Rect * left, const SDL_Rect * right, const SDL_Rect * ball){
    SDL_bool ball_left = SDL_HasIntersection(ball, left);
    SDL_bool ball_right = SDL_HasIntersection(ball, right);

    if(obj->y < 0 || obj->y > WINDOW_HEIGHT-obj->height){
        ball_moving_up *= -1;
    }
    obj->y -= ball_speed_vertical * ball_moving_up;


    if(ball_left == SDL_TRUE){
        int middle_of_ball = ball->y + (ball->h/2);
        int hit_point = ((left->y + left->h) - middle_of_ball) / 10;
        printf("hit point %d\n", hit_point);
        switch(hit_point){
            case 1:
                ball_speed_vertical = 4;
                ball_moving_up = -1;
                break;
            case 2:
                ball_speed_vertical = 3;
                ball_moving_up = -1;
                break;
            case 3:
                ball_speed_vertical = 2;
                ball_moving_up = -1;
                break;
            case 4:
                ball_speed_vertical = 1;
                ball_moving_up = -1;
                break;
            case 5:
                ball_speed_vertical = 1;
                ball_moving_up = 1;
                break;
            case 6:
                ball_speed_vertical = 2;
                ball_moving_up = 1;
                break;
            case 7:
                ball_speed_vertical = 3;
                ball_moving_up = 1;
                break;
            case 8:
                ball_speed_vertical = 4;
                ball_moving_up = 1;
                break;
        }
        printf("up speed %d\n", ball_speed_vertical);
        ball_moving_right *= -1;
    }
    if(ball_right == SDL_TRUE){
        int middle_of_ball = ball->y + (ball->h/2);
        int hit_point = ((right->y + right->h) - middle_of_ball) / 10;
        switch(hit_point){
            case 1:
                ball_speed_vertical = 4;
                ball_moving_up = -1;
                break;
            case 2:
                ball_speed_vertical = 3;
                ball_moving_up = -1;
                break;
            case 3:
                ball_speed_vertical = 2;
                ball_moving_up = -1;
                break;
            case 4:
                ball_speed_vertical = 1;
                ball_moving_up = -1;
                break;
            case 5:
                ball_speed_vertical = 1;
                ball_moving_up = 1;
                break;
            case 6:
                ball_speed_vertical = 2;
                ball_moving_up = 1;
                break;
            case 7:
                ball_speed_vertical = 3;
                ball_moving_up = 1;
                break;
            case 8:
                ball_speed_vertical = 4;
                ball_moving_up = 1;
                break;
        }
        printf("up speed %d\n", ball_speed_vertical);
        ball_moving_right *= -1;
    }

    obj->x += (MOVE_SPEED-1) * ball_moving_right;

    if(obj->x > WINDOW_WIDTH-obj->width || obj->x < 0){
        obj->x = WINDOW_WIDTH/2;
        obj->y = WINDOW_HEIGHT/2;
        ball_moving_right *= -1;
        resetBallPos();
    }
}


void resetBallPos(){
    SDL_Delay(FRAME_TARGET_TIME*(FPS/2));
    render();
    SDL_Delay(FRAME_TARGET_TIME*(FPS/2));
}


void render(void){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect left = {
        objLeft.x,
        objLeft.y,
        objLeft.width,
        objLeft.height
    };

    SDL_Rect right = {
        objRight.x,
        objRight.y,
        objRight.width,
        objRight.height
    };

    SDL_Rect ball = {
        objBall.x,
        objBall.y,
        objBall.width,
        objBall.height
    };

    ballMovement(&objBall, &left, &right, &ball);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &left);
    SDL_RenderFillRect(renderer, &right);
    SDL_RenderFillRect(renderer, &ball);

    SDL_RenderPresent(renderer);
}


void exit(void){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}