//
// Created by Joel on 24/11/2025.
//

#ifndef CLIENTETCP_CONSTANTS_H
#define CLIENTETCP_CONSTANTS_H

#define PORT 8080
#define BUFFER_SIZE 1024
// configure how often the socket thread will send movement (ms)
#define SEND_INTERVAL_MS 50


#define NUM_PLATFORMS 12
#define NUM_VINES 14

#define MAX_FRUITS 100
#define MAX_CROCODILES 50



extern const float GRAVITY;
extern const float JUMP_FORCE;
extern const float MOVE_SPEED;
extern const float CROCODILE_SPEED;
extern const float CLIMB_SPEED;



#endif //CLIENTETCP_CONSTANTS_H