/*=======================================================================

	ウィンドウ表示処理 [main.cpp]

												長岡佑樹

=========================================================================*/

/////////////////////////////////////////////////////////
//
//	ヘッダファイル
//
/////////////////////////////////////////////////////////
#pragma warning (disable:4786)
#include <windows.h>
#include <time.h>
#include "main.h"
#include "utils.h"
#include "precisionTimer.h"
#include "soccerPitch.h"
#include "cgdi.h"
#include "paramLoader.h"
#include "resource.h"
#include "windowUtils.h"
#include "debugConsole.h"

/////////////////////////////////////////////////////////
//
//	グローバル変数
//
/////////////////////////////////////////////////////////
char* g_szApplicationName = "Simple Soccer";
char*	g_szWindowClassName = "MyWindowClass";

SoccerPitch* g_SoccerPitch;
PrecisionTimer timer(Prm.FrameRate);

/////////////////////////////////////////////////////////
//
//	CheckAllMenuItemsAppropriately関数
//
/////////////////////////////////////////////////////////
void CheckAllMenuItemsAppropriately(HWND hwnd)
{
   CheckMenuItemAppropriately(hwnd, IDM_SHOW_REGIONS, Prm.bRegions);
   CheckMenuItemAppropriately(hwnd, IDM_SHOW_STATES, Prm.bStates);
   CheckMenuItemAppropriately(hwnd, IDM_SHOW_IDS, Prm.bIDs);
   CheckMenuItemAppropriately(hwnd, IDM_AIDS_SUPPORTSPOTS, Prm.bSupportSpots);
   CheckMenuItemAppropriately(hwnd, ID_AIDS_SHOWTARGETS, Prm.bViewTargets);
   CheckMenuItemAppropriately(hwnd, IDM_AIDS_HIGHLITE, Prm.bHighlightIfThreatened);
}

/////////////////////////////////////////////////////////
//
//	WindowProc関数
//
/////////////////////////////////////////////////////////
LRESULT CALLBACK WindowProc (HWND   hwnd,
                             UINT   msg,
                             WPARAM wParam,
                             LPARAM lParam)
{
 
	 static int cxClient, cyClient; 

   static HDC		hdcBackBuffer;
   static HBITMAP	hBitmap;
   static HBITMAP	hOldBitmap;

    switch (msg)
    {
	
    case WM_CREATE:
      {
			   RECT rect;

			   GetClientRect(hwnd, &rect);

			   cxClient = rect.right;
			   cyClient = rect.bottom;

         srand((unsigned) time(NULL));

         

         hdcBackBuffer = CreateCompatibleDC(NULL);

         HDC hdc = GetDC(hwnd);

         hBitmap = CreateCompatibleBitmap(hdc,
                                          cxClient,
                                          cyClient);

			  
			   hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap);

         
         ReleaseDC(hwnd, hdc); 
         
         g_SoccerPitch = new SoccerPitch(cxClient, cyClient); 
         
         CheckAllMenuItemsAppropriately(hwnd);

      }

      break;

    case WM_COMMAND:
      {
        switch(wParam)
        {
          case ID_AIDS_NOAIDS:

            Prm.bStates        = 0;
            Prm.bRegions       = 0;
            Prm.bIDs           = 0;
            Prm.bSupportSpots  = 0;
            Prm.bViewTargets   = 0;

            CheckAllMenuItemsAppropriately(hwnd);

            break;
            
          case IDM_SHOW_REGIONS:

            Prm.bRegions = !Prm.bRegions;

            CheckAllMenuItemsAppropriately(hwnd);

            break;

          case IDM_SHOW_STATES:

            Prm.bStates = !Prm.bStates;

            CheckAllMenuItemsAppropriately(hwnd);

            break;

          case IDM_SHOW_IDS:

            Prm.bIDs = !Prm.bIDs;

            CheckAllMenuItemsAppropriately(hwnd);

            break;


          case IDM_AIDS_SUPPORTSPOTS:

            Prm.bSupportSpots = !Prm.bSupportSpots;

            CheckAllMenuItemsAppropriately(hwnd);

             break;

           case ID_AIDS_SHOWTARGETS:

            Prm.bViewTargets = !Prm.bViewTargets;

            CheckAllMenuItemsAppropriately(hwnd);

             break;
              
           case IDM_AIDS_HIGHLITE:

            Prm.bHighlightIfThreatened = !Prm.bHighlightIfThreatened; 

            CheckAllMenuItemsAppropriately(hwnd);

            break;
            
        }
      }

      break;


    case WM_KEYUP:
      {
        switch(wParam)
        {
           case VK_ESCAPE:
            {             
              SendMessage(hwnd, WM_DESTROY, NULL, NULL);
            }
          
            break;

          case 'R':
            {
               delete g_SoccerPitch;
           
               g_SoccerPitch = new SoccerPitch(cxClient, cyClient);
            }

            break;

          case 'P':
            {
              g_SoccerPitch->TogglePause();
            }

            break;

        }
        
      }

      break;

    
    case WM_PAINT:
      {
 		       
         PAINTSTRUCT ps;
          
         BeginPaint (hwnd, &ps);
         
         gdi->StartDrawing(hdcBackBuffer);
         
         g_SoccerPitch->Render();

         gdi->StopDrawing(hdcBackBuffer);

        

			   BitBlt(ps.hdc, 0, 0, cxClient, cyClient, hdcBackBuffer, 0, 0, SRCCOPY); 
          
         EndPaint (hwnd, &ps);

      }

      break;

		case WM_SIZE:
		  {
			  cxClient = LOWORD(lParam);
			  cyClient = HIWORD(lParam);

			SelectObject(hdcBackBuffer, hOldBitmap);

      DeleteObject(hBitmap); 

      HDC hdc = GetDC(hwnd);

      hBitmap = CreateCompatibleBitmap(hdc,
											cxClient,
											cyClient);

			ReleaseDC(hwnd, hdc);
			
      SelectObject(hdcBackBuffer, hBitmap);

      }

      break;
          
		 case WM_DESTROY:
			 {

         SelectObject(hdcBackBuffer, hOldBitmap);

         DeleteDC(hdcBackBuffer);
         DeleteObject(hBitmap); 
         
				 PostQuitMessage (0);
			 }

       break;

     }

		 return DefWindowProc (hwnd, msg, wParam, lParam);
}

