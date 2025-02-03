#include "../Headers/camera.h"
#include "../Headers/input.h"
Camera camera = {
    .position = {0,0,0},
    .direction = {0,0,-1.0f},
    .cam_vel = {0.0f,0.0f,0.0f},
    .right = {1.0,0.0,0.0},
    .worldUp = {0.0,1.0,0.0},
    .pitch = 0.0,
    .yaw = -90.0,
    .roll = 0.0
};

float camSpeed = 12.0f;
float fov = 45.0f;
void init_camera(vec3 position, vec3 w_up){
    //set the camera position and world up vector
    glm_vec3_copy(position,camera.position);
   // glm_vec3_copy(w_up,camera.worldUp);

    //Get the right vector
    glm_cross(&camera.direction[0],&camera.worldUp[0],&camera.right[0]);
    glm_vec3_normalize(&camera.right[0]);

    glm_vec3_cross(&camera.right[0],&camera.direction[0],&camera.up[0]);

    updateCameraVectors(1);

}
void process_mouse_move(float xPos, float yPos,float delta_time){
    float sensitivity = 0.1f;

    camera.yaw += (float)-xPos * sensitivity * delta_time;
    camera.pitch += (float)yPos * sensitivity * delta_time;
    
    if(camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if(camera.pitch < -89.0f)
        camera.pitch = -89.0f;
        
    updateCameraVectors(delta_time);
}
void process_keyboard_movement(float delta_time){
   vec3 movement = {0.0f, 0.0f, 0.0f};

    if (keyStates[SDL_SCANCODE_W]) {
        glm_vec3_scale(camera.direction, camSpeed * delta_time, camera.cam_vel);
        glm_vec3_add(movement, camera.cam_vel, movement);
    }
    if (keyStates[SDL_SCANCODE_S]) {
        glm_vec3_scale(camera.direction, camSpeed * delta_time, camera.cam_vel);
        glm_vec3_sub(movement, camera.cam_vel, movement);
    }
    if (keyStates[SDL_SCANCODE_A]) {
        glm_vec3_cross(camera.direction, camera.up, camera.cam_vel);
        glm_vec3_normalize(camera.cam_vel);
        glm_vec3_scale(camera.cam_vel, camSpeed * delta_time, camera.cam_vel);
        glm_vec3_sub(movement, camera.cam_vel, movement);
    }
    if (keyStates[SDL_SCANCODE_D]) {
        glm_vec3_cross(camera.direction, camera.up, camera.cam_vel);
        glm_vec3_normalize(camera.cam_vel);
        glm_vec3_scale(camera.cam_vel, camSpeed * delta_time, camera.cam_vel);
        glm_vec3_add(movement, camera.cam_vel, movement);
    }

    if (keyStates[SDL_SCANCODE_R]) {
        glm_vec3_scale(camera.worldUp, camSpeed * delta_time, camera.cam_vel);
        glm_vec3_add(movement, camera.cam_vel, movement);
    }
    if (keyStates[SDL_SCANCODE_F]) {
        glm_vec3_scale((vec3){0,-1,0}, -camSpeed * delta_time, camera.cam_vel);
        glm_vec3_sub(movement, camera.cam_vel, movement);
    }

    // Normalize the movement to prevent faster diagonal movement
    if (!glm_vec3_eqv_eps(movement, (vec3){0.0f, 0.0f, 0.0f})) {
        glm_vec3_normalize(movement);
        glm_vec3_scale(movement, camSpeed * delta_time, movement);
        glm_vec3_add(camera.position, movement, camera.position);
    }
    
    updateCameraVectors(delta_time);

}
void camera_look_at(mat4* view){
    vec3 t = {0.0,0.0,0.0};
    glm_vec3_add(&camera.position[0],&camera.direction[0],&t[0]);
    glm_lookat_rh_no(&camera.position[0],&t[0],&camera.up[0], view[0]);
}
void updateCameraVectors(float delta_time){
    // calculate the new Front vector
    float local_pitch = camera.pitch;
    float local_yaw = camera.yaw;
    glm_make_rad(&local_pitch);
    glm_make_rad(&local_yaw);

    vec3 front;
    front[0] = cosf(local_yaw) * cosf(local_pitch);
    front[1] = sinf(local_pitch);
    front[2] = sinf(local_yaw) * cosf(local_pitch);
    glm_vec3_copy(front,camera.direction);
    glm_vec3_normalize(&camera.direction[0]);
    // also re-calculate the Right and Up vector
    glm_cross(&camera.direction[0],&camera.worldUp[0],&camera.right[0]);

    glm_vec3_normalize(&camera.right[0]);
    glm_vec3_cross(&camera.right[0],&camera.direction[0],&camera.up[0]);
    glm_vec3_normalize(&camera.up[0]);

    camera.cam_vel[0] = 0;
    camera.cam_vel[1] = 0;
    camera.cam_vel[2] = 0;
    
    
}
void get_camera_position(vec3* vec){
    glm_vec3_copy(&camera.position[0],vec[0]);
}
void get_camera_direction(vec3* vec){
    glm_vec3_copy(&camera.direction[0],vec[0]);
}
void rotate_around_point(vec3 target, float radius, float angle, mat4* view) {
    // Calculate the new camera position on the circular path
    camera.position[0] = target[0] + radius * cosf(angle);
    camera.position[1] = target[1];
    camera.position[2] = target[2] + radius * sinf(angle);

    // Update the camera's direction to always look at the target point
    vec3 direction;
    glm_vec3_sub(target, camera.position, direction);
    glm_vec3_normalize(direction);
    glm_vec3_copy(direction, camera.direction);

    // Update the right and up vectors
    glm_vec3_cross(camera.direction, camera.worldUp, camera.right);
    glm_vec3_normalize(camera.right);
    glm_vec3_cross(camera.right, camera.direction, camera.up);
    glm_vec3_normalize(camera.up);

    // Update the view matrix
    glm_lookat_rh_no(camera.position, target, camera.up, view[0]);
}