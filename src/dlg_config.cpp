/*
  Copyright (c) 1999 - 2002 Simon Peter <dn.tlp@gmx.net>
  Copyright (c) 2002 Nikita V. Kalaganov <riven@ok.ru>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "plugin.h"

#define STRING_TRUNC	40

extern HINSTANCE myInstance;
extern Config config;
extern FileTypes filetypes;

GuiCtrlTooltip tooltip;

void GuiDlgConfig::open(HWND parent)
{
	config.get(&next);

	DialogBoxParam(myInstance,MAKEINTRESOURCE(IDD_CONFIG),parent,(DLGPROC)DlgProc_Wrapper,(LPARAM)this);

	if (!cancelled)
		config.set(&next);
}

BOOL APIENTRY GuiDlgConfig::DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	//printf("GuiDlgConfig::DlgProc(): Message 0x%08X received.\n",message);
#endif
	TCITEM tci;

	switch (message)
	{
	    case WM_INITDIALOG:

			tab_hwnd = NULL;

			// enable tooltip trigger
			tooltip.trigger(GetDlgItem(hwndDlg,IDC_TOOLTIPS));

			// init tab control
			tci.mask = TCIF_TEXT;

			tci.pszText = "Output";
			SendDlgItemMessage(hwndDlg,IDC_CTABS,TCM_INSERTITEM,0,(LPARAM)&tci);
		
			tci.pszText = "Playback";
			SendDlgItemMessage(hwndDlg,IDC_CTABS,TCM_INSERTITEM,1,(LPARAM)&tci);
		
			tci.pszText = "Formats";
			SendDlgItemMessage(hwndDlg,IDC_CTABS,TCM_INSERTITEM,2,(LPARAM)&tci);

			// set default tab index
			SendDlgItemMessage(hwndDlg,IDC_CTABS,TCM_SETCURSEL,0,0);


		case WM_UPDATE:

			// delete old tab window
			if (tab_hwnd)
			{
				DestroyWindow(tab_hwnd);

				tab_hwnd = NULL;
			}

			// display new tab window
			tab_index = (int)SendDlgItemMessage(hwndDlg,IDC_CTABS,TCM_GETCURSEL,0,0);

			if (tab_index == 0)
				tab_hwnd = CreateDialogParam(myInstance,MAKEINTRESOURCE(IDD_CFG_OUTPUT),GetDlgItem(hwndDlg,IDC_CTABS),(DLGPROC)TabDlgProc_Wrapper,(LPARAM)this);
			if (tab_index == 1)
				tab_hwnd = CreateDialogParam(myInstance,MAKEINTRESOURCE(IDD_CFG_PLAYBACK),GetDlgItem(hwndDlg,IDC_CTABS),(DLGPROC)TabDlgProc_Wrapper,(LPARAM)this);
			if (tab_index == 2)
				tab_hwnd = CreateDialogParam(myInstance,MAKEINTRESOURCE(IDD_CFG_FORMATS),GetDlgItem(hwndDlg,IDC_CTABS),(DLGPROC)TabDlgProc_Wrapper,(LPARAM)this);

			return FALSE;


		case WM_NOTIFY:
			switch (((NMHDR *)lParam)->code)
			{
				case TCN_SELCHANGE:
					PostMessage(hwndDlg,WM_UPDATE,0,0);
					return FALSE;
			}


		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDOK:
					cancelled = false;
					EndDialog(hwndDlg,wParam);
					return 0;

				case IDCANCEL:
					cancelled = true;
					EndDialog(hwndDlg,wParam);
					return 0;
			}
	}

	return FALSE;
}

BOOL APIENTRY GuiDlgConfig::OutputTabDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	//printf("GuiDlgConfig::OutputTabDlgProc(): Message 0x%08X received.\n",message);
#endif
	char bufstr[5];
	unsigned long bufint;
	string bufxstr;

	switch (message)
	{
		case WM_INITDIALOG:

			// add tooltips
			tooltip.add(GetDlgItem(hwndDlg,IDC_FREQ1),      "freq1",      "Set 11 khz frequency");
			tooltip.add(GetDlgItem(hwndDlg,IDC_FREQ2),      "freq2",      "Set 22 khz frequency");
			tooltip.add(GetDlgItem(hwndDlg,IDC_FREQ3),      "freq3",      "Set 44 khz frequency");
			tooltip.add(GetDlgItem(hwndDlg,IDC_FREQ4),      "freq4",      "Set 48 khz frequency");
			tooltip.add(GetDlgItem(hwndDlg,IDC_FREQC),      "freqc",      "Set custom frequency");
			tooltip.add(GetDlgItem(hwndDlg,IDC_FREQC_VALUE),"freqc_value","Specify custom frequency value");
			tooltip.add(GetDlgItem(hwndDlg,IDC_QUALITY8),   "quality8",   "Set 8-bit quality");
			tooltip.add(GetDlgItem(hwndDlg,IDC_QUALITY16),  "quality16",  "Set 16-bit quality");
			tooltip.add(GetDlgItem(hwndDlg,IDC_MONO),       "mono",       "Set mono output");
			tooltip.add(GetDlgItem(hwndDlg,IDC_STEREO),     "stereo",     "Set stereo output");
			tooltip.add(GetDlgItem(hwndDlg,IDC_OUTTS),      "outts",      "Use Tatsuyuki Satoh's emulator output");
			tooltip.add(GetDlgItem(hwndDlg,IDC_OUTKS),      "outks",      "Use Ken Silverman's emulator output");
			tooltip.add(GetDlgItem(hwndDlg,IDC_OUTDISK),    "outdisk",    "Use Disk Writer output.\r\nWrites Rdos' RAW files, they can be replayed by AdPlug).");
			tooltip.add(GetDlgItem(hwndDlg,IDC_OUTOPL2),    "outopl2",    "Use OPL2 chip output");
			tooltip.add(GetDlgItem(hwndDlg,IDC_ADLIBPORT),  "adlibport",  "Specify OPL2 chip port (usually 388)");
			tooltip.add(GetDlgItem(hwndDlg,IDC_NOTEST),     "notest",     "Disable OPL2 chip detection at specified port");
			tooltip.add(GetDlgItem(hwndDlg,IDC_DIRECTORY),  "directory",  "Select output directory for Disk Writer");

			// set "output"
			if (next.useoutput == emuts)
				CheckRadioButton(hwndDlg,IDC_OUTTS,IDC_OUTOPL2,IDC_OUTTS);
			else if (next.useoutput == emuks)
				CheckRadioButton(hwndDlg,IDC_OUTTS,IDC_OUTOPL2,IDC_OUTKS);
			else if (next.useoutput == opl2)
				CheckRadioButton(hwndDlg,IDC_OUTTS,IDC_OUTOPL2,IDC_OUTOPL2);
			else //if (next.useoutput == disk)
				CheckRadioButton(hwndDlg,IDC_OUTTS,IDC_OUTOPL2,IDC_OUTDISK);

			// set "frequency"
			if (next.replayfreq == 11025)
				CheckRadioButton(hwndDlg,IDC_FREQ1,IDC_FREQC,IDC_FREQ1);
			else if (next.replayfreq == 22050)
				CheckRadioButton(hwndDlg,IDC_FREQ1,IDC_FREQC,IDC_FREQ2);
			else if (next.replayfreq == 44100)
				CheckRadioButton(hwndDlg,IDC_FREQ1,IDC_FREQC,IDC_FREQ3);
			else if (next.replayfreq == 48000)
				CheckRadioButton(hwndDlg,IDC_FREQ1,IDC_FREQC,IDC_FREQ4);
			else
			{
				CheckRadioButton(hwndDlg,IDC_FREQ1,IDC_FREQC,IDC_FREQC);
				SetDlgItemInt(hwndDlg,IDC_FREQC_VALUE,next.replayfreq,FALSE);
			}

			// set "resolution"
			if (next.use16bit)
				CheckRadioButton(hwndDlg,IDC_QUALITY8,IDC_QUALITY16,IDC_QUALITY16);
			else
				CheckRadioButton(hwndDlg,IDC_QUALITY8,IDC_QUALITY16,IDC_QUALITY8);

			// set "channels"
			if (next.stereo)
				CheckRadioButton(hwndDlg,IDC_MONO,IDC_STEREO,IDC_STEREO);
			else
				CheckRadioButton(hwndDlg,IDC_MONO,IDC_STEREO,IDC_MONO);

			// set "port"
			SetDlgItemText(hwndDlg,IDC_ADLIBPORT,_itoa(next.adlibport,bufstr,16));

			// set "no-opl2-test"
			if (!next.testopl2)
				CheckDlgButton(hwndDlg,IDC_NOTEST,BST_CHECKED);

			// set "directory"
			bufxstr = tmpxdiskdir = next.diskdir;
			if (bufxstr.size() > STRING_TRUNC)
			{
				bufxstr.resize(STRING_TRUNC);
				bufxstr.append("...");
			}
			SetDlgItemText(hwndDlg,IDC_DIRECTORY,bufxstr.c_str());

			// move tab content on top
			SetWindowPos(hwndDlg,HWND_TOP,3,22,0,0,SWP_NOSIZE);

			return FALSE;


		case WM_DESTROY:

			if (cancelled)
			{
				next.diskdir = tmpxdiskdir;
				return 0;
			}

			// check "frequency"
			if (IsDlgButtonChecked(hwndDlg,IDC_FREQ1) == BST_CHECKED)
				next.replayfreq = 11025;
			else if (IsDlgButtonChecked(hwndDlg,IDC_FREQ2) == BST_CHECKED)
				next.replayfreq = 22050;
			else if (IsDlgButtonChecked(hwndDlg,IDC_FREQ3) == BST_CHECKED)
				next.replayfreq = 44100;
			else if (IsDlgButtonChecked(hwndDlg,IDC_FREQ4) == BST_CHECKED)
				next.replayfreq = 48000;
			else
				next.replayfreq = GetDlgItemInt(hwndDlg,IDC_FREQC_VALUE,NULL,FALSE);

			// check "resolution"
			if (IsDlgButtonChecked(hwndDlg,IDC_QUALITY16) == BST_CHECKED)
				next.use16bit = true;
			else
				next.use16bit = false;

			// check " channels"
			if (IsDlgButtonChecked(hwndDlg,IDC_STEREO) == BST_CHECKED)
				next.stereo = true;
			else
				next.stereo = false;

			// check "output"
			if (IsDlgButtonChecked(hwndDlg,IDC_OUTTS) == BST_CHECKED)
				next.useoutput = emuts;
			else if (IsDlgButtonChecked(hwndDlg,IDC_OUTKS) == BST_CHECKED)
				next.useoutput = emuks;
			else if (IsDlgButtonChecked(hwndDlg,IDC_OUTOPL2) == BST_CHECKED)
				next.useoutput = opl2;
			else //if (IsDlgButtonChecked(hwndDlg,IDC_OUTDISK) == BST_CHECKED)
				next.useoutput = disk;

			// check "port"
			GetDlgItemText(hwndDlg,IDC_ADLIBPORT,bufstr,5);
			sscanf(bufstr,"%x",&bufint);
			next.adlibport = (unsigned short)bufint;

			// check "options"
			next.testopl2 = !(IsDlgButtonChecked(hwndDlg,IDC_NOTEST) == BST_CHECKED);

			return 0;


		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_DIRECTORY:

					// display folder selection dialog
					char shd[_MAX_PATH];

					BROWSEINFO bi;

					bi.hwndOwner = hwndDlg;
					bi.pidlRoot = NULL;
					bi.pszDisplayName = shd;
					bi.lpszTitle = "Select output path for Disk Writer:";
					bi.ulFlags = BIF_RETURNONLYFSDIRS;
					bi.lpfn = NULL;
					bi.lParam = 0;
					bi.iImage = 0;

					if (SHGetPathFromIDList(SHBrowseForFolder(&bi),shd))
					{
						bufxstr = next.diskdir = shd;
						if (bufxstr.size() > STRING_TRUNC)
						{
							bufxstr.resize(STRING_TRUNC);
							bufxstr.append("...");
						}
						SetDlgItemText(hwndDlg,IDC_DIRECTORY,bufxstr.c_str());
					}

					return 0;
			}
	}

	return FALSE;
}

BOOL APIENTRY GuiDlgConfig::PlaybackTabDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	string bufxstr;

#ifdef _DEBUG
	//printf("GuiDlgConfig::PlaybackTabDlgProc(): Message 0x%08X received.\n",message);
#endif
	switch (message)
	{
		case WM_INITDIALOG:

			// add tooltips
			tooltip.add(GetDlgItem(hwndDlg,IDC_TESTLOOP),"autoend" ,"Enable detection of song looping." );
			tooltip.add(GetDlgItem(hwndDlg,IDC_FASTSEEK),"fastseek","Enable fast seek for OPL2 chip output.\r\nThis can results in inaccurate replaying.");
			tooltip.add(GetDlgItem(hwndDlg,IDC_STDTIMER),"stdtimer","Use default playing speed for Disk Writer output.\r\nDisable, if you wanna quick song writing.\r\n\r\nNever disable, if you disabled song looping detection!");
			tooltip.add(GetDlgItem(hwndDlg,IDC_PRIORITY),"priority","Set playing thread priority, like in Winamp.");
			tooltip.add(GetDlgItem(hwndDlg,IDC_DATABASE),"database","Set database to be used for file information.");
			tooltip.add(GetDlgItem(hwndDlg,IDC_USEDB),"usedb","If checked, the database will be used.");

			// set checkboxes
			if (next.testloop)
				CheckDlgButton(hwndDlg,IDC_TESTLOOP,BST_CHECKED);
			if (next.fastseek)
				CheckDlgButton(hwndDlg,IDC_FASTSEEK,BST_CHECKED);
			if (next.stdtimer)
				CheckDlgButton(hwndDlg,IDC_STDTIMER,BST_CHECKED);
			if (next.usedb)
				CheckDlgButton(hwndDlg,IDC_USEDB,BST_CHECKED);

			// set "priority"
			SendDlgItemMessage(hwndDlg,IDC_PRIORITY,TBM_SETRANGE,(WPARAM)FALSE,(LPARAM)MAKELONG(1,7));
			SendDlgItemMessage(hwndDlg,IDC_PRIORITY,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)next.priority);

			// set "database"
			bufxstr = next.db_file;
			if (bufxstr.size() > STRING_TRUNC)
			{
				bufxstr.resize(STRING_TRUNC);
				bufxstr.append("...");
			}
			SetDlgItemText(hwndDlg,IDC_DATABASE,bufxstr.c_str());

			// move tab content on top
			SetWindowPos(hwndDlg,HWND_TOP,3,22,0,0,SWP_NOSIZE);

			return FALSE;


		case WM_DESTROY:

			if (cancelled)
				return 0;

			// check checkboxes :)
			next.testloop = (IsDlgButtonChecked(hwndDlg,IDC_TESTLOOP) == BST_CHECKED);
			next.fastseek = (IsDlgButtonChecked(hwndDlg,IDC_FASTSEEK) == BST_CHECKED);
			next.stdtimer = (IsDlgButtonChecked(hwndDlg,IDC_STDTIMER) == BST_CHECKED);
			next.usedb = (IsDlgButtonChecked(hwndDlg,IDC_USEDB) == BST_CHECKED);

			// check "priority"
			next.priority = (int)SendDlgItemMessage(hwndDlg,IDC_PRIORITY,TBM_GETPOS,0,0);

			return 0;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_DATABASE:
					OPENFILENAME ofn;

					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = hwndDlg;
					ofn.lpstrFilter = "AdPlug Database Files (*.DB)\0*.db\0";
					ofn.lpstrCustomFilter = NULL;
					ofn.nFilterIndex = 0;
					ofn.lpstrFile = (LPSTR)malloc(_MAX_PATH);
					strcpy(ofn.lpstrFile, next.db_file.c_str());
					ofn.nMaxFile = _MAX_PATH;
					ofn.lpstrFileTitle = NULL;
					ofn.lpstrInitialDir = NULL;
					ofn.lpstrTitle = "Select Database File";
					ofn.Flags = OFN_FILEMUSTEXIST;
					ofn.lpstrDefExt = NULL;

					if (GetOpenFileName(&ofn))
					{
						bufxstr = next.db_file = ofn.lpstrFile;
						if (bufxstr.size() > STRING_TRUNC)
						{
							bufxstr.resize(STRING_TRUNC);
							bufxstr.append("...");
						}
						SetDlgItemText(hwndDlg,IDC_DATABASE,bufxstr.c_str());
					}

					return 0;
			}
	}

	return FALSE;
}

BOOL APIENTRY GuiDlgConfig::FormatsTabDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	//printf("GuiDlgConfig::FormatsTabDlgProc(): Message 0x%08X received.\n",message);
#endif
	int i;

	switch (message)
	{
		case WM_INITDIALOG:

			// add tooltips
			tooltip.add(GetDlgItem(hwndDlg,IDC_FORMATLIST),"formatlist","All replayable formats listed here.\r\nDeselected formats will be ignored.");
			tooltip.add(GetDlgItem(hwndDlg,IDC_FTSELALL),  "ftselall",  "Select all formats.");
			tooltip.add(GetDlgItem(hwndDlg,IDC_FTDESELALL),"ftdeselall","Deselect all formats.");

			// fill listbox
			for (i=0;i<filetypes.get_size();i++)
			{
				SendDlgItemMessage(hwndDlg,IDC_FORMATLIST,LB_ADDSTRING,0,(LPARAM)filetypes.get_name(i));
				SendDlgItemMessage(hwndDlg,IDC_FORMATLIST,LB_SETSEL,(WPARAM)!filetypes.get_ignore(i),i);
			}

			// move tab content on top
			SetWindowPos(hwndDlg,HWND_TOP,3,22,0,0,SWP_NOSIZE);

			return FALSE;


		case WM_DESTROY:

			if (cancelled)
				return 0;

			// read listbox
			for (i=0;i<filetypes.get_size();i++)
				filetypes.set_ignore(i,SendDlgItemMessage(hwndDlg,IDC_FORMATLIST,LB_GETSEL,i,0) ? false : true);

			return 0;


		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_FTSELALL:
					SendDlgItemMessage(hwndDlg,IDC_FORMATLIST,LB_SETSEL,TRUE,-1);
					return 0;

				case IDC_FTDESELALL:
					SendDlgItemMessage(hwndDlg,IDC_FORMATLIST,LB_SETSEL,FALSE,-1);
					return 0;
			}
	}

	return FALSE;
}

BOOL APIENTRY GuiDlgConfig::DlgProc_Wrapper(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_INITDIALOG)
		SetWindowLong(hwndDlg,GWL_USERDATA,(LONG)lParam);

	GuiDlgConfig *the = (GuiDlgConfig *)GetWindowLong(hwndDlg,GWL_USERDATA);

	if (!the)
		return FALSE;

	return the->DlgProc(hwndDlg,message,wParam,lParam);
}

BOOL APIENTRY GuiDlgConfig::TabDlgProc_Wrapper(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_INITDIALOG)
		SetWindowLong(hwndDlg,GWL_USERDATA,(LONG)lParam);

	GuiDlgConfig *the = (GuiDlgConfig *)GetWindowLong(hwndDlg,GWL_USERDATA);

	if (!the)
		return FALSE;

	if (the->tab_index == 0)
		return the->OutputTabDlgProc(hwndDlg,message,wParam,lParam);
	if (the->tab_index == 1)
		return the->PlaybackTabDlgProc(hwndDlg,message,wParam,lParam);
	//if (the->tab_index == 2)
		return the->FormatsTabDlgProc(hwndDlg,message,wParam,lParam);
}