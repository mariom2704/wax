#pragma once

class CGraficFileController
{
public:
	CString strSVGContent;
	CFile hPostFile;

public:
	CGraficFileController(void);
	~CGraficFileController(void);

	void WriteSVGContent();
	void CreateSVGFile(CString strFileName);

};

