#ifndef _WAFE_H_
#define _WAFE_H_
/*
** Winamp frontend/plug-in control API documentation v1.0.
** By Justin Frankel.
** Copyright (C) 1997-1999, Nullsoft Inc.
** Last updated: OCT.5.1999.
**
** Introduction
** -----------------------
** This file describes a means to easily communicate to Winamp
** via the classic Win32 Message API.
**
** These definitions/code assume C/C++. Porting to VB/Delphi shouldn't
** be too hard.
**
** First, you find the HWND of the Winamp main window. From a plug-in
** you can easily extract this from the plug-in structure (hMainWindow,
** hwndParent, whatever). For external apps, use:
**
** HWND hwnd_winamp = FindWindow("Winamp v1.x",NULL);
**
** (note: I know, we're in Winamp 2.x, but it's 1.x for compatibility)
**
** Once you have the hwnd_winamp, it's a good idea to check the version
** number. To do this, you send a WM_WA_IPC message to hwnd_winamp.
** Note that WM_WA_IPC is defined as Win32's WM_USER.
**
** Note that sometimes you might want to use PostMessage instead of
** SendMessage.
*/

#define WM_WA_IPC WM_USER

/**************************************************************************/

#define IPC_GETVERSION 0

/*
** int version = SendMessage(hwnd_winamp,WM_WA_IPC,0,IPC_GETVERSION);
**
** Version will be 0x20yx for winamp 2.yx. versions previous to Winamp 2.0
** typically (but not always) use 0x1zyx for 1.zx versions. Weird, I know.
**
** The basic format for sending messages to Winamp is:
** int result=SendMessage(hwnd_winamp,WM_WA_IPC,command_data,command);
** (for the version check, command_data is 0).
*/


#define IPC_DELETE 101

/*
** SendMessage(hwnd_winamp,WM_WA_IPC,0,IPC_DELETE);
**
** You can use IPC_DELETE to clear Winamp's internal playlist.
*/


#define IPC_STARTPLAY 102

/*
** SendMessage(hwnd_winamp,WM_WA_IPC,0,IPC_STARTPLAY);
**
** Using IPC_STARTPLAY is like hitting 'Play' in Winamp, mostly.
*/


#define IPC_ISPLAYING 104

/*
** int res = SendMessage(hwnd_winamp,WM_WA_IPC,0,IPC_ISPLAYING);
**
** IPC_ISPLAYING returns the status of playback.
** If it returns 1, it is playing. if it returns 3, it is paused,
** if it returns 0, it is not playing.
*/


#define IPC_GETOUTPUTTIME 105

/*
** int res = SendMessage(hwnd_winamp,WM_WA_IPC,mode,IPC_GETOUTPUTTIME);
**
** IPC_GETOUTPUTTIME returns the position in milliseconds of the
** current song (mode = 0), or the song length, in seconds (mode = 1).
** Returns -1 if not playing or error.
*/


#define IPC_JUMPTOTIME 106

/* (requires Winamp 1.60+)
** SendMessage(hwnd_winamp,WM_WA_IPC,ms,IPC_JUMPTOTIME);
** IPC_JUMPTOTIME sets the position in milliseconds of the
** current song (approximately).
** Returns -1 if not playing, 1 on eof, or 0 if successful
*/


#define IPC_WRITEPLAYLIST 120

/* (requires Winamp 1.666+)
** SendMessage(hwnd_winamp,WM_WA_IPC,0,IPC_WRITEPLAYLIST);
**
** IPC_WRITEPLAYLIST writes the current playlist to <winampdir>\\Winamp.m3u,
** and returns the current playlist position.
** Kinda obsoleted by some of the 2.x new stuff, but still good for when
** using a front-end (instead of a plug-in)
*/


#define IPC_SETPLAYLISTPOS 121

/* (requires Winamp 2.0+)
** SendMessage(hwnd_winamp,WM_WA_IPC,position,IPC_SETPLAYLISTPOS)
**
** IPC_SETPLAYLISTPOS sets the playlsit position to 'position'.
*/


#define IPC_SETVOLUME 122

