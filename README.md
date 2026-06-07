# TCPLibrary

A cross-platform, multi-threaded TCP server library written in C.

<p align="center">
  <img src="docs/tcp_library_architecture.svg" width="800" alt="Architecture diagram">
</p>

## Overview

TCPLibrary provides a structured, thread-safe foundation for building TCP servers. It separates concerns across dedicated threads to maximize throughput and keep each component focused on a single responsibility.

## Architecture

The server is built around four distinct thread roles:

**Lifecycle thread** — Owns server startup and shutdown. Initializes the `tcp_ipv4` structure and coordinates a clean teardown sequence.

**Listener thread** — Accepts incoming connections. Spins up the worker thread pool and pushes new connections into the job queue.

**Processor thread** — Drains the message queue by invoking your registered callback. This is where application logic runs.

**Worker threads** — Each worker reads bytes from an accepted client connection and pushes complete messages into the message queue for the processor thread.

## Getting started
server.c 
```c
void callback(tcp_event_ctx_t *event) {
  send_message(event->sender, "Hello from server.");
}

int main() {
  tcp_ipv4 host;
  tcp_init();
  tcp_create_host(&host, "127.0.0.1", 4455);
  tcp_subscribe(&host, 3, callback);
  os_sleep(4 * _TO_SEC);
  tcp_shutdown(&host);
  tcp_destroy();
}
```
client.c
```c
int main() {
  tcp_ipv4 client;
  tcp_init();
  tcp_create_client(&client, "127.0.0.1", 4455);
  tcp_connect(&client);
  tcp_send(&client, "Hello from client.");
  char buff[1024];
  tcp_receive(&client, buff);
  printf("%s", buff);
  tcp_shutdown(&client);
  tcp_destroy();
}
```
