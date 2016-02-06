#ifndef LOG_H
#define LOG_H

#include <easylogging++.h>

#define LOG(string) ( EASYLOG(INFO) << string )

#endif