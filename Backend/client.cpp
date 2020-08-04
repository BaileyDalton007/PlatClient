#include <iostream>
#include "rpcClient_h.h"
#include <process.h>
#include <stdio.h>
#include "main.h"
#include <vector>
#include "cps.h"

const char* lcps = "unset";
const char* rcps = "unset";

std::vector<const char*>rpcPacket(2, "unset");

void rpcClientMain(void* data)
{
    RPC_STATUS status;
    RPC_CSTR szStringBinding = NULL;

    // Creates a string binding handle.
    // This function is nothing more than a printf.
    // Connection is not done here.
    while (1) {
        for (int i = 0; i < rpcPacket.size(); i++)
        {
            const char* lcps = getCPS();
            const char* rcps = getRCPS();
            
            rpcPacket[0] = lcps;
            rpcPacket[1] = rcps;



            status = RpcStringBindingComposeA(
                NULL,                                        // UUID to bind to.
                (RPC_CSTR)("ncacn_ip_tcp"),// Use TCP/IP protocol.
                (RPC_CSTR)("localhost"),   // TCP/IP network address to use.
                (RPC_CSTR)("4747"),        // TCP/IP port to use.
                NULL,                                    // Protocol dependent network options to use.
                &szStringBinding);                       // String binding output.

            if (status)
                exit(status);

            // Validates the format of the string binding handle and converts
            // it to a binding handle.
            // Connection is not done here either.
            status = RpcBindingFromStringBindingA(
                szStringBinding,        // The string binding to validate.
                &rpcClientBinding);     // Put the result in the implicit binding
                                        // handle defined in the IDL file.

            if (status)
                exit(status);

            RpcTryExcept
            {
                // Calls the RPC function. The hExample1Binding binding handle
                // is used implicitly.
                // Connection is done here.
                    const char* output = (const char*)rpcPacket[i];
                    Output(output);
            }
                RpcExcept(1)
            {
                std::cerr << "Runtime reported exception " << RpcExceptionCode()
                    << std::endl;
            }
            RpcEndExcept

                // Free the memory allocated by a string.
                status = RpcStringFreeA(
                    &szStringBinding); // String to be freed.

            if (status)
                exit(status);

            // Releases binding handle resources and disconnects from the server.
            status = RpcBindingFree(
                &rpcClientBinding); // Frees the implicit binding handle defined in the IDL file.

            if (status)
                exit(status);
        }
        }
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

int startRPCClientThread()
{
    HANDLE RPCHandle;
    RPCHandle = (HANDLE)_beginthread(&rpcClientMain, 0, 0);
    return 0;
}