/////////////////////////////////////////////////////////
//
//	WinMain関数
//
/////////////////////////////////////////////////////////
int WINAPI WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR     szCmdLine, 
                    int       iCmdShow)
{

  HWND						hWnd;
    
  WNDCLASSEX     winclass;
		 
  winclass.cbSize        = sizeof(WNDCLASSEX);
  winclass.style         = CS_HREDRAW | CS_VREDRAW;
  winclass.lpfnWndProc   = WindowProc;
  winclass.cbClsExtra    = 0;
  winclass.cbWndExtra    = 0;
  winclass.hInstance     = hInstance;
  winclass.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
  winclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
  winclass.hbrBackground = NULL;
  winclass.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU1);
  winclass.lpszClassName = g_szWindowClassName;
  winclass.hIconSm       = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

  if (!RegisterClassEx(&winclass))
  {
    MessageBox(NULL, "Registration Failed!", "Error", 0);

    return 0;
  }

  hWnd = CreateWindowEx (NULL,                 
                         g_szWindowClassName,  
                         g_szApplicationName,  
                         WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION | WS_SYSMENU,
                         GetSystemMetrics(SM_CXSCREEN)/2 - SCREEN_WIDTH/2,
                         GetSystemMetrics(SM_CYSCREEN)/2 - SCREEN_HEIGHT/2,                    
                         SCREEN_WIDTH,     
                         SCREEN_HEIGHT,    
                         NULL,             
                         NULL,             
                         hInstance,        
                         NULL);            
  if(!hWnd)
  {
    MessageBox(NULL, "CreateWindowEx Failed!", "Error!", 0);
  }
  
  timer.Start();

  MSG msg;

  bool bDone = false;

  while(!bDone)
  {
					
    while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
    {
      if( msg.message == WM_QUIT ) 
      {
	      bDone = true;
      } 

      else 
      {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
      }
    }

    if (timer.ReadyForNextFrame() && msg.message != WM_QUIT)
    {
      g_SoccerPitch->Update(); 
      
      RedrawWindow(hWnd, true);

      Sleep(2);
    }
   					
  }

  delete g_SoccerPitch;

  UnregisterClass( g_szWindowClassName, winclass.hInstance );

  return msg.wParam;
}


