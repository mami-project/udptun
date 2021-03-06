/**
 * \file sock.h
 * \brief Socket handling.
 *
 *    This contains the prototypes for system calls wrappers, 
 *    socket and BPF creation functions, tun interface creation
 *    functions, network utility functions and die().
 *    Note that raw socket and tun interface related functions are 
 *    Planetlab-specific.
 *
 * \author k.edeline
 * \version 0.1
 */

#ifndef UDPTUN_SOCK_H
#define UDPTUN_SOCK_H

#include <stdio.h>

#include "sysconfig.h"
#if defined(BSD_OS)
#elif defined(LINUX_OS)
#  include <linux/filter.h>
#endif

#include <netinet/in.h>
#include <sys/time.h>

#include "udptun.h"
#include "state.h"

/**
 * \fn struct sockaddr_in *get_addr4(const char *addr, int port)
 * \brief Allocate an AF_INET socket address structure.
 *
 * \param addr The sockaddr address.
 * \param port The sockaddr port.
 * \return A pointer (malloc) to the created struct sockaddr_in
 */ 
struct sockaddr_in *get_addr4(const char *addr, int port);

/**
 * \fn struct sockaddr_in *get_addr6(const char *addr, int port)
 * \brief Allocate an AF_INET6 socket address structure.
 *
 * \param addr The sockaddr address.
 * \param port The sockaddr port.
 * \return A pointer (malloc) to the created struct in6_addr
 */ 
struct sockaddr_in6 *get_addr6(const char *addr, int port);

/**
 * \fn char *addr_to_itf4(char *addr)
 * \brief Lookup interface name from IPv4 address.
 *    Warning: 
 *          You must provide the actual IP bound to the interface,
 *          not the publicly-routable address (if behind a NAT).    
 *
 * \param addr The ip address 
 * \return The associated interface name.
 */
char *addr_to_itf4(char *addr);

/**
 * \fn char *addr_to_itf4(char *addr)
 * \brief Lookup interface name from IPv6 address.
 *    Warning: 
 *          You must provide the actual IP bound to the interface,
 *          not the publicly-routable address (if behind a NAT).    
 *
 * \param addr The ip address 
 * \return The associated interface name.
 */
char *addr_to_itf6(char *addr);

/**
 * \fn int udp_sock4(int port, uint8_t register_gc, char *addr)
 * \brief Create and bind an IPv4 UDP DGRAM socket.
 *
 * \param port The port for the bind call.
 * \param register_gc Register fd to garbage collector.
 * \return The socket fd.
 */ 
int udp_sock4(int port, uint8_t register_gc, char *addr);

/**
 * \fn int udp_sock6(int port, uint8_t register_gc, char *addr)
 * \brief Create and bind an IPv6 UDP DGRAM socket.
 *
 * \param port The port for the bind call.
 * \param register_gc Register fd to garbage collector.
 * \return The socket fd.
 */ 
int udp_sock6(int port, uint8_t register_gc, char *addr);

#if defined(LINUX_OS)
/**
 * \fn int raw_tcp_sock4(const char *addr, int port, const struct sock_fprog * bpf, const char *dev)
 * \brief Create and bind a TCP RAW socket.
 *    Equivalent to raw_sock(addr, port, bpf, dev, IPPROTO_TCP).
 *    This function is planetlab-specific.
 *
 * \param port The port for the bind call.
 * \param addr The adr for the bind call.
 * \param bpf A pointer to a BPF to be attached or NULL.
 * \param dev A pointer to the name of the device to bind. 
 * \param planetlab if running on a PL OS 1 else 0
 * \return The socket fd.
 */ 
int raw_tcp_sock4(int port, char *addr, const struct sock_fprog * bpf, 
                 const char *dev, int planetlab);

/**
 * \fn int raw_sock4(const char *addr, int port, const struct sock_fprog * bpf, const char *dev, int proto)
 * \brief Create and bind a RAW socket.
 *    This function is planetlab-specific.
 *
 * \param port The port for the bind call.
 * \param addr The adr for the bind call.
 * \param bpf A pointer to a BPF to be attached or NULL.
 * \param dev A pointer to the name of the device to bind. 
 * \param proto The protocol of the raw socket (planetlab-specific).
 * \param register_gc Register fd to garbage collector
 * \param planetlab if running on a PL OS 1 else 0
 * \return The socket fd.
 */ 
