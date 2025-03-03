#include <SDL.h>
#include <stdio.h>
#define FPS 60

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float FRAME_TARGET_TIME = 1000 / FPS;
const int MOVE_SPEED = 3;
SDL_Window * window = NULL;
SDL_Renderer * renderer = NULL;
int is_game_running = 0;
int last_frame_time = 0;
int ball_moving_right = -1; // 1 is moving right, -1 is moving left
int ball_moving_up = 1; // 1 is moving up, -1 us moving down
int platform_height = 90;
int platform_width = 7;
int ball_size = 15;


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
void moveUp(struct object * obj);
void moveDown(struct object * obj);
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
    if(keystate[SDL_SCANCODE_W]){
        moveUp(&objLeft);
    } 
    if(keystate[SDL_SCANCODE_S]){
        moveDown(&objLeft);
    }
    if(keystate[SDL_SCANCODE_I]){
        moveUp(&objRight);
    } 
    if(keystate[SDL_SCANCODE_K]){
        moveDown(&objRight);
    } 
}


void moveUp(struct object * obj){
    obj->y -= MOVE_SPEED;
    if(obj->y < 0){
        obj->y = 0;
    }
}


void moveDown(struct object * obj){
    obj->y += MOVE_SPEED;
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
    obj->y -= MOVE_SPEED * ball_moving_up;


    if(ball_left == SDL_TRUE){
        int middle_of_ball = ball->y + (ball->h/2);
        printf("left y1: %d\n", left->y);
        printf("left y2: %d\n", left->y + (left->h/3));
        printf("left y3: %d\n", left->y + ((left->h/3) * 2));
        if(middle_of_ball >= left->y && middle_of_ball < left->y + (left->h/3)){
            printf("left top\n");
        }
        else if(middle_of_ball >= left->y + (left->h/3) && middle_of_ball < left->y + ((left->h/3) * 2) ){
            printf("left middle\n");
        }
        else{
            printf("left bottom\n");
        }
        ball_moving_right *= -1;
    }
    if(ball_right == SDL_TRUE){
        int middle_of_ball = ball->y + (ball->h/2);
        printf("right y1: %d\n", right->y);
        printf("right y2: %d\n", right->y + (right->h/3));
        printf("right y3: %d\n", right->y + ((right->h/3) * 2));
        if(middle_of_ball >= right->y && middle_of_ball < right->y + (right->h/3)){
            printf("right top\n");
        }
        else if(middle_of_ball >= right->y + (right->h/3) && middle_of_ball < right->y + ((right->h/3) * 2) ){
            printf("right middle\n");
        }
        else{
            printf("right bottom\n");
        }
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