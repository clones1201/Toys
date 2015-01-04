
#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#define _DEBUG

#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

#define RW_ELSE		0
#define RW_IF		1
#define RW_INT		2
#define	RW_RETURN	3
#define	RW_VOID		4
#define	RW_WHILE	5

static string ReservedWords[6]=
    {
        "else","if","int","return","void","while"
    };

#endif
