// vs2012.cpp : �w�q�D���x���ε{�����i�J�I�C
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

