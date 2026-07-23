#pragma once

extern "C" LPCSTR PASCAL EXPORT dummy();

extern "C" bool PASCAL EXPORT convert_init_waxsys();

extern "C" bool PASCAL EXPORT convert_init_stammdb();

extern "C" LPCSTR PASCAL EXPORT convert2dwf(LPCSTR cadmodell_xml);

extern "C" LPCSTR PASCAL EXPORT convert2svg(LPCSTR cadmodell_xml);

extern "C" LPCSTR PASCAL EXPORT convert(LPCSTR cadmodell_xml, int typ );

extern "C" bool PASCAL EXPORT convert_terminate();
