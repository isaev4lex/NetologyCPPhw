#ifndef FIGURE_H
#define FIGURE_H

#include <string>

class Figure {
protected:
    std::string name;
    Figure(const std::string& name);

public:
    virtual void print_info() const = 0;
    virtual ~Figure() = default;
};

#endif
