#ifndef DATA_H
#define DATA_H

struct Set
{
	int size;
	int* elements;
};

struct Family
{
	int size;
	struct Set* sets;
};

void PrintFamily(struct Family* f);
void PrintSet(struct Set* s);

void DeleteFamily(struct Family* f);
void DeleteSet(struct Set* s);


#endif // DATA_H