/* (requires Winamp 2.0+)
** SendMessage(hwnd_winamp,WM_WA_IPC,volume,IPC_SETVOLUME);
**
** IPC_SETVOLUME sets the volume of Winamp (from 0-255).
*/


#define IPC_SETPANNING 123

/* (requires Winamp 2.0+)
** SendMessage(hwnd_winamp,WM_WA_IPC,panning,IPC_SETPANNING);
**
** IPC_SETPANNING sets the panning of Winamp (from 0 (left) to 255 (right)).
*/


#define IPC_GETLISTLENGTH 124

/* (requires Winamp 2.0+)
** int length = SendMessage(hwnd_winamp,WM_WA_IPC,0,IPC_GETLISTLENGTH);
**
** IPC_GETLISTLENGTH returns the length of the current playlist, in
** tracks.
*/


#define IPC_SETSKIN 200

/* (requires Winamp 2.04+, only usable from plug-ins (not external apps))
** SendMessage(hwnd_winamp,WM_WA_IPC,(WPARAM)"skinname",IPC_SETSKIN);
**
** IPC_SETSKIN sets the current skin to "skinname". Note that skinname
** can be the name of a skin, a skin .zip file, with or without path.
** If path isn't specified, the default search path is the winamp skins
** directory.
*/


#define IPC_GETSKIN 201

/* (requires Winamp 2.04+, only usable from plug-ins (not external apps))
** SendMessage(hwnd_winamp,WM_WA_IPC,(WPARAM)skinname_buffer,IPC_GETSKIN);
**
** IPC_GETSKIN puts the directory where skin bitmaps can be found
** into  skinname_buffer.
** skinname_buffer must be MAX_PATH characters in length.
** When using a .zip'd skin file, it'll return a temporary directory
** where the ZIP was decompressed.
*/


#define IPC_EXECPLUG 202

/* (requires Winamp 2.04+, only usable from plug-ins (not external apps))
** SendMessage(hwnd_winamp,WM_WA_IPC,(WPARAM)"vis_file.dll",IPC_EXECPLUG);
**
** IPC_EXECPLUG executes a visualization plug-in pointed to by WPARAM.
** the format of this string can be:
** "vis_whatever.dll"
** "vis_whatever.dll,0" // (first mod, file in winamp plug-in dir)
** "C:\\dir\\vis_whatever.dll,1"
*/


#define IPC_GETPLAYLISTFILE 211

/* (requires Winamp 2.04+, only usable from plug-ins (not external apps))
** char *name=SendMessage(hwnd_winamp,WM_WA_IPC,index,IPC_GETPLAYLISTFILE);
**
** IPC_GETPLAYLISTFILE gets the filename of the playlist entry [index].
** returns a pointer to it. returns NULL on error.
*/


#define IPC_GETPLAYLISTTITLE 212

/* (requires Winamp 2.04+, only usable from plug-ins (not external apps))
** char *name=SendMessage(hwnd_winamp,WM_WA_IPC,index,IPC_GETPLAYLISTTITLE);
**
** IPC_GETPLAYLISTTITLE gets the title of the playlist entry [index].
** returns a pointer to it. returns NULL on error.
*/


#define IPC_GETLISTPOS 125

/* (requires Winamp 2.05+)
** int pos=SendMessage(hwnd_winamp,WM_WA_IPC,0,IPC_GETLISTPOS);
**
** IPC_GETLISTPOS returns the playlist position. A lot like IPC_WRITEPLAYLIST
** only faster since it doesn't have to write out the list. Heh, silly me.
*/


#define IPC_GETINFO 126

/* (requires Winamp 2.05+)
** int inf=SendMessage(hwnd_winamp,WM_WA_IPC,mode,IPC_GETINFO);
**
** IPC_GETINFO returns info about the current playing song. The value
** it returns depends on the value of 'mode'.
** Mode      Meaning
** ------------------
** 0         Samplerate (i.e. 44100)
** 1         Bitrate  (i.e. 128)
** 2         Channels (i.e. 2)
*/


#define IPC_GETEQDATA 127

