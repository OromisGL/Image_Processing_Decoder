//
// Created by Louis Krämer on 05.10.25.
//

#pragma once

#include "processor.h"
#include "Canvas/Canvas.h"


void setSharedCanvas(Canvas* ptr);
void setSharedProcessor(Processor* ptr);

Canvas& getCanvas();
Processor& getProcessor();