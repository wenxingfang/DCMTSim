#pragma once

#include <iostream>

struct WFVec{
    int data;
    std::vector<int> e_id;
    std::vector<int> cell_id;
    std::vector<float> peak_time;
    std::vector<float> peak_value;
    std::vector<std::vector<float> > charges;
};
