/*
 * Copyright (c) 2019 Chris Burns <chris@kitty.city>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

using NUnit.Framework;
using SL;
using System;
using System.Net;

public unsafe class SLIntegration
{
    const ushort LISTEN_PORT = 51343;
    const int SL_OK = 0;

    ushort _port = Util.HtoN(LISTEN_PORT);
    C.Socket _sock_server;
    C.Socket _sock_client;

    [OneTimeSetUp]
    public void FixtureSetup()
    {
    }

    [OneTimeTearDown]
    public void FixtureCleanup()
    {
    }

    [SetUp]
    public void TestSetup()
    {
        _sock_server = default;
        _sock_client = default;
    }

    [TearDown]
    public void TestCleanup()
    {
        if (_sock_client.fd != 0)
        {
            fixed (C.Socket* sockptr = &_sock_client)
            {
                UDP.SocketClose(sockptr);
            }
        }
        if (_sock_server.fd != 0)
        {
            fixed (C.Socket* sockptr = &_sock_server)
            {
                UDP.SocketClose(sockptr);
            }
        }
        UDP.Cleanup();
    }

    [Test]
    public void UDP_SocketSendRecv()
    {
        Random rand = new Random();

        /* server side endpoints, socket, random payload, send/recv buffers */
        C.IPv4 loopback = C.IPv4.New(127, 0, 0, 1);
        C.Endpoint ep_server = C.Endpoint.NewV4(_port, loopback);
        C.Endpoint ep_server_recv = default;
        _sock_server = C.Socket.NewUDP(ep_server);
        C.Buffer buf_server_recv;
        C.Buffer buf_server_send;
        byte[] pl_server = new byte[1235];
        byte[] mem_server = new byte[1408];
        rand.NextBytes(pl_server);
        fixed (byte* bufptr = pl_server) buf_server_send = C.Buffer.New(bufptr, pl_server.Length);
        fixed (byte* bufptr = mem_server) buf_server_recv = C.Buffer.New(bufptr, mem_server.Length);

        /* client side endpoints, socket, random payload, send/recv buffers */
        C.Endpoint ep_client = C.Endpoint.NewV4(_port + 1, loopback);
        C.Endpoint ep_client_recv = default;
        _sock_client = C.Socket.NewUDP(ep_client);
        C.Buffer buf_client_recv;
        C.Buffer buf_client_send;
        byte[] pl_client = new byte[256];
        byte[] mem_client = new byte[1408];
        rand.NextBytes(pl_client);
        fixed (byte* bufptr = pl_client) buf_client_send = C.Buffer.New(bufptr, pl_client.Length);
        fixed (byte* bufptr = mem_client) buf_client_recv = C.Buffer.New(bufptr, mem_client.Length);

        Assert.True(UDP.Setup());

        fixed (C.Socket* sockptr_server = &_sock_server)
        fixed (C.Socket* sockptr_client = &_sock_client)
        {
            Assert.True(UDP.SocketOpen(sockptr_server));

            Assert.True(UDP.SocketOpen(sockptr_client));
            Assert.True(UDP.SocketNonBlocking(sockptr_client, true));

            Assert.AreEqual(UDP.SocketSend(sockptr_client, &buf_client_send, 1, &ep_server), pl_client.Length);
            Assert.AreEqual(UDP.SocketRecv(sockptr_server, &buf_server_recv, 1, &ep_server_recv), pl_client.Length);
            fixed (byte* bufptr = pl_client) Assert.True(Util.MemCmp(bufptr, 0, buf_server_recv.buf, 0, pl_client.Length));
            Assert.True(Util.MemCmp((byte*)&ep_client, 2, (byte*)&ep_server_recv, 2, sizeof(C.Endpoint) - 2));

            Assert.AreEqual(UDP.SocketSend(sockptr_server, &buf_server_send, 1, &ep_client), pl_server.Length);
            Assert.AreEqual(UDP.SocketRecv(sockptr_client, &buf_client_recv, 1, &ep_client_recv), pl_server.Length);
            fixed (byte* bufptr = pl_server) Assert.True(Util.MemCmp(bufptr, 0, buf_client_recv.buf, 0, pl_server.Length));
            Assert.True(Util.MemCmp((byte*)&ep_server, 2, (byte*)&ep_client_recv, 2, sizeof(C.Endpoint) - 2));

            Assert.True(UDP.SocketClose(sockptr_client));
            Assert.True(UDP.SocketClose(sockptr_server));
        }
    }
}
