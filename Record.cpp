#include "Record.h"

void Record::calculateKey()
{
	key = 0;
	for (int i = 0; i < Record::numElements; i++) {
		key += elements[i];
	}
}
