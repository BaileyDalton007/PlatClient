#include <iostream>
#include "rpc_h.h"
#include <process.h>
#include <stdio.h>
#include <vector>

int packetIter = 0;
const char* rpcData[2] = { "unset", "unset"};

const char* rcps;
const char* lcps;

// Server function.
void Output(const char* szOutput)
{
    if (packetIter >= sizeof(rpcData) / sizeof(rpcData[0]))
    {
        packetIter = 0;
    }
    rpcData[packetIter] = szOutput;
    packetIter++;

}

const char* getlcps()
{
    rcps = reinterpret_cast<const char*>(rpcData[0]);
    return rcps;
}

const char* getrcps()
{
    lcps = reinterpret_cast<const char*>(rpcData[1]);
    return lcps;
}

// Naive security callback.
RPC_STATUS CALLBACK SecurityCallback(RPC_IF_HANDLE /*hInterface*/, void* /*pBindingHandle*/)
{
    return RPC_S_OK; // Always allow anyone.
}

void rpcMain(void* data)
{
    RPC_STATUS status;

    // Uses the protocol combined with the endpoint for receiving
    // remote procedure calls.
    status = RpcServerUseProtseqEpA(
        (RPC_CSTR)("ncacn_ip_tcp"), // Use TCP/IP protocol.
        RPC_C_PROTSEQ_MAX_REQS_DEFAULT, // Backlog queue length for TCP/IP.
        (RPC_CSTR)("4747"),         // TCP/IP port to use.
        NULL);                          // No security.

    if (status)
        exit(status);

    // Registers the Example1 interface.
    status = RpcServerRegisterIf2(
        rpc_v1_0_s_ifspec,              // Interface to register.
        NULL,                                // Use the MIDL generated entry-point vector.
        NULL,                                // Use the MIDL generated entry-point vector.
        RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH, // Forces use of security callback.
        RPC_C_LISTEN_MAX_CALLS_DEFAULT,      // Use default number of concurrent calls.
        (unsigned)-1,                        // Infinite max size of incoming data blocks.
        SecurityCallback);                   // Naive security callback.

    if (status)
        exit(status);

    // Start to listen for remote procedure
    // calls for all registered interfaces.
    // This call will not return until
    // RpcMgmtStopServerListening is called.
    status = RpcServerListen(
        1,                                   // Recommended minimum number of threads.
        RPC_C_LISTEN_MAX_CALLS_DEFAULT,      // Recommended maximum number of threads.
        FALSE);                              // Start listening now.

    if (status)
        exit(status);
}

// Memory allocation function for RPC.
// The runtime uses these two functions for allocating/deallocating
// enough memory to pass the string to the server.
void* __RPC_USER midl_user_allocate(size_t size)
{
    return malloc(size);
}

// Memory deallocation function for RPC.
void __RPC_USER midl_user_free(void* p)
{
    free(p);
}

int startRPCThread()
{
    HANDLE RPCHandle;
    RPCHandle = (HANDLE)_beginthread(&rpcMain, 0, 0);
    return 0;
}