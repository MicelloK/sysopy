#include "utils.h"

void* connection_manager(void* arg);
void* ping_manager(void* arg);
void* input_manager(void* arg);

in_port_t inet_port;
char* unix_socket_path_name;

int server_unix_socket_fd = -1;
int server_inet_socket_fd = -1;

struct client clients[SERVER_CLIENT_LIMIT];
int registered_clients_num;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void open_server() {
	struct sockaddr_un sa_unix;
	strcpy(sa_unix.sun_path, unix_socket_path_name);
	sa_unix.sun_family = AF_UNIX;

	if((server_unix_socket_fd = socket(AF_UNIX, SOCKET_PROTOCOL, 0)) == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	if((bind(server_unix_socket_fd, (struct sockaddr*) &sa_unix, sizeof(sa_unix))) == -1) {
		perror("bind");
		exit(EXIT_FAILURE);
	}
	if((listen(server_unix_socket_fd, SERVER_CLIENT_LIMIT)) == -1) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	printf("[SERVER] socket path: %s\n", unix_socket_path_name);

	struct hostent *host_entry = gethostbyname("localhost");
	struct in_addr host_address = *(struct in_addr*) host_entry->h_addr;

	struct sockaddr_in sa_inet;
	sa_inet.sin_family = AF_INET;
	sa_inet.sin_addr.s_addr = host_address.s_addr;
	sa_inet.sin_port = htons(inet_port);

	if((server_inet_socket_fd = socket(AF_INET, SOCKET_PROTOCOL, 0)) == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	if((bind(server_inet_socket_fd, (struct sockaddr*) &sa_inet, sizeof(sa_inet))) == -1) {
		perror("bind");
		exit(EXIT_FAILURE);
	}
	if((listen(server_inet_socket_fd, SERVER_CLIENT_LIMIT)) == -1) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	printf("[SERVER] INET socket: %s, addres: %d\n", inet_ntoa(host_address), inet_port);
}

void close_server() {
	if (server_unix_socket_fd != -1) {
		close(server_unix_socket_fd);
		unlink(unix_socket_path_name);
	}
	if (server_inet_socket_fd != -1) {
		close(server_inet_socket_fd);
	}
}

void signal_handler(int signum) {
	printf("SIGINT received, leaving..\n");
	close_server();
	exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
	for(int i = 0; i < SERVER_CLIENT_LIMIT; i++) {
		clients[i].id = i;
		clients[i].socket_fd = -1;
		clients[i].registered = 0;
		clients[i].active = 0;
		clients[i].name[0] = '\0';
	}

	atexit(close_server); // moze trzeba bedzie zrobic osobna funkcje

	struct sigaction action;
	action.sa_handler = signal_handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGINT, &action, NULL);

	if (argc != 3) {
		fprintf(stderr, "Wrong arguments number!\n");
		exit(EXIT_FAILURE);
	}

	inet_port = (in_port_t) atoi(argv[1]);
	unix_socket_path_name = argv[2];

	open_server();

	pthread_t connection_thd;
	pthread_create(&connection_thd, NULL, connection_manager, NULL);

	pthread_t ping_thd;
	pthread_create(&ping_thd, NULL, ping_manager, NULL);

	pthread_join(connection_thd, NULL);
	pthread_join(ping_thd, NULL);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////

void send_message(int socket_fd, char* message) {
	printf("[SERVER] Sending message: %s, to: %d\n", message, socket_fd);
	if (write(socket_fd, message, strlen(message)) == -1) {
		perror("write");
		exit(EXIT_FAILURE);
	}
}

void send_message_to_all(int socket_fd, char* message) {
	for(int i = 0; i < SERVER_CLIENT_LIMIT; i++) {
		if (clients[i].socket_fd != -1 && clients[i].socket_fd != socket_fd) {
			send_message(clients[i].socket_fd, message);
		}
	}
}

void register_client(int socket, char* name) {
	pthread_mutex_lock(&mutex);

	int id = -1;
	for(int i = 0; i < SERVER_CLIENT_LIMIT; i++) {
		if (clients[i].socket_fd == -1) {
			id = i;
			break;
		}
	}
	if (id == -1) {
		printf("[SERVER] client limit reached\n");
		// send_message(socket, "ERROR\n");
		pthread_mutex_unlock(&mutex);
		return;
	}

	clients[id].socket_fd = socket;
	clients[id].registered = 1;
	clients[id].active = 1;
	clients[id].id = id;
	strcpy(clients[id].name, name);
	registered_clients_num++;

	printf("[SERVER] Client %s registered\n", name);

	pthread_mutex_unlock(&mutex);
}

void remove_client(int socket_fd, int lock) {
	if (lock) {
		pthread_mutex_lock(&mutex);
	}

	for(int i = 0; i < SERVER_CLIENT_LIMIT; i++) {
		if (clients[i].socket_fd == socket_fd) {
			if (clients[i].registered) {
				printf("[SERVER] Client %s disconnected\n", clients[i].name);
				// send_message(socket_fd, "DISCONECTED\n");
			}
			clients[i].socket_fd = -1;
			clients[i].registered = 0;
			clients[i].active = 0;
			clients[i].name[0] = '\0';
			registered_clients_num--;
			break;
		}
	}

	if (lock) {
		pthread_mutex_unlock(&mutex);
	}
}

void process_message(int socket, char* msg) {
	printf("[SERVER] Received: %s, from %d\n", msg, socket);
	int type;
	char* content = "";

	int client_id = -1;
	for (int i = 0; i < SERVER_CLIENT_LIMIT; i++) {
		if (clients[i].registered && clients[i].socket_fd == socket) {
			client_id = i;
			break;
		}
	}

	char* msg_tmp = strdup(msg);
	char* token = strtok(msg_tmp, " ");
	if (token != NULL) {
		type = atoi(token);
		token = strtok(NULL, "");
		if (token != NULL) {
			content = token;
		}
	}

	if(type == REGISTER) {
		register_client(socket, content);
		send_message(socket, "OK");
		char new_client[MSG_SIZE];
		sprintf(new_client, "4 [SERVER] '%s' joined\n", content);
		send_message_to_all(socket, new_client);
	}
	else if(type == PING) {
		for (int i = 0; i < SERVER_CLIENT_LIMIT; i++) {
			if (clients[i].socket_fd == socket) {
				clients[i].active = 1;
				break;
			}
		}
	}
	else if(type == LIST) {
		char list[MSG_SIZE] = "2 ";
		for(int i = 0; i < SERVER_CLIENT_LIMIT; i++) {
			if (clients[i].registered) {
				char client[MSG_SIZE];
				sprintf(client, "ID: %d %s\n", clients[i].id, clients[i].name);
				strcat(list, client);
			}
		}
		send_message(socket, list);
	}
	else if(type == TONE) {
		int catcher_id;
		char* msg_tmp = strdup(content);
		char* token = strtok(msg_tmp, " ");
		if (token != NULL) {
			catcher_id = atoi(token);
			token = strtok(NULL, "\0");
			if (token != NULL) {
				content = token;
			}
		}
		if (!clients[catcher_id].registered) {
			printf("[SERVER] Client %d is not registered\n", catcher_id);
			msg_tmp = "3 Client is not registered\n";
			send_message(socket, msg_tmp);
			return;
		}
		char one_msg[MSG_SIZE];
		snprintf(one_msg, MSG_SIZE, "3 [PRIVATE] %s: %s", clients[client_id].name, content);
		send_message(clients[catcher_id].socket_fd, one_msg);
	}
	else if(type == TALL) {
		char all_msg[MSG_SIZE];
		snprintf(all_msg, MSG_SIZE, "4 [ALL] %s: %s", clients[client_id].name, content);
		send_message_to_all(socket, all_msg);
	}
	else if(type == STOP) {
		remove_client(socket, 1);
	}
	else {
		printf("[SERVER] Unknown message type\n");
	}

	free(msg_tmp);
	memset(msg, 0, MSG_SIZE);
}

/////////////////////////////////////////////////////////////////////////////////////

void* connection_manager(void* arg) {
	struct pollfd sockets[SERVER_CLIENT_LIMIT + 2];

	sockets[SERVER_CLIENT_LIMIT + 1].fd = server_inet_socket_fd;
	sockets[SERVER_CLIENT_LIMIT + 1].events = POLLIN;

	sockets[SERVER_CLIENT_LIMIT].fd = server_unix_socket_fd;
	sockets[SERVER_CLIENT_LIMIT].events = POLLIN;

	while(1) {
		for(int i = 0; i < SERVER_CLIENT_LIMIT; i++) {
			sockets[i].fd = clients[i].socket_fd;
			sockets[i].events = POLLIN;
		}
		for(int i = 0; i < SERVER_CLIENT_LIMIT + 2; i++) {
			sockets[i].revents = 0;
		}

		poll(sockets, SERVER_CLIENT_LIMIT + 2, -1);

		for(int i = 0; i < SERVER_CLIENT_LIMIT; i++) {
			if(sockets[i].revents & POLLHUP) {
				remove_client(sockets[i].fd, 1);
				sockets[i].revents = 0;
			}
		}

		int socket = -1;
		for (int i = 0; i < SERVER_CLIENT_LIMIT + 2; i++) {
			if (sockets[i].revents & POLLIN) {
				socket = sockets[i].fd;
				break;
			}
		}

		if(socket != -1) {
			if (socket == server_unix_socket_fd) {
				printf("[SERVER] UNIX socket connection request\n");
			}
			else if (socket == server_inet_socket_fd) {
				printf("[SERVER] INET socket connection request\n");
			}
			else {
				char msg_buffer[MSG_SIZE];
				read(socket, msg_buffer, MSG_SIZE);
				process_message(socket, msg_buffer);
				continue;
			}

			pthread_mutex_lock(&mutex);

			int client_id = -1;
			for(int i = 0; i < SERVER_CLIENT_LIMIT; i++) {
				if(clients[i].socket_fd == -1) {
					client_id = i;
					break;
				}
			}
			if(client_id == -1) {
				printf("[SERVER] No free client slots\n");
				send_message(socket, "NO");
				continue;
			}

			int client_socket_fd = accept(socket, NULL, 0);

			if(client_socket_fd == -1) {
				perror("accept");
				exit(EXIT_FAILURE);
			}

			printf("[SERVER] Client %d connected\n", client_id);
			clients[client_id].socket_fd = client_socket_fd;
			write(client_socket_fd, "OK", 3);

			pthread_mutex_unlock(&mutex);
		}
	}

	return NULL;
}

void* ping_manager(void* arg) {
	while(1) {
		sleep(PING_INTERVAL);
		pthread_mutex_lock(&mutex);

		for (int i =  0; i < SERVER_CLIENT_LIMIT; i++) {
			if (clients[i].registered) {
				clients[i].active = 0;
				send_message(clients[i].socket_fd, "1 PING");
			}
		}

		pthread_mutex_unlock(&mutex);

		sleep(PING_TIMEOUT);

		pthread_mutex_lock(&mutex);

		for (int i = 0; i < SERVER_CLIENT_LIMIT; i++) {
			if (clients[i].registered && !clients[i].active) {
				remove_client(clients[i].socket_fd, 0);
			}
		}

		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}