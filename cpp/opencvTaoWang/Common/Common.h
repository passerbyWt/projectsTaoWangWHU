#ifndef _COMMON_H
#define _COMMON_H

#ifdef _DEBUG
#pragma comment(lib, "cv200d.lib")
#pragma comment(lib, "cvaux200d.lib")
#pragma comment(lib, "cxcore200d.lib")
#pragma comment(lib, "highgui200d.lib")
#pragma comment(lib, "ml200d.lib")
#else
#pragma comment(lib, "cv200.lib")
#pragma comment(lib, "cvaux200.lib")
#pragma comment(lib, "cxcore200.lib")
#pragma comment(lib, "highgui200.lib")
#pragma comment(lib, "ml200.lib")
#endif

#endif