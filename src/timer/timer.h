/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once

void timer_step(void);
float timer_getTime(void);
float timer_getFPS(void);
float timer_getDelta(void);
float timer_getAverageDelta(void);
void timer_sleep(short ms);
void timer_init(void);

