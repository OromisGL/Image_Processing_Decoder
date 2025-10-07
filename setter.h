//
// Created by Louis Krämer on 05.10.25.
//

#pragma once
#include <memory>

class Canvas;
class Processor;

void setSharedCanvas(std::shared_ptr<Canvas> ptr);
void setSharedProcessor(std::shared_ptr<Processor> ptr);

std::shared_ptr<Canvas> getCanvas();
std::shared_ptr<Processor> getProcessor();