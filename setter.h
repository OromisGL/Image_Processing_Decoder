//
// Created by Louis Krämer on 05.10.25.
//

#pragma once
#include <memory>

class Canvas;
class Processor;
class Simulation;

void setSharedCanvas(std::shared_ptr<Canvas> ptr);
void setSharedProcessor(std::shared_ptr<Processor> ptr);
void setSharedSimulation(std::shared_ptr<Simulation> ptr);

std::shared_ptr<Canvas> getCanvas();
std::shared_ptr<Processor> getProcessor();
std::shared_ptr<Simulation> getSimulation();