#ifndef camera_h
#define camera_h
#include <C:\SDL2\include\SDL2\SDL.h>
#include "../../include/cglm/cglm.h"
typedef struct {
    vec3 position;
    vec3 direction;
    vec3 up;
    vec3 right;
    vec3 worldUp;
    vec3 cam_vel;
    mat4 view;
    mat4 projection;
    float pitch;
    float yaw;
    float roll;
} Camera;
void init_camera(vec3 position, vec3 w_up);
void camera_look_at();
void process_mouse_move(float xPos, float yPos, float delta_time);
void process_keyboard_movement(float delta_time);
void updateCameraVectors(float delta_time);
void get_camera_position(vec3* vec);
void get_camera_direction(vec3* vec);
void rotate_camera_y(float delta);
void rotate_around_point(vec3 target, float radius, float angle, mat4* view);

extern float fov;
extern Camera camera;
#endif