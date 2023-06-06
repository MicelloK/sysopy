#include "utils.h"

void* input_handler(void* arg);
void* connection_handler(void* arg);

char* client_name;
short socket_mode;
int server_socket_fd = -1;

int wait = 0;

char* serrver_IP_addres;
in_port_t server_port_number;
char* server_unix_socket_path;

void signal_handler(int signum) {
    printf("Signal %d received, leaving..\n", signum);
    write(server_socket_fd, "5 STOP", 6);
    exit(EXIT_SUCCESS);
}

void connect_to_server() {
    if (socket_mode == INET_MODE) {
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = inet_addr(serrver_IP_addres);
        server_address.sin_port = htons(server_port_number);

        if ((server_socket_fd = socket(AF_INET, SOCKET_PROTOCOL, 0)) == -1) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        if (connect(server_socket_fd, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
            perror("connect");
            exit(EXIT_FAILURE);
        }
    } 
    else {
        struct sockaddr_un server_address;
        server_address.sun_family = AF_UNIX;
        strcpy(server_address.sun_path, server_unix_socket_path);

        if ((server_socket_fd = socket(AF_UNIX, SOCKET_PROTOCOL, 0)) == -1) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        if (connect(server_socket_fd, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
            perror("connect");
            exit(EXIT_FAILURE);
        }
    }
}

void process_message() {
    char* msg = calloc(MSG_SIZE, sizeof(char));
    if (read(server_socket_fd, msg, MSG_SIZE) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    int type;
	char* content = "";
    // printf("Q%sQ\n", msg);

	char* msg_tmp = strdup(msg);
	char* token = strtok(msg_tmp, " ");
	if (token != NULL) {
		type = atoi(token);
		token = strtok(NULL, "\0");
		if (token != NULL) {
			content = token;
		}
	}
    // printf("A%sA%sA\n", msg, content);

    if (type == PING) {
        write(server_socket_fd, "1 PONG", 6); //ping
    }
    else if (type == STOP) {
        exit(EXIT_SUCCESS);
    }
    else if (type == TONE || type == TALL || type == LIST) {
        printf("%s", content);
        wait = 0;
    }
    else {
        printf("Unknown message type\n");
        wait = 0;
    }

    free(msg);
}

int main(int argc, char** argv) {
    struct sigaction action;
	action.sa_handler = signal_handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGINT, &action, NULL);

    if (argc < 4) {
        printf("Wrong number of arguments\n");
        exit(EXIT_FAILURE);
    }

    client_name = argv[1];

    if (strcmp(argv[2], "INET") == 0 || strcmp(argv[2], "inet") == 0) {
        if (argc != 5) {
            printf("Wrong number of arguments\n");
            exit(EXIT_FAILURE);
        }
        socket_mode = INET_MODE;
        serrver_IP_addres = argv[3];
        server_port_number = (in_port_t) atoi(argv[4]);
    } 
    else if (strcmp(argv[2], "UNIX") == 0 || strcmp(argv[2], "unix") == 0) {
        socket_mode = UNIX_MODE;
        server_unix_socket_path = argv[3];
    }
    else {
        printf("Wrong socket mode\n");
        exit(EXIT_FAILURE);
    }

    connect_to_server();

    char msg[MSG_SIZE];
    read(server_socket_fd, msg, MSG_SIZE);
    
    if (strcmp(msg, "OK") == 0) {
        printf("Connected to server\n");
    }
    else {
        printf("Error while connecting to server\n");
        exit(EXIT_FAILURE);
    }

    //login msg

    char login_msg[MSG_SIZE];
    sprintf(login_msg, "%d %s", REGISTER, client_name);
    write(server_socket_fd, login_msg, MSG_SIZE);

    read(server_socket_fd, msg, MSG_SIZE);
    if (strcmp(msg, "OK") == 0) {
        printf("Logged in\n");
    }
    else {
        printf("Error while logging in\n");
        exit(EXIT_FAILURE);
    }

    pthread_t connection_thd, input_thd;
    pthread_create(&connection_thd, NULL, connection_handler, NULL);
    pthread_create(&input_thd, NULL, input_handler, NULL);

    pthread_join(connection_thd, NULL);
    pthread_join(input_thd, NULL);

    return 0;
}

void* connection_handler(void* arg) {
    while(1) {
        process_message();
    }
}

void* input_handler(void* arg) {
    while(1) {
        // if (!wait) {
        //     printf("> ");
        //     wait = 1;
        // }
        char* line = NULL;
        size_t len = 0;
        getline(&line, &len, stdin);
        if (strcmp(line, "\n") == 0) {
            // wait = 0;
            continue;
        }
        char* line_tmp = strdup(line);
        char* command = strtok(line_tmp, " ");

        if (strcmp(command, "LIST\n") == 0 || strcmp(command, "LIST") == 0) {
            write(server_socket_fd, "2", 1); //list
        }
        else if (strcmp(command, "TALL") == 0) {
            command = strtok(NULL, "\0");
			char message[MSG_SIZE] = "3 ";
            strcat(message, command);
            write(server_socket_fd, message, strlen(message)+1); //tall
        }
        else if (strcmp(command, "TONE") == 0) {
            command = strtok(NULL, "\0");
            char message[MSG_SIZE] = "4 ";
            strcat(message, command);
            write(server_socket_fd, message, strlen(message)+1); //tone
        }
        else if (strcmp(command, "STOP\n") == 0 || strcmp(command, "STOP") == 0) {
            write(server_socket_fd, "5", 1); //stop
            exit(EXIT_SUCCESS);
        }
        else {
            printf("Unknown command\n");
            // wait = 0;
        }
    }
}



