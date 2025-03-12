#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>

#ifndef INBUFSIZE
#define INBUFSIZE 1024
#endif

const char response[] = "Ok\n";
const char length_err[] = "Line too long! Good bye...\n";

/* === session === */

struct session {
	int fd;
	unsigned long from_ip;
	unsigned short from_port;
	char buf[INBUFSIZE];
	int buf_used;
};

struct session *make_new_session(int fd, struct sockaddr_in *from)
{
	struct session *sess = malloc(sizeof(struct session));

	sess->fd = fd;
	sess->from_ip = ntohl(from->sin_addr.s_addr);
	sess->from_port = ntohs(from->sin_port);
	sess->buf_used = 0;

	return sess;
}

void session_send_string(struct session *sess, const char *str)
{
	write(sess->fd, str, strlen(str));
}

void session_check_lf(struct session *sess)
{
	int i, pos = -1;
	
	for(i = 0; i < sess->buf_used; i++) {
		if(sess->buf[i] == '\n') {
			session_send_string(sess, response);
			pos = i;
			break;
		}
	}

	if(pos == -1)
		return;

	sess->buf_used -= (pos + 1);
	memmove(sess->buf, sess->buf + pos + 1, sess->buf_used);
}

int session_do_read(struct session *sess)
{
	int read_res, bufp = sess->buf_used;

	read_res = read(sess->fd, sess->buf + bufp, INBUFSIZE - bufp);
	if(read_res == -1) 
		return -1;

	if(read_res == 0)
		return -1;

	if(read_res == 1 && *(sess->buf + bufp) == '\4')	/* EOT */
		return -1;

	sess->buf_used += read_res;
	session_check_lf(sess);
	if(sess->buf_used >= INBUFSIZE) {
		session_send_string(sess, length_err);
		return -1;
	}

	return 0;
}

/* === server === */

struct server {
	int ls;
	struct session **sess_array;
	int sess_array_size;
};

int server_init(struct server *serv, int port)
{
	int i, ls, ok;
	struct sockaddr_in addr;
	socklen_t len;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	ls = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(ls == -1) {
		perror("socket");
		return -1;
	}

	int opt = 1;
	setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	len = sizeof(addr);
	ok = bind(ls, (struct sockaddr *) &addr, len);
	if(ok == -1) {
		perror("bind");
		close(ls);
		return -1;
	}

	ok = listen(ls, SOMAXCONN);
	if(ok == -1) {
		perror("listen");
		close(ls);
		return -1;
	}

	serv->ls = ls;
	serv->sess_array = malloc(sizeof(*serv->sess_array) * SOMAXCONN);
	serv->sess_array_size = SOMAXCONN;
	for(i = 0; i < SOMAXCONN; i++)
		serv->sess_array[i] = NULL;

	return 0;
}

void server_accept_client(struct server *serv)
{
	int sd;
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);

	sd = accept(serv->ls, (struct sockaddr *) &addr, &len);
	if(sd == -1) {
		perror("accept");
		return;
	}

	serv->sess_array[sd] = make_new_session(sd, &addr);
}

void server_close_session(struct server *serv, int sd)
{
	close(sd);
	serv->sess_array[sd]->fd = -1;
	free(serv->sess_array[sd]);
	serv->sess_array[sd] = NULL;
	
}

int server_go(struct server *serv)
{
	int ok, i, res;
	int max_d = serv->ls;
	fd_set readfds;

	for(;;) {
		FD_ZERO(&readfds);
		FD_SET(serv->ls, &readfds);
		max_d = serv->ls;

		for(i = 0; i < SOMAXCONN; i++) {
			if(serv->sess_array[i]) {
				FD_SET(i, &readfds);
				if(i > max_d)
					max_d = i;
			}
		}

		res = select(max_d + 1, &readfds, NULL, NULL, NULL);
		if(res == -1 && errno != EINTR) {
			perror("select");
			return 4;
		}
		
		if(FD_ISSET(serv->ls, &readfds)) 
			server_accept_client(serv);

		for(i = 0; i < SOMAXCONN; i++) {
			if(serv->sess_array[i] && FD_ISSET(i, &readfds)) {
				ok = session_do_read(serv->sess_array[i]);
				if(ok == -1)
					server_close_session(serv, i);
			}
		}
	}

	return 0;
}

int main(int argc, char **argv)
{
	struct server serv;
	long port;
	char *endptr = NULL;

	if(argc != 2) {
		fprintf(stderr, "Specify the port number\n");
		return 1;
	}
	
	port = strtol(argv[1], &endptr, 10);
	if(!*argv[1] || *endptr) {
		fprintf(stderr, "Invalid port number\n");
		return 2;
	}

	if(server_init(&serv, port) == -1)
		return 3;

	return server_go(&serv);
}
