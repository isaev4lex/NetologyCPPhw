#include "counter.h"

void Counter::increase_counter() {
	counter_value++;
}

void Counter::decrease_counter() {
	counter_value--;
}
int Counter::show_counter_value() {
	return counter_value;
}
