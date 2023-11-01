#ifndef _GAME_H_
#define _GAME_H_

/*
	运行环境支持
*/

#pragma comment(lib, "GameTool.lib")

// 初始化运行环境
extern "C" _declspec(dllexport) int InitGame();

// 检查是否初始化成功
extern "C" _declspec(dllexport) void CheckInitSuccess();

// 测试运行
int TestRun();

#endif // !_GAME_H_
