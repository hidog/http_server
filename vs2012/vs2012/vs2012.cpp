// vs2012.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include "AsyncHttpServer.h"
#include "tool.h"

int _tmain(int argc, _TCHAR* argv[])
{
    load_config();

    AsyncHttpServer a_server;
    a_server.init();
    a_server.start();

	return 0;
}

