#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sd;
	FILE *fp;
	
	char buf[BUF_SIZE];
	char file_name[BUF_SIZE];
	char file_name2[BUF_SIZE];
	int read_cnt, i;
	struct sockaddr_in serv_adr;

	if(argc!=3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sd=socket(PF_INET, SOCK_STREAM, 0);

	if(sd == -1)	
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

	if(connect(sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error");

	for(i=0;i<5;i++){ // 서버에서 5회 파일 수신

		printf("input server file name: ");
		scanf("%s", file_name);
	
		fp = fopen(file_name, "rb");

		write(sd, file_name, strlen(file_name)+1); // 서버에 파일 전송 요청	

		while(1){
			if((read_cnt = read(sd, buf, BUF_SIZE)) != 0){ // 파일 존재할 경우
			printf("client file name: ");
			scanf("%s",file_name2);
			fp = fopen(file_name2, "wb");
			fwrite((void*)buf, 1, read_cnt, fp);
			fclose(fp);
			break;
			}

			else{ // 존재 하지 않을 경우 종료
				close(sd);
				return 0;
			}
		}
	}
	close(sd);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

