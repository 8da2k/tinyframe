#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include "listener.h"
#include "inetaddr.h"
#include "event.h"
#include "debug.h"


/* �����ӵ���ʱ,�ú����ᱻ����,
 * ���������accept,�ٵ���listener�е�accept_cb�ص�����
 */
static void event_accept_callback(int listenfd, void *arg)
{
	listener *ls = (listener *)arg;
	socklen_t clilen = sizeof(ls->client_addr.addr);
	
	//int connfd = accept(listenfd, (struct sockaddr *)&ls->client_addr.addr, &clilen);

	int connfd = accept4(listenfd, (struct sockaddr *)&ls->client_addr.addr,
                         &clilen, SOCK_NONBLOCK | SOCK_CLOEXEC);
	if (connfd < 0)
	{
		/* ������,�ο�muduo��<<UNP>>16.6 */
		debug_ret("file: %s, line: %d", __FILE__, __LINE__);
		int save = errno;
	    switch (save)
	    {
			case EAGAIN:
			case ECONNABORTED:
			case EINTR:
			case EPROTO:
			case EPERM:
			case EMFILE:
				return;
			case EBADF:
			case EFAULT:
			case EINVAL:
			case ENFILE:
			case ENOBUFS:
			case ENOMEM:
			case ENOTSOCK:
			case EOPNOTSUPP:
				debug_sys("file: %s, line: %d", __FILE__, __LINE__);
			default:
				debug_sys("file: %s, line: %d", __FILE__, __LINE__);
	    }
	}

	char buff[50];
	debug_msg("connection from %s, port %d",
			inet_ntop(AF_INET, &ls->client_addr.addr.sin_addr, buff, sizeof(buff)),
			ntohs(ls->client_addr.addr.sin_port));

	ls->accept_cb(connfd, ls);	/* accept_callback() */
}

/* ����һ����������,������ַ���û��ṩ,��ls_addr�������� */
listener *create_listener(server *server, accept_cb_pt callback, inet_address *ls_addr)
{
	listener *ls = malloc(sizeof(listener));
	ls->server = server;
	ls->accept_cb = callback;
	ls->listen_addr = ls_addr;
	
	/* �����������׽��� */
	ls->listen_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
	if (ls->listen_fd < 0)
	{
		debug_ret("file: %s, line: %d", __FILE__, __LINE__);
		free(ls);
		return NULL;
	}

	int ret = bind(ls->listen_fd, (struct sockaddr *)&ls_addr->addr, sizeof(ls_addr->addr));
	if (ret < 0)
	{
		debug_ret("file: %s, line: %d", __FILE__, __LINE__);
		close(ls->listen_fd);
		free(ls);
		return NULL;
	}

	/* SOMAXCONN : /proc/sys/net/core/somaxconn */
	ret = listen(ls->listen_fd, SOMAXCONN);
	if (ret < 0)
	{
		debug_ret("file: %s, line: %d", __FILE__, __LINE__);
		close(ls->listen_fd);
		free(ls);
		return NULL;
	}

	ls->ls_event = create_event(server->manager, ls->listen_fd, EPOLLIN | EPOLLPRI,
									event_accept_callback, ls, NULL, NULL);
	if (ls->ls_event == NULL)
	{
		debug_ret("file: %s, line: %d", __FILE__, __LINE__);
		close(ls->listen_fd);
		free(ls);
		return NULL;
	}
	
	event_add(ls->ls_event);

	/* ��ʱ�����׽��ִ��ڼ���״̬,�յ������ӵ���event_accept_callback���� */
	
	return ls;
}

void listener_free(listener *ls)
{
	close(ls->listen_fd);
	event_free(ls->ls_event);
	free(ls);
}

