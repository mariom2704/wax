#include "stdafx.h"
#include <string>
#include <mxutil/unicode_defs.h>
#include "interface.h"
#include <mxodbc/dbconnection.h>


#include "../../rohrkonst/cadwerkauftrag.h"
#include "../../waxgl/waxgrafik.h"



extern "C" LPCSTR PASCAL EXPORT dummy()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DBConnection userdb;
	static char x[2048];
	strcpy(x, userdb.create_connectstring(".\\waxuser.mdb").c_str());
	return x;
}

extern "C" bool PASCAL EXPORT convert_init_waxsys()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	LoadLibrary("mxparam.dll");
	LoadLibrary("Waxstamm.dll");
	LoadLibrary("rohrkonst.dll");
	LoadLibrary("waxgl.dll");
	LoadLibrary("xerces-c_2_8.dll");

	TRY
	{
#ifdef DEBUG
		tstring db = "D:\\work\\bin\\WaxGlServer\\Debug32\\waxuser.mdb";
#else
		tstring db = ".\\waxuser.mdb";
#endif

		DBConnection userdb;
		WaxSys::load(userdb.create_connectstring(db).c_str());
	}
	CATCH_ALL(e)
	{
		return false;
	}
	END_CATCH_ALL
	return true;
}


extern "C" bool PASCAL EXPORT convert_init_stammdb()
{
	TRY
	{
#ifdef DEBUG
		tstring db = "D:\\work\\bin\\WaxGlServer\\Debug32\\waxstamm.mdb";
#else
		tstring db = ".\\waxstamm.mdb";
#endif
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		DBConnection stammdb;
		getStammdaten().loadData(stammdb.create_connectstring(db).c_str());
	}
	CATCH_ALL(e)
	{
		return false;
	}
	END_CATCH_ALL

	return true;
}



extern "C" LPCSTR PASCAL EXPORT convert(LPCSTR cadmodell_xml, int typ )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#define MAXSIZEOFGLFILE 64000
	static char glchar[MAXSIZEOFGLFILE];

	TRY
	{
		const std::string gl_filename = "waxglserver." + WaxGrafik::Creator::get_fileextension((enum WaxGrafik::GRAFIKTYP)typ) ;

		CadWerkauftrag w;

		
		string tmp = cadmodell_xml;

		// Debug Trace, letzte Anzeige
		ofstream out("trace.xml",ios_base::trunc | ios_base::out);
		out <<  tmp;
		out.close();

		w.set_xml_modell(tmp);

		WaxGrafik::Creator::create(&w, gl_filename);

		ifstream inp(gl_filename.c_str());

		memset(glchar, 0, MAXSIZEOFGLFILE);
		while (!inp.eof())
		{
			string str;
			inp >> str;
			if ((strlen(glchar) + str.length() + 1) > MAXSIZEOFGLFILE)
			{
				memset(glchar, 0, 64000);
				return glchar;
			}

			strcat(glchar, (str + " ").c_str());
		}
		return glchar;
	}
	CATCH_ALL(e)
	{
		return glchar;
	}
	END_CATCH_ALL
}


extern "C" LPCSTR PASCAL EXPORT convert2dwf(LPCSTR cadmodell_xml)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return convert(cadmodell_xml, WaxGrafik::DWF);
}


extern "C" LPCSTR PASCAL EXPORT convert2svg(LPCSTR cadmodell_xml)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return convert(cadmodell_xml, WaxGrafik::SVG);
}



extern "C" bool PASCAL EXPORT convert_terminate()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	WaxSys::unload();
	return true;
}

