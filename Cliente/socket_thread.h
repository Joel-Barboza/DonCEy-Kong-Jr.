//
// Created by Joelb on 8/11/2025.
//
#pragma once

#include <winsock2.h>
#ifndef CLIENTETCP_SOCKET_THREAD_H
#define CLIENTETCP_SOCKET_THREAD_H

int socket_thread(void *data);
int send_message(const char *message);
int receive_message(char *buffer, size_t buffer_size, int blocking);
int is_connected();
void fruit_JSON(const char* str);
int retry_connection(const char *ip);
void close_connection();

#endif //CLIENTETCP_SOCKET_THREAD_H