/* (requires Winamp 2.05+)
** int data=SendMessage(hwnd_winamp,WM_WA_IPC,pos,IPC_GETEQDATA);
**
** IPC_GETEQDATA queries the status of the EQ.
** The value returned depends on what 'pos' is set to:
** Value      Meaning
** ------------------
** 0-9        The 10 bands of EQ data. 0-63 (+20db - -20db)
** 10         The preamp value. 0-63 (+20db - -20db)
** 11         Enabled. zero if disabled, nonzero if enabled.
** 12         Autoload. zero if disabled, nonzero if enabled.
*/


#define IPC_SETEQDATA 128

/* (requires Winamp 2.05+)
** SendMessage(hwnd_winamp,WM_WA_IPC,pos,IPC_GETEQDATA);
** SendMessage(hwnd_winamp,WM_WA_IPC,value,IPC_SETEQDATA);
**
** IPC_SETEQDATA sets the value of the last position retrieved
** by IPC_GETEQDATA.
*/

/**************************************************************************/

/*
** Some API calls tend to require that you send data via WM_COPYDATA
** instead of WM_USER. Such as IPC_PLAYFILE:
*/

#define IPC_PLAYFILE 100

/*
** COPYDATASTRUCT cds;
** cds.dwData = IPC_PLAYFILE;
** cds.lpData = (void *) "file.mp3";
** cds.cbData = strlen((char *) cds.lpData)+1; // include space for null char
** SendMessage(hwnd_winamp,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds);
**
** This will play the file "file.mp3".
**
*/


#define IPC_CHDIR 103

/*
** COPYDATASTRUCT cds;
** cds.dwData = IPC_CHDIR;
** cds.lpData = (void *) "c:\\download";
** cds.cbData = strlen((char *) cds.lpData)+1; // include space for null char
** SendMessage(hwnd_winamp,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds);
**
** This will make Winamp change to the directory C:\\download
**
*/


/**************************************************************************/

/*
** Finally there are some WM_COMMAND messages that you can use to send
** Winamp misc commands.
**
** To send these, use:
**
** SendMessage(hwnd_winamp, WM_COMMAND,command_name,0);
*/

#define WINAMP_OPTIONS_EQ               40036 // toggles the EQ window
#define WINAMP_OPTIONS_PLEDIT           40040 // toggles the playlist window
#define WINAMP_VOLUMEUP                 40058 // turns the volume up a little
#define WINAMP_VOLUMEDOWN               40059 // turns the volume down a little
#define WINAMP_FFWD5S                   40060 // fast forwards 5 seconds
#define WINAMP_REW5S                    40061 // rewinds 5 seconds

// the following are the five main control buttons, with optionally shift
// or control pressed
#define WINAMP_BUTTON1                  40044 //Previous Track
#define WINAMP_BUTTON2                  40045 //Play
#define WINAMP_BUTTON3                  40046 //Pause
#define WINAMP_BUTTON4                  40047 //Stop
#define WINAMP_BUTTON5                  40048 //Next Track
#define WINAMP_BUTTON1_SHIFT            40144 //rewinds 5 seconds
#define WINAMP_BUTTON2_SHIFT            40145 //Open file and play
#define WINAMP_BUTTON3_SHIFT            40146 //Null
#define WINAMP_BUTTON4_SHIFT            40147 //Fades out and stops
#define WINAMP_BUTTON5_SHIFT            40148 //fast forwards 5 seconds
#define WINAMP_BUTTON1_CTRL             40154 //Starts previous file in the playlist
#define WINAMP_BUTTON2_CTRL             40155 //Open location
#define WINAMP_BUTTON3_CTRL             40156 //Null
#define WINAMP_BUTTON4_CTRL             40157 //Null
#define WINAMP_BUTTON5_CTRL             40158 //Starts next file in the playlist

#define WINAMP_FILE_PLAY                40029 // pops up the load file(s) box
#define WINAMP_OPTIONS_PREFS            40012 // pops up the preferences
#define WINAMP_OPTIONS_AOT              40019 // toggles always on top
#define WINAMP_HELP_ABOUT               40041 // pops up the about box


/*
** EOF.. Enjoy.
*/

#endif

