/**
 * FlappyBird-N64 - collision.h
 *
 * Copyright 2017, Christopher Bonhage
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __FLAPPY_COLLISION_H
#define __FLAPPY_COLLISION_H

#include "system.h"

#include "bird.h"
#include "pipes.h"

#define COLLISION_PIPE_HALF_WIDTH   0.052
#define COLLISION_PIPE_HALF_GAP     0.23
#define COLLISION_SCORE_X_TOLERANCE 0.01

void collision_tick(bird_t *bird, pipes_t *pipes);

#endif
