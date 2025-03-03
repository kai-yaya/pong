#include <SDL.h>
#include <stdio.h>


const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int FPS = 60;
const float FRAME_TARGET_TIME = 1000 / FPS;
const int MOVE_SPEED = 3;
SDL_Window * window = NULL;
SDL_Renderer * renderer = NULL;
int is_game_running = 0;
int last_frame_time = 0;
int ball_moving_right = -1; // 1 is moving right, -1 is moving left
int ball_moving_up = 1; // 1 is moving up, -1 us moving down


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
    objLeft.x = 50;
    objLeft.y = 275;
    objLeft.height = 55;
    objLeft.width = 5;

    objRight.x = 720;
    objRight.y = 275;
    objRight.height = 55;
    objRight.width = 5;

    objBall.x = WINDOW_WIDTH/2;
    objBall.y = WINDOW_HEIGHT/2;
    objBall.height = 15;
    objBall.width = 15;
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
    SDL_bool ball_left = SDL_HasIntersection(ball, right);
    SDL_bool ball_right = SDL_HasIntersection(ball, left);
    

    if(obj->y < 0 || obj->y > WINDOW_HEIGHT-obj->height){
        ball_moving_up *= -1;
    }
    obj->y -= MOVE_SPEED * ball_moving_up;


    if(ball_left == SDL_TRUE){
        printf("%d\n", ball_moving_right);
        ball_moving_right *= -1;
    }
    if(ball_right == SDL_TRUE){
        printf("%d\n", ball_moving_right);
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