int raw_sock4(int port, char *addr, const struct sock_fprog * bpf, const char *dev, 
             int proto, uint8_t register_gc, int planetlab);
int raw_sock6(int port, char *addr, const struct sock_fprog * bpf, const char *dev, 
             int proto, uint8_t register_gc, int planetlab);
#endif

/**
 * \fn int xsendto(int fd, struct sockaddr *sa, const void *buf, size_t buflen)
 * \brief sendto syscall wrapper that dies with failure.
 *
 * \param fd The file descriptor of the sending socket. 
 * \param sa The address of the target.
 * \param buf A pointer to the buffer.
 * \param buflen The size of the buffer.
 * \return The amount of bytes sent.
 */ 
int xsendto4(int fd, struct sockaddr *sa, const void *buf, size_t buflen);
int xsendto6(int fd, struct sockaddr *sa, const void *buf, size_t buflen);

/**
 * \fn int xrecv(int fd, void *buf, size_t buflen)
 * \brief recv syscall wrapper that does not dies with failure.
 *
 * \param fd The file descriptor of the receiving socket. 
 * \param buf A pointer to the buffer.
 * \param buflen The size of the buffer.
 * \return The amount of bytes received.
 */ 
int xrecv(int fd, void *buf, size_t buflen);

/**
 * \fn int xselect(fd_set *input_set, int fd_max, struct timeval *tv, int timeout)
 * \brief select wrapper
 *
 * \param input_set fd set
 * \param fd_max max fd value
 * \param tv a pointer to a struct timeval
 * \param timeout The timeout value, -1 for infinite
 * \return The amount of bytes received.
 */ 
int xselect(fd_set *input_set, int fd_max, struct timeval *tv, int timeout);

/**
 * \fn int xrecvfrom(int fd, struct sockaddr *sa, unsigned int *salen, void *buf, size_t buflen)
 * \brief recvfrom syscall wrapper that dies with failure.
 *
 * \param fd The file descriptor of the receiving socket. 
 * \param sa modified on return to indicate the source address.
 * \param salen modified on return to indicate the actual size of the source address.
 * \param buf A pointer to the buffer.
 * \param buflen The size of the buffer.
 * \return The amount of bytes received.
 */ 
int xrecvfrom(int fd, struct sockaddr *sa, unsigned int *salen, void *buf, size_t buflen);

/**
 * \fn int xrecverr(int fd, void *buf, size_t buflen)
 * \brief Receive an error msg from MSG_ERRQUEUE and print a description 
 *        of it via the debug macro.
 *
 * \param fd The socket fd
 * \param buf The buffer to write err msg to
 * \param buflen The len of buf 
 * \param fd_out The fd to forward icmp to, ignore if state is NULL
 * \param state udptun state to forward or NULL
 * 
 * \return 0 if an error msg was received, 
 *         a negative value if an error happened
 */ 
int xrecverr(int fd, void *buf, size_t buflen, int fd_out, struct tun_state *state);

/**
 * \fn int xread(int fd, char *buf, int buflen)
 * \brief read syscall wrapper that dies with failure.
 *
 * \param fd The file descriptor of the receiving socket. 
 * \param buf A pointer to the buffer.
 * \param buflen The size of the buffer.
 * \return The amount of bytes read.
 */ 
int xread(int fd, char *buf, int buflen);

/**
 * \fn int xwrite(int fd, char *buf, int buflen)
 * \brief write syscall wrapper that dies with failure.
 *
 * \param fd The file descriptor of the sending socket.
 * \param buf A pointer to the buffer.
 * \param buflen The size of the buffer.
 * \return The amount of bytes written.
 */ 
int xwrite(int fd, char *buf, int buflen);

/**
 * \fn int xfwrite(FILE *fp, char *buf, int size, int nmemb)
 * \brief fwrite syscall wrapper that dies with failure.
 *
 * \param fp The file pointer
 * \param buf A pointer to the buffer.
 * \param size The size of data elements to write
 * \param nmemb The number of data elements to write
 * \return The amount of bytes written.
 */ 
int xfwrite(FILE *fp, char *buf, int size, int nmemb);

/**
 * \fn void die(char *s)
 * \brief Exit with an error message.
 *
 * \param s The error message.
 */ 
void die(char *s);

void *xmalloc(size_t size);

#endif

