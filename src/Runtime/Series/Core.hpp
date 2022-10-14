#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
void AssertLog(bool j, const std::string& log)
{
    if (!j) {
        throw std::runtime_error(log);
    }
}