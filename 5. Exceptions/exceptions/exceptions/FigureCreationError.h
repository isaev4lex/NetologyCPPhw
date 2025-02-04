#ifndef FIGURE_CREATION_ERROR_H
#define FIGURE_CREATION_ERROR_H

#include <stdexcept>
#include <string>

class FigureCreationError : public std::domain_error {
public:
    explicit FigureCreationError(const std::string& message) : std::domain_error(message) {}
};

#endif